#include "LogManager.hpp"
#include "Bank.hpp"
#include "Helpers.hpp"

extern Bank *bank;
extern LogManager *logManager;

LogManager::LogManager(string logFilePath)
{
    logFileStream.open(logFilePath);
    if(!logFileStream.is_open())
        Helpers::EndProgramWithSTDERR("Bank error: can't open log file\n");
    
    if (pthread_mutex_init(&mutex, NULL) != 0)
        Helpers::EndProgramWithPERROR("Bank error: pthread_mutex_init failed\n");
}

LogManager::~LogManager()
{
    logFileStream.close();
    pthread_mutex_destroy(&mutex);
}

void LogManager::PrintToLog(string massage)
{
    pthread_mutex_lock(&mutex);
    logFileStream << massage << endl;
    pthread_mutex_unlock(&mutex);    
}