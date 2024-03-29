#include "Helpers.hpp"
#include "LogManager.hpp"

extern Bank *bank;
extern LogManager *logManager;
extern vector<ATM *> *ATMs;

void Helpers::EndProgramWithPERROR(string errorMessage)
{
    printf("\033[2J"); //clear screen
    printf("\033[1;1H");//moves cursor to beginning of screen
    fflush(stdout);
    perror(errorMessage.c_str());
    exit(EXIT_FAILURE);
}

void Helpers::EndProgramWithSTDERR(string errorMessage)
{
    printf("\033[2J"); //clear screen
    printf("\033[1;1H");//moves cursor to beginning of screen
    fflush(stdout);
    fprintf(stderr, "%s", errorMessage.c_str());
    exit(EXIT_FAILURE);
}

vector<ATM*>* Helpers::InitializeATMsVector(int argc, char* argv[])
{
    vector<ATM*>* ATMs = new vector<ATM*>();
    for (int currentATMIndex = 1; currentATMIndex < argc; currentATMIndex++)
    {
        ifstream currentATMFile(argv[currentATMIndex]);
        if(!currentATMFile.is_open())
        {
            EndProgramWithSTDERR("Bank error: illegal arguments\n");
        }
        ATM *currentATM = new ATM(currentATMFile, currentATMIndex);
        ATMs->push_back(currentATM);
        currentATMFile.close();
    }
    return ATMs;
}

void Helpers::deleteATMs()
{
    if(ATMs == NULL)
        return;
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

void Helpers::EnterWritersSorted(Account* firstAccount, Account* secondAccount)
{
    if(firstAccount->id < secondAccount->id)
    {
        firstAccount->EnterWriter();
        secondAccount->EnterWriter();
    }
    else
    {
        secondAccount->EnterWriter();
        firstAccount->EnterWriter();
    }
}

void Helpers::EnterReaderAllAccounts()
{
    for (size_t currentAccountIndex = 0; currentAccountIndex < bank->accounts.size(); currentAccountIndex++)
    {
        bank->accounts[currentAccountIndex]->EnterReader();
    }
}

void Helpers::EnterWriterAllAccounts()
{
    for (size_t currentAccountIndex = 0; currentAccountIndex < bank->accounts.size(); currentAccountIndex++)
    {
        bank->accounts[currentAccountIndex]->EnterWriter();
    }
}

void Helpers::ExitReaderAllAccounts()
{
    for (size_t currentAccountIndex = 0; currentAccountIndex < bank->accounts.size(); currentAccountIndex++)
    {
        bank->accounts[currentAccountIndex]->ExitReader();
    }
}

void Helpers::ExitWriterAllAccounts()
{
    for (size_t currentAccountIndex = 0; currentAccountIndex < bank->accounts.size(); currentAccountIndex++)
    {
        bank->accounts[currentAccountIndex]->ExitWriter();
    }
}
