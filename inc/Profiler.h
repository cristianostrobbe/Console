#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <iostream>

#include <thread>
#include <mutex>

struct ProfileResult
{
    std::string Name;
    long long Start, End;
    uint32_t ThreadID;
};

struct InstrumentationSession
{
    std::string Name;
};

class Instrumentor
{
private:
    InstrumentationSession* m_CurrentSession;
    std::ofstream m_OutputStream;
    int m_ProfileCount;
public:
    Instrumentor();
    ~Instrumentor();

    void BeginSession(const std::string& name, const std::string& filepath = "prof_result.json");

    void EndSession();

    void WriteProfile(const ProfileResult& result);

    void WriteHeader();

    void WriteFooter();

    static Instrumentor& Get()
    {
        static Instrumentor instance;
        return instance;
    }

    std::mutex mtx;
};

class InstrumentationTimer
{
public:
    InstrumentationTimer(const char* name);

    ~InstrumentationTimer();

    void Stop();
private:
    const char* m_Name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
    bool m_Stopped;
};


#define PROFILER_START(name) Instrumentor::Get().BeginSession(name);\
    Instrumentor& instrumentor_instance = Instrumentor::Get();
#define PROFILER_START_PATH(name, path) Instrumentor::Get().BeginSession(name, path);\
    Instrumentor& instrumentor_instance = Instrumentor::Get();
#define PROFILE(name) InstrumentationTimer timer(name);
#define PROFILE_END Instrumentor::Get().EndSession();
