#include "ATM.hpp"
#include "Bank.hpp"
#include "LogManager.hpp"

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
    {
        ATMToRun->RunOperation(currentOperationIndex);
    }
    return NULL;
}

void ATM::RunOperation(int operationIndex)
{
    if(operations[operationIndex].operationType == "O")
    {
        Account accountToAdd = Account(operations[operationIndex].amount,
                                       operations[operationIndex].accountID,
                                       operations[operationIndex].accountPassword);
        AddAccountToBank(accountToAdd);
    }
    else if(operations[operationIndex].operationType == "D")
    {
        
    }

}

void ATM::AddAccountToBank(Account accountToAdd)
{
    for (size_t currentAccount = 0; currentAccount < bank->accounts.size(); currentAccount++)
        {
            if(bank->accounts[currentAccount].id == accountToAdd.id)
            {
                logManager->PrintToLog("Error " + to_string(this->id) + " : Your transaction failed - account with the same id exists");
                return;
            }
        }
        
        bank->accounts.push_back(accountToAdd);
        logManager->PrintToLog(to_string(this->id) + ": New account id is " + to_string(accountToAdd.id) + " with password " + to_string(accountToAdd.password) + " and initial balance " + to_string(accountToAdd.balance));
}