#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <typeinfo>
#include <vector>
#include <fstream>
#include "ATM.hpp"
#include "Bank.hpp"
#include "LogManager.hpp"

using namespace std;

Bank *bank;
LogManager *logManager;

int main(int argc, char* argv[])
{
    bank = new Bank();
    logManager = new LogManager("log.txt");
    vector<ATM> ATMs;

    for (int currentATMIndex = 1; currentATMIndex < argc; currentATMIndex++)
    {
        ifstream currentATMFile(argv[currentATMIndex], ios::in);
        ATMs.push_back(ATM(currentATMFile));
    }

    pthread_join(bank->thread, NULL);

    for (size_t currentATMIndex = 0; currentATMIndex < ATMs.size(); currentATMIndex++)
    {
        pthread_join(ATMs[currentATMIndex].thread, NULL);
    }
    
    return 0;
}