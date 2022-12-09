#include "Bank.hpp"

Bank::Bank()
{
    pthread_create(&thread, NULL, RunBank, NULL);
}

void Bank::RunBank()
{

}