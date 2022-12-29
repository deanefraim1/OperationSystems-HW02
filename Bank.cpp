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
    while(sleep(3) == 0)
    {
        bankToRun->TakeCommissions();
    }
    pthread_exit(NULL);
    return NULL;
}

void *Bank::RunBankStatus(void *bankToRunAsVoid)
{
    Bank *bankToRun = (Bank *) bankToRunAsVoid;
    while(usleep(500000) == 0) //0.5 seconds
    {
        bankToRun->PrintStatus();
    }
    pthread_exit(NULL);
    return NULL;
}

void Bank::TakeCommissions()
{
    EnterWriter();
    float commissionInPrecentage = ((float)((rand() % 5) + 1)) / 100;
    int amoutToTake;
    for (size_t currentAccount = 0; currentAccount < accounts.size(); currentAccount++)
    {
        accounts[currentAccount].EnterWriter();
        amoutToTake = accounts[currentAccount].balance * commissionInPrecentage;
        accounts[currentAccount].balance -= amoutToTake;
        this->balance += amoutToTake;
        logManager->PrintToLog("Bank: commissions of " + to_string(commissionInPrecentage) + "% were charged, the bank gained " + to_string(amoutToTake) + "$ from account " + to_string(accounts[currentAccount].id));
        accounts[currentAccount].ExitWriter();
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
    cout << "The bank has " + to_string(this->balance) + "$" << endl;
    ExitReader();
}

int Bank::GetAccountIndexFromAccountID(int accountID)//helper function, not thread safe
{
    for (size_t currentAccount = 0; currentAccount < accounts.size(); currentAccount++)
    {
        if(accounts[currentAccount].id == accountID)
            return currentAccount;
    }
    return -1;
}

int Bank::FindIndexToInsertAccountSorted(int accountID)//helper function, not thread safe
{
    for (size_t currentAccountIndex = 0; currentAccountIndex < accounts.size(); currentAccountIndex++)
    {
        if(accounts[currentAccountIndex].id > accountID)
            return currentAccountIndex;

        else if(accounts[currentAccountIndex].id == accountID)
            return -1;
    }
    return accounts.size();
}
