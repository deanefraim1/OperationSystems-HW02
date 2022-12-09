#include "ATM.hpp"

#define MAX_ARG 5

extern Bank *bank;

using namespace std;
ATM::ATM(ifstream ATMFile)
{
    string command;
    while(getline(ATMFile, command))
    {
        Operation currentOperation(command);
        operations.push_back(currentOperation);
        command.clear();
    }
    pthread_create(&(thread), NULL, (THREADFUNCPTR) &ATM::RunATM, this);
}

void* ATM::RunATM()
{
    for (size_t currentOperationIndex = 0; currentOperationIndex < operations.size(); currentOperationIndex++)
    {
        RunOperation(currentOperationIndex);
    }
}

void ATM::RunOperation(int operationIndex)
{
    if(operations[operationIndex].operationType == "O")
    {
        for (size_t currentAccount = 0; currentAccount < bank->accounts.size(); currentAccount++)
        {
            if(bank->accounts[currentAccount].id == operations[operationIndex].accountID)
            {
                
            }
        }
        
        bank->accounts.push_back(Account(operations[operationIndex].amount,
                                         operations[operationIndex].accountID,
                                         operations[operationIndex].password));
    }
}