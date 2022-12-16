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
}

LogManager::~LogManager()
{
    logFileStream.close();
}

void LogManager::PrintToLog(string massage)
{
    EnterWriter();
    logFileStream << massage << endl;
    ExitWriter();  
}