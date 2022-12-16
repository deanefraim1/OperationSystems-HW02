#ifndef _BANK_HPP
#define _BANK_HPP

#include <vector>
#include "Account.hpp"
#include "IThreadSafe.hpp"

using namespace std;

class Bank : public IThreadSafe
{
public:
    int balance;
    vector<Account> accounts;
    pthread_t thread;

    Bank();
    ~Bank();
    static void *RunBank(void *bankToRunAsVoid);
    void TakeCommissions();
};

#endif