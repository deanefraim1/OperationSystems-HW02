#ifndef _ATM_HPP
#define _ATM_HPP

#include <fstream>
#include <vector>
#include <string>
#include "Operation.hpp"
#include "Bank.hpp"
#include "Account.hpp"

using namespace std;

class ATM
{
public:

    vector<Operation> operations;
    int currentCommandIndex;
    pthread_t ATMRunThread;
    int id;

    ATM(ifstream& ATMFile);
    ~ATM();
    static void *RunATM(void *ATMToRunAsVoid);
    void RunOperation(int operationIndex);
    void AddAccountToBank(int accountID, int accountPassword, int initialBalance);
    void DepositToAccount(int accountID, int accountPassword, int amountToDeposit);
    void WithdrawFromAccount(int accountID, int accountPassword, int amountToWithdraw);
    void TransferBetweenAccounts(int accountID, int accountPassword, int accountIDToTransferTo, int amountToTransfer);
    void BalanceInquiry(int accountID, int accountPassword);
    void CloseAccount(int accountID, int accountPassword);

};

#endif