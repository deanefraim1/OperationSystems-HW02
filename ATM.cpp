#include "ATM.hpp"
#include "Bank.hpp"
#include "IThreadSafe.hpp"
#include "LogManager.hpp"
#include "Operation.hpp"
#include <unistd.h>
#include <string>
#include <iostream>
#include "Helpers.hpp"


extern Bank *bank;
extern LogManager *logManager;

using namespace std;

ATM::ATM(ifstream& ATMFile, int ATMID)
{
    this->id = ATMID;
    this->currentCommandIndex = 0;
    string command;
    while(getline(ATMFile, command))
    {
        Operation currentOperation(command);
        operations.push_back(currentOperation);
        command.clear();
    }
    if(pthread_create(&ATMRunThread, NULL, ATM::RunATM, this) != 0)
        Helpers::EndProgramWithPERROR("Bank error: pthread_create failed\n");
    
}

void* ATM::RunATM(void* ATMToRunAsVoid)
{
    ATM *ATMToRun = (ATM *)ATMToRunAsVoid;

    for (size_t currentOperationIndex = 0; currentOperationIndex < ATMToRun->operations.size(); currentOperationIndex++)
    {
        ATMToRun->RunOperation(currentOperationIndex);
        usleep(100000);
    }
    pthread_exit(NULL);
    return NULL;
}

void ATM::RunOperation(int operationIndex)
{
    Operation& operation = operations[operationIndex];

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
    int accountIndexToInsertTo = bank->FindIndexToInsertAccountSorted(accountID);
    if(accountIndexToInsertTo == -1)
    {
        logManager->PrintToLog("Error " + to_string(this->id) + " : Your transaction failed - account with the same id exists");
        sleep(1);
        bank->ExitWriter();
        return;
    }
    else 
    {
        Account* accountToAdd = new Account(accountID, accountPassword, initialBalance);   
        bank->accounts.insert(bank->accounts.begin() + accountIndexToInsertTo, accountToAdd);
        logManager->PrintToLog(to_string(this->id) + ": New account id is " + to_string(accountToAdd->id) + " with password " + to_string(accountToAdd->password) + " and initial balance " + to_string(accountToAdd->balance));
        sleep(1);
        bank->ExitWriter();
        return;
    }
}

void ATM::DepositToAccount(int accountID, int accountPassword, int amountToDeposit)
{
    bank->EnterReader();
    int accountIndexToDepositeTo = bank->GetAccountIndexFromAccountID(accountID);
    if(accountIndexToDepositeTo == -1)
    {
        logManager->PrintToLog("Error " + to_string(this->id) + " : Your transaction failed - account " + to_string(accountID) + " does not exist");
        sleep(1);
        bank->ExitReader();
        return;
    }
    Account* accountToDepositTo = bank->accounts[accountIndexToDepositeTo];
    if(accountToDepositTo->password != accountPassword)
    {
        logManager->PrintToLog("Error " + to_string(this->id) + " : Your transaction failed - password for account id " + to_string(accountID) + " is incorrect");
        sleep(1);
        bank->ExitReader();
        return;
    }
    else
    {
        accountToDepositTo->EnterWriter();
        sleep(1);
        bank->ExitReader();
        accountToDepositTo->balance += amountToDeposit;
        logManager->PrintToLog(to_string(this->id) + ": Account " + to_string(accountToDepositTo->id) + " new balance is " + to_string(accountToDepositTo->balance) + " after " + to_string(amountToDeposit) + " $ was deposited");
        accountToDepositTo->ExitWriter();
        return;
    }
}

void ATM::WithdrawFromAccount(int accountID, int accountPassword, int amountToWithdraw)
{
    bank->EnterReader();
    int accountIndexToWithdrawFrom = bank->GetAccountIndexFromAccountID(accountID);
    if(accountIndexToWithdrawFrom == -1)
    {
        logManager->PrintToLog("Error " + to_string(this->id) + " : Your transaction failed - account " + to_string(accountID) + " does not exist");
        sleep(1);
        bank->ExitReader();
        return;
    }
    Account* accountToWithdrawFrom = bank->accounts[accountIndexToWithdrawFrom];
    if(accountToWithdrawFrom->password != accountPassword)
    {
        logManager->PrintToLog("Error " + to_string(this->id) + " : Your transaction failed - password for account id " + to_string(accountID) + " is incorrect");
        sleep(1);
        bank->ExitReader();
        return;
    }
    else
    {
        accountToWithdrawFrom->EnterWriter();
        sleep(1);
        bank->ExitReader();
        if(accountToWithdrawFrom->balance < amountToWithdraw)
        {
            logManager->PrintToLog("Error " + to_string(this->id) + " : Your transaction failed - account " + to_string(accountID) + " balance is lower than " + to_string(amountToWithdraw));
            accountToWithdrawFrom->ExitWriter();
            return;
        }
        else
        {
            accountToWithdrawFrom->balance -= amountToWithdraw;
            logManager->PrintToLog(to_string(this->id) + ": Account " + to_string(accountToWithdrawFrom->id) + " new balance is " + to_string(accountToWithdrawFrom->balance) + " after " + to_string(amountToWithdraw) + " $ was withdrawn");
            accountToWithdrawFrom->ExitWriter();
            return;
        }
    }
}

void ATM::TransferBetweenAccounts(int accountID, int accountPassword, int accountIDToTransferTo, int amountToTransfer)
{
    bank->EnterReader();
    int accountIndexToTransferFrom = bank->GetAccountIndexFromAccountID(accountID);
    if(accountIndexToTransferFrom == -1)
    {
        logManager->PrintToLog("Error " + to_string(this->id) + " : Your transaction failed - account " + to_string(accountID) + " does not exist");
        sleep(1);
        bank->ExitReader();
        return;
    }
    Account* accountToTransferFrom = bank->accounts[accountIndexToTransferFrom];
    if(accountToTransferFrom->password != accountPassword)
    {
        logManager->PrintToLog("Error " + to_string(this->id) + " : Your transaction failed - password for account id " + to_string(accountID) + " is incorrect");
        sleep(1);
        bank->ExitReader();
        return;
    }
    else
    {
        int accountIndexToTransferTo = bank->GetAccountIndexFromAccountID(accountIDToTransferTo);
        if(accountIndexToTransferTo == -1)
        {
            logManager->PrintToLog("Error " + to_string(this->id) + " : Your transaction failed - account " + to_string(accountIDToTransferTo) + " does not exist");
            sleep(1);
            bank->ExitReader();
            return;
        }
        else
        {
            Account* accountToTransferTo = bank->accounts[accountIndexToTransferTo];
            Helpers::EnterWritersSorted(accountToTransferFrom, accountToTransferTo); // to prevent deadlock in case of two ATMs trying to transfer between the same two accounts A->B and B->A
            sleep(1);
            bank->ExitReader();
            if(accountToTransferFrom->balance < amountToTransfer)
            {
                logManager->PrintToLog("Error " + to_string(this->id) + " : Your transaction failed - account " + to_string(accountID) + " balance is lower than " + to_string(amountToTransfer));
                accountToTransferFrom->ExitWriter();
                accountToTransferTo->ExitWriter();
                return;
            }
            else
            {
                accountToTransferFrom->balance -= amountToTransfer;
                accountToTransferTo->balance += amountToTransfer;
                logManager->PrintToLog(to_string(this->id) + ": Account " + to_string(accountToTransferFrom->id) + " new balance is " + to_string(accountToTransferFrom->balance) + " after " + to_string(amountToTransfer) + " $ was transferred to account " + to_string(accountToTransferTo->id));
                accountToTransferFrom->ExitWriter();
                accountToTransferTo->ExitWriter();
                return;
            }
        }
    }
}

void ATM::BalanceInquiry(int accountID, int accountPassword)
{
    bank->EnterReader();
    int accountIndexToInquire = bank->GetAccountIndexFromAccountID(accountID);
    if(accountIndexToInquire == -1)
    {
        logManager->PrintToLog("Error " + to_string(this->id) + " : Your transaction failed - account " + to_string(accountID) + " does not exist");
        sleep(1);
        bank->ExitReader();
        return;
    }
    Account* accountToInquire = bank->accounts[accountIndexToInquire];
    if(accountToInquire->password != accountPassword)
    {
        logManager->PrintToLog("Error " + to_string(this->id) + " : Your transaction failed - password for account id " + to_string(accountID) + " is incorrect");
        sleep(1);
        bank->ExitReader();
        return;
    }
    else
    {
        accountToInquire->EnterReader();
        sleep(1);
        bank->ExitReader();
        logManager->PrintToLog(to_string(this->id) + ": Account " + to_string(accountToInquire->id) + " balance is " + to_string(accountToInquire->balance));
        accountToInquire->ExitReader();
        return;
    }
}

void ATM::CloseAccount(int accountID, int accountPassword)
{
    bank->EnterWriter();
    int accountIndexToClose = bank->GetAccountIndexFromAccountID(accountID);
    if(accountIndexToClose == -1)
    {
        logManager->PrintToLog("Error " + to_string(this->id) + " : Your transaction failed - account " + to_string(accountID) + " does not exist");
        sleep(1);
        bank->ExitWriter();
        return;
    }
    Account* accountToClose = bank->accounts[accountIndexToClose];
    if(accountToClose->password != accountPassword)
    {
        logManager->PrintToLog("Error " + to_string(this->id) + " : Your transaction failed - password for account id " + to_string(accountID) + " is incorrect");
        sleep(1);
        bank->ExitWriter();
        return;
    }
    else
    {
        // to make sure no one is reading or writing to the account
        accountToClose->EnterWriter();
        sleep(1);
        accountToClose->ExitWriter();

        bank->accounts.erase(bank->accounts.begin() + accountIndexToClose);
        delete accountToClose;
        logManager->PrintToLog(to_string(this->id) + ": Account " + to_string(accountToClose->id) + " is closed");
        bank->ExitWriter();
        return;
    }
}
