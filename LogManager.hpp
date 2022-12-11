#ifndef _LOG_MANAGER_HPP
#define _LOG_MANAGER_HPP

#include <vector>
#include <fstream>

using namespace std;

class LogManager
{
public:
    ofstream logFileStream;
    pthread_mutex_t mutex;

    LogManager(string logFilePath);
    void PrintToLog(string massage);
};

#endif
