#ifndef _LOG_MANAGER_HPP
#define _LOG_MANAGER_HPP

#include <vector>
#include <fstream>
#include "IThreadSafe.hpp"

using namespace std;

class LogManager : public IThreadSafe
{
public:
    ofstream logFileStream;

    LogManager(string logFilePath);
    ~LogManager();
    void PrintToLog(string massage);
};

#endif
