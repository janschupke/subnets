#ifndef FUNCTIONS_HPP
    #define FUNCTIONS_HPP

#include "subnets.hpp"

using namespace std;

void printHelp();

// these take care of input
void handleFromPrompt(Network & network);
void handleFromCLI(Network & network, char ** argv);

// calculates the closest higher 2^n power for the given number
int getNextBoundary(int value);

// prints line to provide nicer output
ostream & printLine(ostream & os);

// value convertors
string decToBin(int number);
int binToDec(string number);

// bit operation simulators
string mergeAddresses(string str1, string str2);
string bitAnd(string str1, string str2);
string bitNot(string str);

// convertors from number to binary IP
string clientsToIP(int value);
string numberToMask(int value);

#endif // FUNCTIONS_HPP
