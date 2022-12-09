#include "Account.hpp"
#include <pthread.h>


Account::Account(int initialBalance, int id, int password)
{
    this->id = id;
    this->balance = initialBalance;
    this->password = password;
    pthread_mutex_init(&(this->mutex), NULL);
}