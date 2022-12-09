#ifndef _ATM_HPP
#define _ATM_HPP

#include <fstream>
#include <vector>
#include <string>
#include "Operation.hpp"
#include "Bank.hpp"
#include "Account.hpp"

using namespace std;

typedef void * (*THREADFUNCPTR)(void *);
class ATM
{
public:

    vector<Operation> operations;
    int currentCommandIndex;
    pthread_t thread;

    ATM(ifstream ATMFile);
    void *RunATM();
    void RunOperation(int operationIndex);
};

#endif