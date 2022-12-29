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
    pthread_t commissionThread;
    pthread_t statusThread;

    Bank();
    ~Bank();
    static void *RunBankCommision(void *bankToRunAsVoid);
    static void *RunBankStatus(void *bankToRunAsVoid);
    void TakeCommissions();
    void PrintStatus();
    int GetAccountIndexFromAccountID(int accountID);
    int FindIndexToInsertAccount(int accountID);
};

#endif
