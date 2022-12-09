#include "Bank.hpp"

Bank::Bank()
{
    pthread_create(&thread, NULL, (THREADFUNCPTR) &Bank::RunBank, this);
}

void *Bank::RunBank()
{
    
}