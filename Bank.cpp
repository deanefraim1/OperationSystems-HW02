#include "Bank.hpp"
#include "LogManager.hpp"
#include "ATM.hpp"
#include <string>
#include <unistd.h>
#include <iostream>

extern LogManager *logManager;

using namespace std;

Bank::Bank()
{
    pthread_create(&commissionThread, NULL, Bank::RunBankCommision, this);
    pthread_create(&statusThread, NULL, Bank::RunBankStatus, this);
}

Bank::~Bank()
{
    
}

void *Bank::RunBankCommision(void *bankToRunAsVoid)
{
    Bank *bankToRun = (Bank *) bankToRunAsVoid;
    while(sleep(3))
    {
        bankToRun->TakeCommissions();
    }
    pthread_exit(NULL);
    return NULL;
}

void *Bank::RunBankStatus(void *bankToRunAsVoid)
{
    Bank *bankToRun = (Bank *) bankToRunAsVoid;
    while(usleep(500000))
    {
        bankToRun->PrintStatus();
    }
    pthread_exit(NULL);
    return NULL;
}

void Bank::TakeCommissions()
{
    EnterWriter();
    int commissionInPrecentage;
    int amoutToTake;
    for (size_t currentAccount = 0; currentAccount < accounts.size(); currentAccount++)
    {
        commissionInPrecentage = ((rand() % 5) + 1)/100;
        amoutToTake = accounts[currentAccount].balance * commissionInPrecentage;
        accounts[currentAccount].balance -= amoutToTake;
        this->balance += amoutToTake;
        logManager->PrintToLog("Bank: commissions of " + to_string(commissionInPrecentage) + "% were charged, the bank gained " + to_string(amoutToTake) + "$ from account " + to_string(accounts[currentAccount].id));
    }
    ExitWriter();
}

void Bank::PrintStatus()
{
    EnterReader();
    printf("\033[2J"); //clear screen
    printf("\033[1;1H");//moves cursor to beginning of screen
    cout << "Current Bank Status" << endl;
    for (size_t currentAccount = 0; currentAccount < accounts.size(); currentAccount++)
    {
        cout << "Account " + to_string(accounts[currentAccount].id) + ": Balance – " + to_string(accounts[currentAccount].balance) + "$, Account Password – " + to_string(accounts[currentAccount].password) << endl;
    }
    ExitReader();
}