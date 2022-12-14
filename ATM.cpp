#include "ATM.hpp"
#include "Bank.hpp"
#include "LogManager.hpp"
#include "Operation.hpp"

#define MAX_ARG 5

extern Bank *bank;
extern LogManager *logManager;

using namespace std;

ATM::ATM(ifstream& ATMFile)
{
    string command;
    while(getline(ATMFile, command))
    {
        Operation currentOperation(command);
        operations.push_back(currentOperation);
        command.clear();
    }
    pthread_create(&(thread), NULL, ATM::RunATM, this);
}

void* ATM::RunATM(void* ATMToRunAsVoid)
{
    ATM *ATMToRun = (ATM*) ATMToRunAsVoid;
    
    for (size_t currentOperationIndex = 0; currentOperationIndex < ATMToRun->operations.size(); currentOperationIndex++)
        ATMToRun->RunOperation(currentOperationIndex);
    
    return NULL;
}

void ATM::RunOperation(int operationIndex)
{
    Operation operation = operations[operationIndex];

    if(operation.type == "O")
        AddAccountToBank(operation.accountID, operation.accountPassword, operation.amount);
    
    else if(operation.type == "D")
        DepositToAccount(operation.accountID, operation.accountPassword, operation.amount);
    
    else if(operation.type == "W")
        WithdrawFromAccount(operation.accountID, operation.accountPassword, operation.amount);
    
    else if(operation.type == "T")
        TransferBetweenAccounts(operation.accountID, operation.accountPassword, operation.targetAccountID, operation.amount);
    
    else if(operation.type == "B")
        BalanceInquiry(operation.accountID, operation.accountPassword);

    else if(operation.type == "Q")
        CloseAccount(operation.accountID, operation.accountPassword);
    
    else
        logManager->PrintToLog("Error " + to_string(this->id) + " : Your transaction failed - illegal command");
    
}

void ATM::AddAccountToBank(int accountID, int accountPassword, int initialBalance)
{
    for (size_t currentAccount = 0; currentAccount < bank->accounts.size(); currentAccount++)
        {
            if(bank->accounts[currentAccount].id == accountID)
            {
                logManager->PrintToLog("Error " + to_string(this->id) + " : Your transaction failed - account with the same id exists");
                return;
            }
        }

        Account accountToAdd(accountID, accountPassword, initialBalance);
        
        bank->accounts.push_back(accountToAdd);
        logManager->PrintToLog(to_string(this->id) + ": New account id is " + to_string(accountToAdd.id) + " with password " + to_string(accountToAdd.password) + " and initial balance " + to_string(accountToAdd.balance));
}