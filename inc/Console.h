#pragma once
/*
* Cristiano Strobbe - Feb 2020
* Cool console debug messages
*
* Changelog:
* v1.0 - Simple header files for cool terminal messages.
* v1.1 - Added wait message and clear window functionality.
* v2.0 - No more single header file. Added message system time.
         Fix class style.
* v2.1 - Operator <<, () and [] overload. Added singleton. 
*/

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <string>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <ctime>
#include <math.h>
#include <mutex>
#include <time.h>
#include <vector>
#include <algorithm>
#include <sstream>
#include <sys/time.h> // gettimeofday

#include "TerminalCommads.h"

using namespace std;

namespace Debug
{
    // RGB color struct
    struct RGB
    {
        // Default random color
        RGB(){ Random();};

        RGB(int r_, int g_, int b_)
        { 
            r = r_;
            g = g_;
            b = b_;
        };

        int r,g,b; // Values
        
        // Get RGB color as cmd string
        string str() const
        {
            return to_string(r) + COMBINED + to_string(g) + COMBINED + to_string(b);
        };

        // Set random color
        void Random()
        {
            r = rand()/(RAND_MAX/255.0);
            g = rand()/(RAND_MAX/255.0);
            b = rand()/(RAND_MAX/255.0);
        };

        // // Set random color Red-ish
        // void Random(double offset_=50.0)
        // {
        //     r = 255;
        //     g = rand()/(RAND_MAX/(255.0-offset_));
        //     // g = rand()/(RAND_MAX/(255.0-offset_)) + offset_;
        //     b = 0;
        // };
    };

    class Console
    {
        public:
            // Defualt constructor.
            Console();

            // Initiale Console with non default configuration.
            Console(const int &head_color, 
                    const int &head_type, 
                    const int &txt_color, 
                    const int &txt_type);
                            
            // Class destructor
            ~Console();

            // Close output log file
            void Close();

            // TODO: Put carriage return at the begin of the line
            void SetDefault(const int &head_color, 
                            const int &head_type, 
                            const int &txt_color, 
                            const int &txt_type);

            // Save all console messages inside a .log file
            void SaveAllMessages(string path="");

            // Make private?
            void Message(const string &header,
                         const int &head_color,
                         const string &text,
                         const int &txt_color);

            // Message with default arguments
            void Message(const string &header,
                         const string &text);

            // Set header
            // Exampple: HH:MM:SS.sss [HEAD] [SUB_HEADER] text ...
            Console& SetHeader(const string &header_)
            {
                header = header_;
                return *this;
            }

            // Push default SubHeader with color
            void PushSubHeader(string name, int r_, int g_, int b_);

            // Set sub-header
            // Exampple: HH:MM:SS.sss [HEAD] [SUB_HEADER] text ...
            string SetSubHeader(const string &header)
            {
                // std::unique_lock<std::mutex> lock(writing_mtx);
                bool ok = false;
                for (SubHeadName* value : list_sub_head)
                {
                    if (value->name == header)
                    {
                        ok = true;
                        break;
                    }
                }

                if (!ok)
                {
                    RGB* tmp_color = new RGB();
                    SubHeadName* tmp = new SubHeadName(header, tmp_color);
                    list_sub_head.push_back(tmp);
                }

                sub_header = header;
                return "";
            }

            /* ---------------- Debug messages ---------------- */
            // Default informatation message.
            void Info(const string &text);
            
            // Information message with custom header content.
            void Info(const string &header,
                      const string &text);

            /* ---------------- Error messages ---------------- */
            // Default error message.
            void Error(const string &text);

            // Default error message with custom header.
            void Error(const string &header, 
                       const string &text);
            
            /* ---------------- Wait messages ---------------- */
            // Hold 
            void Wait(const string &text);
            
            // Hold??
            void Wait(const string &header, 
                      const string &text);

            // Default warning message. 
            void Warn(const string &text);
      
            // Ring system bell.
            void BellNotification();

            // Test function
            void Test();

            // Class operator
            template <class T>
            Console& operator<<(const T &in)
            {
                stringstream ss;
                ss << std::fixed << in; // 6 pos. precision

                std::size_t idx = ss.str().find("\n");
                if(idx != string::npos)
                {
                    QueueMessage(ss.str().substr(0, idx));
                    FlushMessages();
                }
                else
                {
                    QueueMessage(ss.str());
                }
                return *this;
            };

            string begin()
            {
                operator_mtx.lock();
                return "";
            }
            string end()
            {
                // this->msg_queue += "\n"
                FlushMessages();
                operator_mtx.unlock();
                return "";
            }

            // Operator overload. Default log message.
            void operator()(const string &rhs) 
            {                           
                Message(LOG, rhs);
            };

            // Operator overload. Set sub_header message
            Console& operator[](const string &rhs) 
            {   
                SetSubHeader(rhs);
                return *this;
            };

        private:
            // Runtime variables
            int header_color = YELLOW;
            int header_type  = BOLD;
            int text_color   = DEFAULT;
            int text_type    = NORMAL;
            int header_width    {4};
            int subheader_width {10};
            bool save_all_messages { false };
            time_t start; // Library start time
            bool is_first_time { true }; // Used to print header once.
            string msg_queue;  // Temporary message value. Cleared every message.
            string header;     // Temporary header value. Cleared every message. TODO(cstrobbe)
            string sub_header; // Temporary sub-header value. Cleared every message.
            RGB color;         // Current color
            mutex writing_mtx; // Cout mutex
            mutex operator_mtx; // Cout mutex
            // Output log file
            ofstream console_messages; // Store all console messages

            // Sub-header list
            struct SubHeadName
            {
                SubHeadName(const string &sub_header_, RGB *color_) 
                            : name(sub_header_), color(color_)
                {};

                string name=""; // Sub header name
                RGB* color; // Current color
            };
            // List of avaialble sub-header
            vector<SubHeadName*> list_sub_head;


            // Customize the entire mnessage. From text mode and color to
            // header mode and color.
            string MakeTextColor(const int &text_mode,
                                 const int &terminal_mode,
                                 const int &terminal_color,
                                 const string &text);

            // Make text RGB 
            string MakeTextRGB(const int &color_mode,  
                               const string &text_color,
                               const string &text);

            // Customize the entire mnessage. From text mode and color to
            // header mode and color. You can also set text background color.
            string MakeTextColor(const int &text_mode,
                                 const int &terminal_mode,
                                 const int &terminal_color,
                                 const string &text,
                                 const int &background_color);

            // Copy color code related to sub-header
            void FindSubHeaderColor(std::vector<SubHeadName*> &vec, string &sub_header_, string &color_);
            

            // Append argument to message string
            void QueueMessage(string s);

            // Flush entire message
            void FlushMessages();

            // Create custom [XXXX] message descriptors
            string MakeHeader(string header, int width=-1);

            // Get backgrpund color code as non printable terminal command
            string GetBgColorCode(int color=DEFAULT);

            // Set text backgorund color
            void SetBackgorundColor(int color=DEFAULT);

            // Make printable into non printable. Useful to reate terminal commands.
            string MakeNonPrintable(const string &printable);

            // TO BE FIXED
            void ClearCurrentLine();

            // Get deltatime between now and when the log file started.
            double get_dt();

            // Move terminal cursor up by # lines 
            void MoveCursorUp(const int &lines);
            
            // Clear terminal
            void ClearWindow();

            // Clear # lines of the terminal
            void ClearTerminal(const int &lines);

            // Get current datetime formatted as string
            string GetDatetimeAsString();

            // Get system time as "hh:mm:ss.sss"
            string GetSystemTime(); 

    };

    // Singleton implementation
    class StaticConsole : public Console
    {
        public:
            static Console& Get()
            {
                return (*console);
            };

        private:
            static Console* console;
        
    };
}; // namespace debug

#define LOGGAMELO Debug::StaticConsole::Get()
// #define WARNAMELO Debug::StaticConsole::Warn() // TODO: @cstrobbe

#define PUSH_SUBHEADER_STYLE(header,r_,g_,b_) Debug::StaticConsole::Get().PushSubHeader(header,r_,g_,b_)

#define INI Debug::StaticConsole::Get().begin()
#define FINI Debug::StaticConsole::Get().end()

#define SAFELOG LOGGAMELO << INI
#define HEADERLOG(header) LOGGAMELO << INI << LOGGAMELO.SetSubHeader(header)

#define LOGHERE LOGGAMELO << INI << LOGGAMELO.SetSubHeader(__PRETTY_FUNCTION__)