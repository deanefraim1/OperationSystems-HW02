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
#include "Helpers.hpp"

using namespace std;

Bank *bank;
LogManager *logManager;
vector<ATM *> *ATMs;

int main(int argc, char* argv[])
{
    srand(time(NULL)); //random seed
    if(argc < 2)
        Helpers::EndProgramWithSTDERR("Bank error: illegal arguments\n");
    
    bank = new Bank();
    logManager = new LogManager("log.txt");
    ATMs = Helpers::InitializeATMsVector(argc, argv);

    Helpers::JoinAllATMsThreads(ATMs);

    delete bank;
    delete logManager;
    Helpers::deleteATMs();

    return 0;
}
