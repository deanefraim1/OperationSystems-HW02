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

int main(int argc, char* argv[])
{
    if(argc < 2)
        Helpers::EndProgramWithSTDERR("Bank error: illegal arguments\n");
    
    bank = new Bank();
    logManager = new LogManager("log.txt");
    vector<ATM> ATMs = Helpers::InitializeATMsVector(argc, argv);

    Helpers::JoinAllATMsThreads(ATMs);

    delete logManager;
    delete bank;
    
    return 0;
}