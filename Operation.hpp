#ifndef _OPERATION_HPP
#define _OPERATION_HPP

#include <string>

using namespace std;

class Operation
{
public:
    string type;
    int accountID;
    int accountPassword;
    int amount;
    int targetAccountID;

    Operation(string command);
};

#endif
