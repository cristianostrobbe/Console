#include "LogTimes.h"

using namespace Debug;


LogTimes::LogTimes(string path, string proc_name)
{
    // If path is not available put the log file
    // near the executable
    if (path == "")
    {
        char cwd[100];
        string wd = getcwd(cwd, 100);
        cout << "Log path: " << wd << endl;
        path = wd;
    }

    if (proc_name != "")
        proc_name.insert(0,1,'_');

    string _name = GetDatetime();
    log_fl = _name + proc_name + ext;
    string filename = path + "/" + log_fl;

    if (path.back() == '/')
        filename = path + log_fl;

    is_open = false;
    try
    {
        stat_log.open(filename);
        is_open = true;
    }
    catch (const ifstream::failure& e)
    {
        cout << e.what() << endl;
        return;
    }

    if (header_ok)
        SetHeader(header);
}


LogTimes::~LogTimes()
{
    Close();
}

void LogTimes::Close()
{
    if (is_open)
    {
        stat_log << endl;
        stat_log.close();
        cout << "Log " << log_fl << " closed" << endl;
    }
}


void LogTimes::StartByKey(string key)
{
    times.push_back(steady_clock::now());
    keys.push_back(key);
}


void LogTimes::StopByKey(string key_)
{
    steady_clock::time_point t_stop  = steady_clock::now();
    steady_clock::time_point t_start = t_stop;
    bool is_ok = false;

    // Get t_start by key
    int index = 0;
    for (string key : keys)
    {
        if (key == key_)
        {
            t_start = times[index];
            times.erase(times.begin() + index); // Pop values
            keys.erase(keys.begin() + index);
            is_ok = true;
            //break;
        }
        index++;
    }

    if (!is_ok)
    {
        cerr << "Wrong time key!" << endl;
        exit(0);
    }

    double dt = duration_cast<duration<double, milli>>(t_stop - t_start).count();
    d_times.push_back(dt);
    names.push_back(key_);
}


void LogTimes::AddKeyAndValue(string key, double value)
{
    names.push_back(key);
    d_times.push_back(value);
}


void LogTimes::SumDtByKey(bool store)
{
    CreateStatHeader();
    //PrintDtimes(); // DEBUG

    if (store)
    {
        string values_csv = "";
        for (double value : d_times)
            values_csv += to_string(value) + ",";
        
        values_csv.pop_back(); // remove last comma

        stat_log << values_csv << endl;
    }

    delta_times.push_back(d_times);
    d_times.clear();
}


void LogTimes::SetHeader(string log_header)
{
    header = log_header;
    header_ok = true;

    if (is_open)
        stat_log << log_header << endl;
}

string LogTimes::GetStatHeader()
{
    return stat_header;
}

string LogTimes::GetDatetime()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d_%m_%Y__%H_%M_%S", timeinfo);
    std::string str(buffer);

    return str;
}

void LogTimes::CreateStatHeader()
{
    if (!is_init)
    {
        for (string value : names)
        {
            stat_header_csv += value + ",";
            value.resize(8, ' '); // Column of 8 chars
            stat_header += value + " ";
        }
        stat_header_csv.pop_back(); // remove last comma

        stat_log << stat_header_csv << endl;
        is_init = true;
    }
}

string LogTimes::GetMeanValues()
{
    // PrintTimeMatrix(); // DEBUG
    string mean_values = "";

    int rows = delta_times.size();
    int cols = delta_times[0].size(); // n_samples

    vector<double> mean_val;

    // Sum colums
    for (int i=0; i<cols; i++)
    {
        double row_sum = 0.0;
        for (int j=0; j<rows; j++)
        {
            row_sum += delta_times[j][i];
        }
        mean_val.push_back(row_sum/rows);
        string tmp_str = to_string(row_sum/rows);
        tmp_str.resize(8, ' '); // Resize to display
        mean_values += tmp_str + " ";
        //cout << to_string(row_sum/rows) << " ";
    }

    // Reset
    delta_times.clear();
    // names.clear();
    // is_init = false;

    return mean_values;
}

std::vector<string> LogTimes::GetMeanAndStdDev()
{
    // PrintTimeMatrix(); // DEBUG
    string mean_values_str = "";
    // string variances_str = "";
    string std_devs_str    = "";

    int rows = delta_times.size();
    int cols = delta_times[0].size(); // n_samples

    vector<double> mean_val;

    // Sum colums
    for (int i=0; i<cols; i++)
    {
        double row_sum = 0.0;
        for (int j=0; j<rows; j++)
        {
            row_sum += delta_times[j][i];
        }
        mean_val.push_back(row_sum/rows);
        string tmp_str = to_string(row_sum/rows);
        tmp_str.resize(8, ' '); // Resize to display
        mean_values_str += tmp_str + " ";
    }

    // vector<double> vars;
    vector<double> std_devs;

    // Sum colums
    for (int i=0; i<cols; i++)
    {
        double row_squared_sum = 0.0;
        for (int j=0; j<rows; j++)
        {
            row_squared_sum += ( (delta_times[j][i] - mean_val[i]) * (delta_times[j][i] - mean_val[i]) );
        }
        double std_dev = sqrt(row_squared_sum/rows);
        // vars.push_back(row_squared_sum/rows);
        std_devs.push_back(std_dev);
        string tmp_str = to_string(std_dev);
        tmp_str.resize(8, ' '); // Resize to display
        std_devs_str += tmp_str + " ";
    }

    // Reset
    delta_times.clear();
    // names.clear();
    // is_init = false;

    return {mean_values_str, std_devs_str};
}

/* -------------------- DEBUG  -------------------- */

void LogTimes::PrintDtimes()
{
    for (double dt : d_times)
        cout << to_string(dt) << " - ";
    cout << endl;
}


void LogTimes::PrintTimeMatrix()
{
    for (vector<double> line : delta_times)
    {
        for (double value : line)
            cout << to_string(value) << " ";
        cout << endl;
    }
}