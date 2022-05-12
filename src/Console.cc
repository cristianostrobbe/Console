#include "Console.h"

using namespace Debug;

Console* StaticConsole::console = new Console();

/**
 * @brief Construct a new Console:: Console object
 * Intialize style and set start time.
 * 
 */
Console::Console()
{
    // Style
    header_color = GREEN;
    header_type  = BOLD;
    text_color   = DEFAULT;
    text_type    = NORMAL;

    start = time(0); // Open time
    srand (time(NULL)); // initialize random seed
    this->color.Random(); // Init RGB color
}

/**
 * @brief Construct a new Console:: Console object
 * 
 * @param head_color 
 * @param head_type 
 * @param txt_color 
 * @param txt_type 
 */
Console::Console(const int &head_color, 
                 const int &head_type, 
                 const int &txt_color, 
                 const int &txt_type)
        :header_color(head_color), header_type(head_type), text_color(txt_color), text_type(txt_type)
{
    start = time(0); // Open time
    srand (time(NULL)); // initialize random seed
    this->color.Random(); // Init RGB color
}

/**
 * @brief Destroy the Console:: Console object
 * 
 */
Console::~Console()
{
    Close();
}

/**
 * @brief 
 * 
 * @param name 
 * @param r_ 
 * @param g_ 
 * @param b_ 
 */
void Console::PushSubHeader(string name_, int r_, int g_, int b_)
{
    RGB* tmp_color = new RGB(r_, g_, b_);
    SubHeadName* tmp = new SubHeadName(name_, tmp_color);
    this->list_sub_head.push_back(tmp);
    sub_header = name_;
    Info(name_);
}

/**
 * @brief Close library. Close log file.
 * 
 */
void Console::Close()
{
    cout << endl;

    if (save_all_messages)
    {
        console_messages.close();
        save_all_messages = false;
    }
}

/**
 * @brief Set deafult meassage style.
 * 
 * @param head_color Header color (RED, YELLOW...)
 * @param head_type Header format (NORMAL, BOLD...)
 * @param txt_color Message color (RED, YELLOW...)
 * @param txt_type Meassage format (NORMAL, BOLD...)
 */
void Console::SetDefault(const int &head_color, 
                         const int &head_type, 
                         const int &txt_color, 
                         const int &txt_type)
{
    this->header_color = head_color;
    this->header_type  = head_type;
    this->text_color   = txt_color;
    this->text_type    = txt_type;
}

/**
 * @brief Save all the messages on a text file.
 * The filename is the datetime of creation.
 * 
 * @param path Absolute path.
 */
void Console::SaveAllMessages(string path)
{
    if (console_messages.is_open())
        return;
    
    // If path is not available put the log file
    // near the executable
    if (path == "")
    {
        char cwd[100];
        string wd = getcwd(cwd, 100);
        path = wd;
    }

    save_all_messages = true;

    // Init console messages
    string _name = GetDatetimeAsString();
    string filename = path + "/" + _name + ".log";

    if (path.back() == '/')
        filename = path + _name + ".log";

    try
    {
        console_messages.open(filename);
    }
    catch (const ifstream::failure& e)
    {
        save_all_messages = false;
        Error(e.what());
    }

    cout << "Log filename " << filename << endl;
}

/**
 * @brief Methods that build all messages and save them into log file.
 * Deafult, header is BOLD while text is normal. 
 * 
 * @param header Header content. What's inside [ header ];
 * @param head_color This is the header color.
 * @param text This is the text that you want to print.
 * @param txt_color This is the text color.
 */
void Console::Message(const string &header,
                      const int &head_color,
                      const string &text,
                      const int &txt_color)
{
    string message; // Buffer
    string timestamp = GetSystemTime();

    // Timestamp and message
    message += timestamp + " " + MakeTextColor(BOLD, FOREGROUND, head_color, MakeHeader(header));

    if (sub_header != "")
    {
        bool ok = false;
        string color_ = "";
        // Take sub-header color
        FindSubHeaderColor(list_sub_head, sub_header, color_);

        // @cstrobbe: or use a predefined color?
        if (color_ == "")
        {
            this->color.Random(); // Generate another random color
            color_ = this->color.str();
        }

        message += " " + MakeTextRGB(BACKGROUND, color_, MakeHeader(sub_header, subheader_width));
        sub_header = ""; // Clear header
    }

    // Message
    message += SEPARATOR + MakeTextColor(NORMAL, FOREGROUND, txt_color, text);
    
    // std::unique_lock<std::mutex> lock{writing_mtx};
    cout << message << endl;

    if (save_all_messages)
        console_messages << timestamp << SEPARATOR << MakeHeader(header) << SEPARATOR << text << endl;
}

/**
 * @brief Copy color code related to an old sub-header
 * 
 * @param vec Vector of sub-header
 * @param sub_header_ what you want to find
 * @param color_ output sub-header color
 */
void Console::FindSubHeaderColor(std::vector<SubHeadName*> &vec, string &sub_header_, string &color_)
{
    // std::unique_lock<std::mutex> lock{writing_mtx};
    for (SubHeadName* value : vec)
    {
        if (value->name == sub_header_)
        {
            color_ = value->color->str();
            break;
        }
    }  
}

/**
 * @brief Append argument to message string
 * 
 * @param s message you want to append to the final message
 */
void Console::QueueMessage(string s)
{
    // std::unique_lock<std::mutex> lock{writing_mtx};
    // this->msg_queue += s + " ";
    this->msg_queue += s;
};

/**
 * @brief Flush entire message
 * 
 */
void Console::FlushMessages()
{
    // std::unique_lock<std::mutex> lock{writing_mtx};
    Message(LOG, this->msg_queue);
    msg_queue = ""; // clear message buffer
};

/**
 * @brief MEssage constructor with deafult parameter like header abd text color.
 * 
 * @param header Header of your message
 * @param text Text message
 */
void Console::Message(const string &header,
                      const string &text)
{
    Message(header, header_color, text, text_color);
}

/**
 * @brief Default informatation message.
 * Example: HH:MM:SS.SSS [Info] text you want to print.
 * 
 * @param text 
 */
void Console::Info(const string &text)
{
    Message(INFO, header_color, text, text_color);
}

/**
 * @brief Information message with custom header content.
 * Example: HH:MM:SS.SSS [Head] text you want to print.
 * 
 * @param text 
 */
void Console::Info(const string &header, 
                   const string &text)
{
    Message(header, header_color, text, text_color);
}

/**
 * @brief Default error message
 * Example: HH:MM:SS.SSS [ERR ] text you want to print.
 * The header color is RED by deafult.
 * 
 * @param text 
 */
void Console::Error(const string &text)
{
    Message(ERROR, RED, text, text_color);
}

/**
 * @brief Default error message with custom header.
 * Example: HH:MM:SS.SSS [Head] text you want to print.
 * The header color is RED by deafult.
 * 
 * @param header 
 * @param text 
 */
void Console::Error(const string &header, 
                    const string &text)
{
    Message(header, RED, text, text_color);
}

/**
 * @brief This methods prints meassages that stays on the same terminal line.
 * If other wait messages are called then the previous message will be overwritten.
 * If other messages will be used then the wait message will be overwritten on the terminal
 * but not in the log file.
 * 
 * @param header Wait message header
 * @param text Thex of the message
 */
void Console::Wait(const string &header, 
                   const string &text)
{
    string message; // Buffer
    string timestamp = GetSystemTime();
    
    // Header
    message += timestamp + " " + MakeTextColor(BLINK, FOREGROUND, header_color, MakeHeader(header));
    // Message
    message += SEPARATOR + MakeTextColor(NORMAL, FOREGROUND, text_color, text);

    // std::unique_lock<std::mutex> lock{writing_mtx};
    // Reset terminal cursor right after the message.
    cout << message << RESET_CURSOR << std::flush;

    if (save_all_messages)
        console_messages << timestamp << SEPARATOR << MakeHeader(header) << SEPARATOR << text << endl;
}

/**
 * @brief Same as before but defaukt header content is INFO.
 * 
 * @param text 
 */
void Console::Wait(const string &text)
{
    Wait(INFO, text);
}

/**
 * @brief Default warning message. 
 * Example: HH:MM:SS.SSS [WARN] text you want to print.
 * Default header color is yellow.
 * 
 * @param text 
 */
void Console::Warn(const string &text)
{
    Message(WARN, YELLOW, text, text_color);
}

/**
 * @brief Clear vertical lines of the terminal.
 * 
 * @param lines Number of lines to clear.
 */
void Console::ClearTerminal(const int &lines)
{
    string cmd = std::string(NON_PRINT) + to_string(lines) + MOVE_UP + EOM;
    // std::unique_lock<std::mutex> lock{writing_mtx};
    cout << cmd;
}

/**
 * @brief System bell notification.
 * 
 */
void Console::BellNotification()
{
    // std::unique_lock<std::mutex> lock{writing_mtx};
    cout << (string)BELL << endl;
}

/**
 * @brief Show some methods.
 * 
 */
void Console::Test()
{
    const string text = "Now I'm here! Think I'll stay around around around...";
    Info(text);
    Info("QUEEN", "around");
    Error("QUEEN", "around");
    Warn("around");
    Wait("QUEEN", "around");
}

/**
 * @brief Get current date time as formatted string.
 * 
 * @return string datetime as d_m_Y__H_M_S
 */
string Console::GetDatetimeAsString()
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

/**
 * @brief Get precise day time as string.
 * 
 * @return string day time as H:M:S.SSS. 
 */
string Console::GetSystemTime()
{
    time_t rawtime;
    char buffer[80];
    struct tm * timeinfo;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    int ms_ = lrint(tv.tv_usec/1000.0);
    std::stringstream stream;
    stream << std::fixed << std::setprecision(3) << ms_;
    std::string ms = stream.str();
    ms.resize(3, '0'); // pad empty space

    // time (&rawtime);
    timeinfo = localtime(&tv.tv_sec);
    strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
    std::string str(buffer);
    str += "." + ms;

    return str;
}

/**
 * @brief Make message descriptor header given the header
 * content. The header width is 4 char by default.
 * 
 * @param header 
 * @return string 
 */
string Console::MakeHeader(string header, int width)
{
    if(width == -1)
        width = header_width;
    
    header.resize(width, ' ');
    return "[" + header + "]";
}

/**
 * @brief Build message. Set text mode (NORMAL, TEXT...) terminal_mode,
 * terminal color (RED, YELLOW...).
 * 
 * @param text_mode Text format NORMAL, BOLD, DIM, BLINK,...
 * @param terminal_mode What you want to color, TEXT or BACKGROUND.
 * @param terminal_color Text color RED, GREEN, BLUE, YELLOW...
 * @param text message
 * @return string formatted message
 */
string Console::MakeTextColor(const int &text_mode, 
                              const int &terminal_mode, 
                              const int &terminal_color, 
                              const string &text)
{
    string result = NON_PRINT + to_string(text_mode) + COMBINED + to_string(terminal_mode) + to_string(terminal_color) + EOC + text + EOM;
    return result;
}

/**
 * @brief Set text RGB color.
 * 
 * @param color_mode FOREGROUND or BACKGROUND
 * @param text_color RGB color as {r;g;b}
 * @param text Text you want to color
 * @return string Foramtted string
 */
string Console::MakeTextRGB(const int &color_mode,  
                            const string &text_color, 
                            const string &text)
{
    string result = NON_PRINT + to_string(color_mode) + "8" + MODE_RGB + text_color + EOC + text + EOM;
    return result;
}


/**
 * @brief Build message. Set text mode (NORMAL, TEXT...) terminal_mode,
 * terminal color (RED, YELLOW...) and background color.
 * 
 * @param text_mode Text format NORMAL, BOLD, DIM, BLINK,...
 * @param terminal_mode What you want to color, TEXT or BACKGROUND.
 * @param terminal_color  Text color RED, GREEN, BLUE, YELLOW...
 * @param text message
 * @param background_color Text background color RED, GREEN, BLUE, YELLOW...
 * @return string formatted message
 */
string Console::MakeTextColor(const int &text_mode, 
                              const int &terminal_mode, 
                              const int &terminal_color, 
                              const string &text, 
                              const int &background_color)
{
    string result = GetBgColorCode(background_color) + NON_PRINT + to_string(text_mode) + COMBINED + to_string(terminal_mode) + to_string(terminal_color) + EOC + text + EOM;
    return result;
}

/**
 * @brief Get background color code.
 * 
 * @param color desired color code.
 * @return string terminal color code
 */
string Console::GetBgColorCode(int color)
{
    string bg_color = NON_PRINT + to_string(4) + to_string(color);
    return bg_color;
}

/**
 * @brief Set terminal line background color.
 * 
 * @param color 
 */
void Console::SetBackgorundColor(int color)
{
    // std::unique_lock<std::mutex> lock{writing_mtx};
    string msg_ = GetBgColorCode(color) + "m";
    cout << msg_;
}

/**
 * @brief 
 * 
 * @param printable 
 * @return string 
 */
string Console::MakeNonPrintable(const string &printable)
{
    string non_printable = NON_PRINT + printable + NON_PRINT;
    return non_printable;
}

// DOES NOT WORK
void Console::ClearCurrentLine()
{
    // std::unique_lock<std::mutex> lock{writing_mtx};
    cout << CLEAR_LINE;
}

/**
 * @brief Get delta time between now and start of the library. 
 * 
 * @return double elapsed time as seconds
 */
double Console::get_dt()
{
    time_t now = time(0);
    double seconds = difftime(now, start);

    return seconds;
}

/**
 * @brief Move terminal cursor up by # lines.
 * 
 * @param lines
 */
void Console::MoveCursorUp(const int &lines)
{
    string cmd = string(NON_PRINT) + to_string(lines) + MOVE_UP + EOM;
    // std::unique_lock<std::mutex> lock{writing_mtx};
    cout << cmd;
}

/**
 * @brief Clear current terminal window.
 * 
 */
void Console::ClearWindow()
{
    string cmd = std::string(NON_PRINT) + CLEAR_DISPLAY + EOM;
    // std::unique_lock<std::mutex> lock{writing_mtx};
    cout << cmd;
}