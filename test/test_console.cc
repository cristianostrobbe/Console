#include "Console.h"

#include <unistd.h>

using namespace std;
using namespace Debug;

int main()
{
    Debug::Console Debug; // Default configuration
    // Debug::Console Debug(YELLOW, BOLD, DEFAULT, NORMAL); // Overwrite default configuration
    // Debug.SetDefault(YELLOW, BOLD, DEFAULT, NORMAL); // Overwrite default configuration

    // Enable write to log file
    Debug.SaveAllMessages(); // .log near executable file
    // Debug.SaveAllMessages("absolute/path"); // .log where you want

    bool running = true;
    while(running)
    {
        Debug.Test();
        usleep(100000); // 100ms
        Debug.Info("Debug");
        usleep(100000); // 100ms
        Debug.Info("Head", "Debug with header");
        usleep(100000); // 100ms
        Debug.Warn("Warn");
        usleep(100000); // 100ms
        Debug.Error("Error");
        usleep(100000); // 100ms
        Debug.Error("Head", "Error with header");
        usleep(100000); // 100ms
        Debug.Wait("Wait");

        Debug.Info("Enabling Reprint");
        for(int i = 0; i < 15; i++){
          Debug.Wait("Reprinting: " + to_string(i+1));
          usleep(100000);
        }


        // Close log file and disable logs writing.
        // Messages are only printed on terminal.
        // This functionaly is already embedded inside class destructor.
        // Debug.Close();

        running = false;
    }

}
