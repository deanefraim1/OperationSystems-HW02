#ifndef _ACCOUNT_HPP
#define _ACCOUNT_HPP

#include <pthread.h>
#include "IThreadSafe.hpp"

class Account : public IThreadSafe
{
public:

    int balance;
    int id;
    int password;  

    Account(int id, int password, int initialBalance);
};

#endif
