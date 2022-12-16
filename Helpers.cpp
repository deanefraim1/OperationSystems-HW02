#include "Helpers.hpp"
#include "LogManager.hpp"

extern Bank *bank;
extern LogManager *logManager;
extern vector<ATM> *ATMs;

void Helpers::EndProgramWithPERROR(string errorMessage)
{
    perror(errorMessage.c_str());
    exit(EXIT_FAILURE);
}

void Helpers::EndProgramWithSTDERR(string errorMessage)
{
    fprintf(stderr, "%s", errorMessage.c_str());
    exit(EXIT_FAILURE);
}

vector<ATM> Helpers::InitializeATMsVector(int argc, char* argv[])
{
    vector<ATM> ATMs;
    for (int currentATMIndex = 1; currentATMIndex < argc; currentATMIndex++)
    {
        ifstream currentATMFile(argv[currentATMIndex]);
        if(!currentATMFile.is_open())
        {
            EndProgramWithSTDERR("Bank error: illegal arguments\n");
        }
        ATMs.push_back(ATM(currentATMFile));
        currentATMFile.close();
    }
    return ATMs;
}

void Helpers::JoinAllATMsThreads(vector<ATM> ATMs)
{
    for (size_t currentATMIndex = 0; currentATMIndex < ATMs.size(); currentATMIndex++)
    {
        if(pthread_join(ATMs[currentATMIndex].ATMRunThread, NULL) != 0)
        {
            EndProgramWithPERROR("Bank error: pthread_join failed\n");
        }
    }
}