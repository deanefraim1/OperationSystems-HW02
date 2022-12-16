#include "Account.hpp"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "Bank.hpp"
#include "LogManager.hpp"
#include "Helpers.hpp"

extern Bank *bank;
extern LogManager *logManager;

Account::Account(int id, int password, int initialBalance)
{
    this->id = id;
    this->balance = initialBalance;
    this->password = password;
    if(pthread_mutex_init(&mutex, NULL) != 0)
        Helpers::EndProgramWithPERROR("Bank error: pthread_mutex_init failed\n");
}

Account::~Account()
{
    pthread_mutex_destroy(&mutex);
}