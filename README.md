# Console
An easy to use C++ library that allows you to print colorful terminal messages, navigate inside your folder and list files, measure execution time and generate profiler file.

# Dependecies
This library depends on:
- boost filesystem
- [nlohman json](https://github.com/nlohmann/json)

# Installation
1. Install dependecies
On __MacOS__:
~~~bash
brew install nlohmann-json
brew install boost
~~~
On __Ubuntu__
~~~bash
sudo apt install nlohmann-json3-dev
sudo apt-get install libboost-all-dev
~~~


This project contains multiple useful libraries:
- [Console](console)
- [Browse](browse)
- [LogTimes](log-times)
- [Profiler](profiler)


## Console
Easy way to print something on the terminal.

### Usage
~~~c++

~~~
### Static usage
~~~c++

~~~

## Browse
Allows browing from terminal files in system.  

### Usage
~~~c++
Browse b;
b.SetSelectionType(SelectionType::sel_all);
b.Start();

b.SetMaxSelections(1);
b.SetExtension(".log");
b.SetSelectionType(SelectionType::sel_file);

b.Start();
~~~

Simple as that.
#### Options
- Extension
- Selection type
- Max selections

These are the three possible selection types:  
Every file type, only folders or only files.
~~~c++
SelectionType::sel_all;
SelectionType::sel_folder;
SelectionType::sel_file;
~~~


This function sets the only extension accepted when selecting files
~~~c++
SetExtension(".json");
~~~

To set a maximum number of files/folders selected:
~~~c++
SetMaxSelections(number);
~~~


## Profiler
Tool to time functions and output data to a json formatted file.
The result can be opened in Google Chorme tool.

### Usage
Initialize the profiler class:
~~~c++
PROFILER_START("Test")
PROFILER_START_PATH("Test","/home/res.json")               // To specify output path
// OR
Instrumentor::Get().BeginSession("Test");
Instrumentor::Get().BeginSession("Test","/home/res.json"); // To specify output path
~~~

***These two lines do the same thing, one uses macros.***

To time a block of code, place:
~~~c++
PROFILE("timer1")
// OR
InstrumentationTimer timer("timer1");
~~~
This code will time all the scope, so an example can be like this:
~~~c++
{
  PROFILE("Scope")
  function1();
}
function2();
~~~
Here only function1 will be profiled because is in the same scope of **PROFILE**

To end a session:
~~~c++
PROFILE_END
// Same as
Instrumentor::Get().EndSession();
~~~

### Displaying result
Open Google Chrome, in the topbar type ***chrome://tracing/***.

From there, load prof_result.json.
