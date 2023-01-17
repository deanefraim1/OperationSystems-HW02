#include "Bank.hpp"
#include "LogManager.hpp"
#include "ATM.hpp"
#include <string>
#include <unistd.h>
#include <iostream>
#include "Helpers.hpp"
#include <pthread.h>

extern LogManager *logManager;

using namespace std;

Bank::Bank()
{
    balance = 0;

    if(pthread_create(&commissionThread, NULL, Bank::RunBankCommision, this) != 0)
        Helpers::EndProgramWithPERROR("Bank error: pthread_create failed");

    if(pthread_create(&statusThread, NULL, Bank::RunBankStatus, this) != 0)
        Helpers::EndProgramWithPERROR("Bank error: pthread_create failed");
}

Bank::~Bank()
{
    for (size_t currentAccountIndex = 0; currentAccountIndex < accounts.size(); currentAccountIndex++)
        delete accounts[currentAccountIndex];
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
    do//0.5 seconds
    {
        bankToRun->PrintStatus();
    } while (usleep(500000) == 0);
    pthread_exit(NULL);
    return NULL;
}

void Bank::TakeCommissions()
{
    EnterReader();
    int commisionInPercents = (rand() % 5) + 1;
    float commission = ((float)(commisionInPercents)) / 100;
    int amoutToTake;
    for (size_t currentAccount = 0; currentAccount < accounts.size(); currentAccount++)
    {
        accounts[currentAccount]->EnterWriter();
        amoutToTake = accounts[currentAccount]->balance * commission;
        accounts[currentAccount]->balance -= amoutToTake;
        this->balance += amoutToTake;
        logManager->PrintToLog("Bank: commissions of " + to_string(commisionInPercents) + "% were charged, the bank gained " + to_string(amoutToTake) + "$ from account " + to_string(accounts[currentAccount]->id));
        accounts[currentAccount]->ExitWriter();
    }
    ExitReader();
}

void Bank::PrintStatus()
{
    EnterReader();
    Helpers::EnterReaderAllAccounts();
    printf("\033[2J"); //clear screen
    printf("\033[1;1H");//moves cursor to beginning of screen
    fflush(stdout);
    cout << "Current Bank Status" << endl;
    for (size_t currentAccount = 0; currentAccount < accounts.size(); currentAccount++)
    {
        cout << "Account " + to_string(accounts[currentAccount]->id) + ": Balance - " + to_string(accounts[currentAccount]->balance) + "$, Account Password - " + to_string(accounts[currentAccount]->password) << endl;
    }
    cout << "The bank has " + to_string(this->balance) + "$" << endl;
    Helpers::ExitReaderAllAccounts();
    ExitReader();
}

int Bank::GetAccountIndexFromAccountID(int accountID)//helper function, not thread safe
{
    for (size_t currentAccount = 0; currentAccount < accounts.size(); currentAccount++)
    {
        if(accounts[currentAccount]->id == accountID)
            return currentAccount;
    }
    return -1;
}

int Bank::FindIndexToInsertAccountSorted(int accountID)//helper function, not thread safe
{
    for (size_t currentAccountIndex = 0; currentAccountIndex < accounts.size(); currentAccountIndex++)
    {
        if(accounts[currentAccountIndex]->id > accountID)
            return currentAccountIndex;

        else if(accounts[currentAccountIndex]->id == accountID)
            return -1;
    }
    return accounts.size();
}
