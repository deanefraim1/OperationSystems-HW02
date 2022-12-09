#ifndef _OPERATION_HPP
#define _OPERATION_HPP

#include <string>

using namespace std;

class Operation
{
public:
    string operationType;
    int accountID;
    int password;
    int amount;
    int targetAccountID;

    Operation(string command);
};

#endif