#include "LogManager.hpp"


LogManager::LogManager(string logFilePath)
{
    logFileStream.open(logFilePath);
    pthread_mutex_init(&mutex, NULL);
}

void LogManager::PrintToLog(string massage)
{
    pthread_mutex_lock(&mutex);
    logFileStream << massage << endl;
    pthread_mutex_unlock(&mutex);    
}