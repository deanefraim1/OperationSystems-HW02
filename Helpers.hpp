#ifndef _HELPERS_HPP
#define _HELPERS_HPP

#include <iostream>
#include <string>
#include <vector>
#include "ATM.hpp"

using namespace std;

class Helpers
{
public:
    static void EndProgramWithPERROR(string errorMessage);
    static void EndProgramWithSTDERR(string errorMessage);
    static vector<ATM*>* InitializeATMsVector(int argc, char* argv[]);
    static void deleteATMs();
    static void JoinAllATMsThreads(vector<ATM*>* ATMs);
    static int GetATMIDFromFileName(string fileName);
    static void ExitProgramWithPERROR(string errorMessage);
    static void EnterWritersSorted(Account* firstAccount, Account* secondAccount);
    static void EnterReaderAllAccounts();
    static void ExitReaderAllAccounts();
};

#endif
