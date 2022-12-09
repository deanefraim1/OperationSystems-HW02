#include "Operation.hpp"

Operation::Operation(string command)
{
    string delimiters = " \t\n";
    string operationType = strtok(strdup(command.c_str()), strdup(delimiters.c_str()));
    int accountID = atoi(strtok(NULL, strdup(delimiters.c_str())));
    int accountPassword = atoi(strtok(NULL, strdup(delimiters.c_str())));
    int amount;
    int targetAccountID;
    if (operationType == "O")
    {
        amount = atoi(strtok(NULL, strdup(delimiters.c_str())));
    }
    else if(operationType == "D")
    {
        amount = atoi(strtok(NULL, strdup(delimiters.c_str())));
    }
    else if(operationType == "W")
    {
        amount = atoi(strtok(NULL, strdup(delimiters.c_str())));
    }
    else if(operationType == "T")
    {
        targetAccountID = atoi(strtok(NULL, strdup(delimiters.c_str())));
        amount = atoi(strtok(NULL, strdup(delimiters.c_str())));
    }
    this->operationType = operationType;
    this->accountID = accountID;
    this->password = accountPassword;
    this->amount = amount;
    this->targetAccountID = targetAccountID;
}