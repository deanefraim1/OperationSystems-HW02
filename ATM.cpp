#include "ATM.hpp"
#include "Bank.hpp"
#include "IThreadSafe.hpp"
#include "LogManager.hpp"
#include "Operation.hpp"
#include <unistd.h>
#include <string>
#include <iostream>


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
    pthread_create(&(ATMRunThread), NULL, ATM::RunATM, this);
}

ATM::~ATM()
{
    pthread_exit(NULL);
}

void* ATM::RunATM(void* ATMToRunAsVoid)
{
    ATM *ATMToRun = (ATM *)ATMToRunAsVoid;

    for (size_t currentOperationIndex = 0; currentOperationIndex < ATMToRun->operations.size(); currentOperationIndex++)
    {
        ATMToRun->RunOperation(currentOperationIndex);
        usleep(100000);
    }

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
    bank->EnterWriter();
    for (size_t currentAccountIndex = 0; currentAccountIndex < bank->accounts.size(); currentAccountIndex++)
        {
            if(bank->accounts[currentAccountIndex].id == accountID)
            {
                logManager->PrintToLog("Error " + to_string(this->id) + " : Your transaction failed - account with the same id exists");
                bank->ExitWriter();
                return;
            }
        }

    Account accountToAdd(accountID, accountPassword, initialBalance);   
    bank->accounts.push_back(accountToAdd);
    logManager->PrintToLog(to_string(this->id) + ": New account id is " + to_string(accountToAdd.id) + " with password " + to_string(accountToAdd.password) + " and initial balance " + to_string(accountToAdd.balance));
    bank->ExitWriter();
}

void ATM::DepositToAccount(int accountID, int accountPassword, int amountToDeposit)
{
    bank->EnterReader();
    for (size_t currentAccountIndex = 0; currentAccountIndex < bank->accounts.size(); currentAccountIndex++)
    {
        if(bank->accounts[currentAccountIndex].id == accountID)
        {
            if(bank->accounts[currentAccountIndex].password == accountPassword)
            {
                Account& accountToDepositTo = bank->accounts[currentAccountIndex];
                bank->ExitReader();
                accountToDepositTo.EnterWriter();
                accountToDepositTo.balance += amountToDeposit;
                logManager->PrintToLog(to_string(this->id) + ": Account " + to_string(accountID) + " new balance is " + to_string(bank->accounts[currentAccountIndex].balance) + " after " + to_string(amountToDeposit) + " $ was deposited");
                accountToDepositTo.ExitWriter();
                return;
            }
            else
            {
                logManager->PrintToLog("Error " + to_string(this->id) + " : Your transaction failed - password for account id " + to_string(accountID) + " is incorrect");
                bank->ExitReader();
                return;
            }
        }
    }
    logManager->PrintToLog("Error " + to_string(this->id) + " : Your transaction failed - account " + to_string(accountID) + " does not exist");
    bank->ExitReader();
}

void ATM::WithdrawFromAccount(int accountID, int accountPassword, int amountToWithdraw)
{
    bank->EnterReader();
    for (size_t currentAccountIndex = 0; currentAccountIndex < bank->accounts.size(); currentAccountIndex++)
    {
        if(bank->accounts[currentAccountIndex].id == accountID)
        {
            if(bank->accounts[currentAccountIndex].password == accountPassword)
            {
                Account& accountToWithdrawFrom = bank->accounts[currentAccountIndex];
                bank->ExitReader();
                accountToWithdrawFrom.EnterWriter();
                if(accountToWithdrawFrom.balance >= amountToWithdraw)
                {
                    accountToWithdrawFrom.balance -= amountToWithdraw;
                    logManager->PrintToLog(to_string(this->id) + ": Account " + to_string(accountID) + " new balance is " + to_string(bank->accounts[currentAccountIndex].balance) + " after " + to_string(amountToWithdraw) + " $ was withdrew");
                    accountToWithdrawFrom.ExitWriter();
                    return;
                }
                else
                {
                    logManager->PrintToLog("Error " + to_string(this->id) + " : Your transaction failed - account " + to_string(accountID) + " balance is lower than " + to_string(amountToWithdraw));
                    accountToWithdrawFrom.ExitWriter();
                    return;
                }
            }
            else
            {
                logManager->PrintToLog("Error " + to_string(this->id) + " : Your transaction failed - password for account id " + to_string(accountID) + " is incorrect");
                bank->ExitReader();
                return;
            }
        }
    }
    logManager->PrintToLog("Error " + to_string(this->id) + " : Your transaction failed - account " + to_string(accountID) + " does not exist");
    bank->ExitReader();
}

void ATM::TransferBetweenAccounts(int accountID, int accountPassword, int accountIDToTransferTo, int amountToTransfer)
{
    
}

void ATM::BalanceInquiry(int accountID, int accountPassword)
{

}

void ATM::CloseAccount(int accountID, int accountPassword)
{

}
