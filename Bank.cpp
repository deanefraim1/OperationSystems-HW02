#include "Bank.hpp"
#include "LogManager.hpp"
#include "ATM.hpp"
#include <string>
#include <unistd.h>

extern int numberOfATMsRunning;

Bank::Bank()
{
    pthread_create(&thread, NULL, Bank::RunBank, this);
}

Bank::~Bank()
{
    pthread_exit(NULL);
}

void *Bank::RunBank(void *bankToRunAsVoid)
{
    Bank *bankToRun = (Bank *) bankToRunAsVoid;
    while(numberOfATMsRunning > 0)
    {
        bankToRun->TakeCommissions();
        sleep(3);
    }
    return NULL;
}

void Bank::TakeCommissions()
{
    int commissionInPrecentage;
    int amoutToTake;
    for (size_t currentAccount = 0; currentAccount < accounts.size(); currentAccount++)
    {
        commissionInPrecentage = ((rand() % 5) + 1)/100;
        amoutToTake = accounts[currentAccount].balance * commissionInPrecentage;
        accounts[currentAccount].balance -= amoutToTake;
        this->balance += amoutToTake;
    }
}