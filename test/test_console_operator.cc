#include "Console.h"
#include "test.h"

using namespace std;
// using namespace Debug;

int main()
{
    // Debug::Console Debug; // Default configuration
    Debug::Console Debug(YELLOW, BOLD, DEFAULT, NORMAL); // Overwrite default configuration
    // Debug.SetDefault(YELLOW, BOLD, DEFAULT, NORMAL); // Overwrite default configuration

    // Enable write to log file
    // Debug.SaveAllMessages(); // .log near executable file
    // Debug.SaveAllMessages("absolute/path"); // .log where you want
    
    // Singleton usage.
    Debug::StaticConsole::Get().SaveAllMessages(); // Defualt near executable that initiate first.
    // Debug::StaticConsole::Get().SaveAllMessages("absolute/path");
    
    // Set default text style as header color, header formatting, text color, text formatting
    Debug::StaticConsole::Get().SetDefault(YELLOW, BOLD, DEFAULT, NORMAL);

    bool running = true;
    while(running)
    {
        // 1) Test << operator. Single string. 
        Debug << "Method #1";
        Debug << "Schei e paura mai avui\n";

        // Same but using singleton
        LOGGAMELO << "Schei e paura mai avui\n";

        // 2) Test multiple strings and types. Remeber carriage retrun.
        Debug << "Method #2";
        Debug << "Schei e paura mai avui" << 10 << "e mai li avro'" << 200.1234560 << "\n";

        // Same but using singleton and give __PRETTY_FUNTION__ as sub-header argument
        LOGHERE << "Schei e paura mai avui" << 10 << "e mai li avro'" << 200.1234560 << "\n";

        // 3) With open brackets
        Debug("Method #3");
        Debug("Schei e paura mai avui");

        // 4) Define your string and print it
        Debug("Method #4");
        string msg = (string)"Schei e paura" + " mai avui";
        Debug(msg);

        // 5) Define log message with sub header [] operator
        Debug["Method #5"] << "Schei e paura mai avui" << "e mai li avro'\n";
        // 5.1) Operator another operator
        Debug["Method #5"]("Schei e paura mai avui e mai li avro'");

        // A little bit of RGB pride
        for (int i=0; i<25;i++)
        {
            Debug["Call #" + to_string(i)]("Schei e paura mai avui e mai li avro'");
        }

        // Test sub-header color memory
        Debug << "Test sub-header color memory\n";
        for (int i=0; i<10;i++)
        {
            Debug["Call #" + to_string(i)]("Schei e paura mai avui e mai li avro'");
        }

        running = false;
    }
}
