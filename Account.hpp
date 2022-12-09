#ifndef _ACCOUNT_HPP
#define _ACCOUNT_HPP

#include <pthread.h>

class Account
{
public:

    int balance;
    int id;
    int password;
    pthread_mutex_t mutex;

    Account(int initialBalance, int id, int password);
};

#endif