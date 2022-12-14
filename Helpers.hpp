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
    static vector<ATM> InitializeATMsVector(int argc, char* argv[]);
    static void JoinAllThreads(vector<ATM> ATMs);
};

#endif