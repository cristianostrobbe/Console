#include "LogTimes.h"

#include <unistd.h>
#include <thread>

using namespace std;
using namespace Debug;

int main()
{
    // Default configuration. Log file named as datetime and placed near executable.
    Debug::LogTimes Log;
    // Debug::LogTimes Log(""); // Same as previous
    // Default configuration with "test" at the end of the log filename and near executable.
    // Debug::LogTimes Log("", "test"); 

    bool running = true;
    while(running)
    {
        // Test Start and Stop
        for (int i=0; i<10; i++)
        {
            // --------------------------- START STOP --------------------------- //
            // Measure time difference between two keys
            Log.StartByKey("Gne");
            // Do something you want to measure
            this_thread::sleep_for(chrono::microseconds(100000)); // sleep 100ms
            Log.StopByKey("Gne");
            // Note: Stop can be called where you want but pay attention, this
            // library is not suitable for multi-threading application. 

            // --------------------------- KEY VALUE ---------------------------- //
            // Add key and value without measuring time difference
            Log.AddKeyAndValue("Oho", 420.0);

            
            // ---------------------- SUM KEYS DELTA TIMES ---------------------- //
            // Save current times. 
            // Must be called every cycle otherwise same key will recognized as 
            // new key and value. If, for example, the call is placed at the end of 
            // the code then the library will show you all the single values.
            // So, mean will be the value and standard deviation will be 0.
            Log.SumDtByKey();

            // ------------------------------------------------------------------ //
            // Note: If another value will be added after calling SumDtByKey
            // then the new key will be associated to the first key you added.
            // No matter which key you used.
            // --------------------------- BAD TEST ----------------------------- //
            // // Bad test - Add value after calling SumDtByKey().
            // Log.StartByKey("Ohoh");
            // this_thread::sleep_for(chrono::microseconds(50000)); // sleep 50ms
            // Log.StopByKey("Ohoh");
            // Log.SumDtByKey();
            // ------------------------------------------------------------------ //
        }

        // --------------------------- GET STAT ----------------------------- //
        // Once you call GetMeanValues or GetMeanAndStdDev the library computes
        // mean values and/or standard deviation for all the key and values
        // that you append. 
        // string mean_val = Log.GetMeanValues(); // Get mean values for each key
        vector<string> mean_val = Log.GetMeanAndStdDev(); // Get mean value and std. dev. for each key
        string header   = Log.GetStatHeader();

        // Print
        cout << header   << endl;
        cout << "Mean values" << endl;
        cout << mean_val[0] << endl;
        cout << "Standard deviations" << endl;
        cout << mean_val[1] << endl;

        running = false;
    }

    // Log.Close(); // Will force output log to be closed but this is not mandatory.

}
