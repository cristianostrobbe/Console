#ifndef LOGTIMES_H
#define LOGTIMES_H
/*
* Filippo Faccini - Cristiano Strobbe - Mar 2021
* Simple class for log code times.
*
* Changelog:
* v1.0 - Simple header files for log code times
* v2.0 - Improved functionalities. Fix class style.
*/

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <ctime>
#include <chrono>
#include <vector>
#include <iomanip>
#include <math.h>

using namespace std;
using namespace std::chrono;

namespace Debug
{
    class LogTimes
    {

    private:
        // Statistics vectors and matrix
        vector<chrono::steady_clock::time_point> times; // TODO template class??
        vector<string> keys;
        vector<double> d_times;
        vector<string> names;
        vector<vector<double>> delta_times;
        // vector<chrono::steady_clock::time_point> timesVector;

        // General
        bool is_init = false;
        string stat_header = "";
        string stat_header_csv = "";
        string header;
        bool header_ok = false;
        // Output file
        ofstream stat_log;
        bool is_open = false;
        string log_fl = "";
        string ext = ".logtimes"; // extension
        string line; // Store all times for each cycle

        /**
         * @brief Get the Datetime object
         * 
         * @return string 
         */
        string GetDatetime();

        /**
         * @brief Create a Stat Header object
         * 
         */
        void CreateStatHeader();

        // DEBUG
        /**
         * @brief 
         * 
         */
        void PrintDtimes();

        // DEBUG - Print delta_times
        /**
         * @brief 
         * 
         */
        void PrintTimeMatrix();


    public:

        /**
         * @brief Construct a new Log Times object
         * 
         */
        // LogTimes();

       /**
         * @brief Construct a new Log Times object
         * 
         */
        LogTimes(string path="", string proc_name="");

        /**
         * @brief Destroy the Log Times object
         * 
         */
        ~LogTimes();

        /**
         * @brief 
         * 
         */
        void Close();

        /**
         * @brief Store times by key values
         * 
         * @param key 
         */
        void StartByKey(string key);

        /**
         * @brief 
         * 
         * @param key_ 
         */
        void StopByKey(string key_);

        /**
         * @brief 
         * 
         * @param key
         * @param value 
         */
        void AddKeyAndValue(string key, double value);


        /**
         * @brief Get time differences
         * 
         * @param store 
         */
        void SumDtByKey(bool store=true);

        /**
         * @brief Set the Header object
         * 
         * @param header_ 
         */
        void SetHeader(string log_header);

        /**
         * @brief Get the Stat Header object
         * 
         * @return string 
         */
        string GetStatHeader();

        /**
         * @brief Get the Mean Values object
         * 
         * @return string 
         */
        string GetMeanValues();

        /**
         * @brief Get the Mean And Std Dev object
         * 
         * @return std::vector<string> 
         */
        std::vector<string> GetMeanAndStdDev();

        // string GetCurrentTimes();

        // vector<string> GetCurrentTimesVec();

        // void getTimes();

    };

}; // namespace debug

#endif
