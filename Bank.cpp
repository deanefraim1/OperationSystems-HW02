#include "Bank.hpp"
#include "LogManager.hpp"
#include <string>
#include <unistd.h>

Bank::Bank()
{
    pthread_create(&thread, NULL, Bank::RunBank, this);
}

void *Bank::RunBank(void *bankToRunAsVoid)
{
    Bank *bankToRun = (Bank *) bankToRunAsVoid;
    while(sleep(3))
    {
        bankToRun->TakeCommissions();
    }
    return NULL;
}

void Bank::TakeCommissions()
{

}