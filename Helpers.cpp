#include "Helpers.hpp"
#include "LogManager.hpp"

extern Bank *bank;
extern LogManager *logManager;
extern vector<ATM *> *ATMs;

void Helpers::EndProgramWithPERROR(string errorMessage)
{
    perror(errorMessage.c_str());
    delete logManager;
    delete bank;
    Helpers::deleteATMsVector();
    exit(EXIT_FAILURE);
}

void Helpers::EndProgramWithSTDERR(string errorMessage)
{
    fprintf(stderr, "%s", errorMessage.c_str());
    delete logManager;
    delete bank;
    Helpers::deleteATMsVector();
    exit(EXIT_FAILURE);
}

vector<ATM*>* Helpers::InitializeATMsVector(int argc, char* argv[])
{
    vector<ATM*>* ATMs = new vector<ATM*>();
    for (int currentATMIndex = 1; currentATMIndex < argc; currentATMIndex++)
    {
        ifstream currentATMFile(argv[currentATMIndex]);
        int ATMID = GetATMIDFromFileName(argv[currentATMIndex]);
        if(!currentATMFile.is_open())
        {
            EndProgramWithSTDERR("Bank error: illegal arguments\n");
        }
        ATM *currentATM = new ATM(currentATMFile, ATMID);
        ATMs->push_back(currentATM);
        currentATMFile.close();
    }
    return ATMs;
}

void Helpers::deleteATMsVector()
{
    for (size_t currentATMIndex = 0; currentATMIndex < ATMs->size(); currentATMIndex++)
    {
        delete (*ATMs)[currentATMIndex];
    }
    delete ATMs;
}

void Helpers::JoinAllATMsThreads(vector<ATM*>* ATMs)
{
    for (size_t currentATMIndex = 0; currentATMIndex < ATMs->size(); currentATMIndex++)
    {
        if(pthread_join((*ATMs)[currentATMIndex]->ATMRunThread, NULL) != 0)
        {
            EndProgramWithPERROR("Bank error: pthread_join failed\n");
        }
    }
}

int Helpers::GetATMIDFromFileName(string fileName)
{
    int ATMID = 0;
    bool foundNumber = false;
    for (size_t currentCharIndex = 0; currentCharIndex < fileName.length(); currentCharIndex++)
    {
        if((fileName[currentCharIndex] >= '0') && (fileName[currentCharIndex] <= '9'))
        {
            foundNumber = true;
            ATMID = ATMID * 10 + (fileName[currentCharIndex] - '0');
        }

        else if(foundNumber)
        {
            break;
        }
    }
    return ATMID;
}
