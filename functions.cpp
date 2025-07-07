#include <iostream>
#include <cstdlib>
#include <fstream>
#include <algorithm>

#include "functions.hpp"

using namespace std;

void printHelp() {
    cout << "Usage:";
    cout << "\n\tAutomatic startup:\tsubnets [file]";
    cout << "\n\tInteractive startup:\tsubnets";
    cout << "\n\tRequest help:\t\tsubnets --help";
    cout << endl;
}

/*
 * Converts provided value into a 32 bit binary representation for
 * further calculations.
 */
string numberToMask(int value) {
    string result, temp;

    temp = decToBin(value);

    // add leading ones to match the length as well as the netmask format
    if(temp.length() < 32) {
        for(int i = 0; i < (32 - (int)temp.length()); i++) {
            result += "1";
        }
    }

    result += temp;

    return result;
}

/*
 * Converts provided value into a 32 bit binary representation for
 * further calculations.
 */
string clientsToIP(int value) {
    string result, temp;

    temp = decToBin(value);

    // add leading zeros to match the 32 bit IPv4 length
    if(temp.length() < 32) {
        for(int i = 0; i < (32 - (int)temp.length()); i++) {
            result += "0";
        }
    }

    result += temp;

    return result;
}

/*
 * Converts provided decimal value into its binary string representation.
 */
string decToBin(int number) {
    if ( number == 0 ) return "0";
    if ( number == 1 ) return "1";

    if ( number % 2 == 0 )
        return decToBin(number / 2) + "0";
    else
        return decToBin(number / 2) + "1";
}

/*
 * Converts provided string of binary values into a decimal int.
 */
int binToDec(string number) {
    int result = 0, pow = 1;
    for ( int i = number.length() - 1; i >= 0; --i, pow <<= 1 )
        result += (number[i] - '0') * pow;

    return result;
}

/*
 * Simulation of a bit and on a string consisting of binary values.
 */
string bitAnd(string str1, string str2) {
    string::iterator it1;
    string::iterator it2;

    string result;

    for(it1 = str1.begin(), it2 = str2.begin(); it1 < str1.end(); it1++, it2++) {
        if( (*it1) == '1' && (*it2) == '1' ) {
            result += '1';
        } else {
            result += '0';
        }
    }

    return result;
}

/*
 * Simulation of a numerical addition for two binary strings.
 */
string mergeAddresses(string str1, string str2) {
    string::iterator it1;
    string::iterator it2;

    string temp, result;
    bool carry = false;

    // only 1 ending constraint since both strings have equal length
    for(it1 = str1.end() - 1, it2 = str2.end() - 1; it1 >= str1.begin(); it1--, it2--) {
        if(!carry) {
            if( (*it1) == '1' && (*it2) == '1' ) {
                // add value
                result += "0";
                carry = true;
            // no carry, but still 1
            } else if( (*it1) == '1' || (*it2) == '1' ) {
                result += "1";
            } else {
                result += "0";
            }
        } else {
            // handle carry flag:
            if( (*it1) == '1' && (*it2) == '1' ) {
                // add value
                result += "1";
            // no carry, but still 1
            } else if( (*it1) == '1' || (*it2) == '1' ) {
                result += "0";
            } else {
                result += "1";
                carry = false;
            }
        }

    }

    // swap the string - is reversed now
    reverse(result.begin(), result.end());

    return result;
}

/*
 * Simulation of a bit negation on a string consisting of binary values.
 */
string bitNot(string str) {
    string::iterator it;

    string result;

    for(it = str.begin(); it < str.end(); it++) {
        if( (*it) == '1' ) {
            result += '0';
        } else {
            result += '1';
        }
    }

    return result;
}

/*
 * Prints a line to enhance output. Used in the final printout.
 */
ostream & printLine(ostream & os) {
    os << "------------------------------------------------";
    os << "-----------------------------------------------";
    os << endl;

    return os;
}

/*
 * Calculates the closest higher 2^n value for the provided number.
 */
int getNextBoundary(int value) {
    int boundary = 2;

    // + 2 == net + bc addr
    while(boundary <= (value + 2)) {
        boundary *= 2;
    }

    return boundary;
}

/*
 * Reads an IPv4 address in the decimal octet format and assigns
 * the final value to the respective variable
 */
bool setAddr(string & line, IPv4 & addr) {
    string::iterator it;

    int octet = 1;
    string val;
    int a, b, c, d;
    bool empty = true;

    // iterate through the provided line
    for(it = line.begin(); it < line.end(); it++) {
        // ignore spaces
        if( (*it) == ' ' || (*it) == '\t' ) {
            continue;
        }

        // not a black => there are data on the line
        empty = false;

        // '.' means the end of an octet
        if(*it == '.' ) {
            switch(octet) {
                case 1:
                    a = atoi(val.c_str());
                    break;
                case 2:
                    b = atoi(val.c_str());
                    break;
                case 3:
                    c = atoi(val.c_str());
                    break;
            }

            octet++;
            val.clear();

        // value is not a dot
        } else {
            // not even a digit == failure
            if(*it < 48 || *it > 57) {
                throw SubnetException("Not a valid IPv4 address", line);

            // in a case of digit, save the value to be processed later
            } else {
                val += *it;
            }
        }
    }

    // do not continue processing if the line is empty
    if(empty) return false;

    // add the last octet separately, since there are only three dots
    // and the condition inside the loop is never met
    d = atoi(val.c_str());

    if(octet != 4) {
        throw SubnetException("Provided address did not contain exactly 4 octets", line);
    }

    // run a verification to make sure the values are real
    if(a > 255 || b > 255 || c > 255 || d > 255) {
        throw SubnetException("Values not in range of 0-255", line);
    }

    // save the final IPv4 value
    addr.set(a, b, c, d);

    return true;
}

/*
 * Accepts data until the end of input occurs.
 * Data in this case represent subnet names and required number of hosts.
 */
void parseSubnets(istream & iStr, Network & network) {
    string line;

    string::iterator it;
    string name, clients;
    bool nameDone;
    char ch;

    // read a line with subnet information, try to process
    while( getline(iStr, line) ) {
        name.clear();
        clients.clear();
        nameDone = false;

        // space indicates the end of a subnet name and the beginning
        // of the host number
        for(it = line.begin(); it < line.end(); it++) {
            // ignore leading spaces
            if( ((*it) == ' ' || (*it) == ' ') && name.length() == 0 ) continue;

            // subnet name ended, client number follows, skip spaces
            // spaces can even occur inside the number
            if( (*it) == ' ' || (*it) == '\t' ) {
                nameDone = true;
                continue;
            }

            ch = *it;

            if(!nameDone) {
                name += ch;
            } else {
                // host amount can only consist of digits
                if(ch < 48 || ch > 57) {
                    throw SubnetException("Expected integer as the second value", line);
                }

                clients += ch;
            }
        }

        // add the processes subnet to the list
        // ignore empty lines
        if(name.length() > 0 && clients.length() > 0) {
            network.addSubnet( Subnet(name, atoi(clients.c_str())) );
        }
    }
}

/*
 * Function attempts to read required initial data from a text file
 * that has been provided on a command line.
 */
void handleFromCLI(Network & network, char ** argv) {
    string line;
    IPv4 addr, mask;

    ifstream dataFile;

    bool isAddressOK = false;

    // try to open file that has been specified on the command line
    dataFile.open(argv[1]);

    if (dataFile.is_open()) {
        // read the default network range
        while(!isAddressOK) {
            getline(dataFile, line);
            isAddressOK = setAddr(line, addr);
        }

        // reset the value to read the netmask
        isAddressOK = false;

        // read the default mask
        while(!isAddressOK) {
            getline(dataFile, line);
            isAddressOK = setAddr(line, mask);
        }

        network.setRange(addr, mask);

        // read subnets
        parseSubnets(dataFile, network);

    } else {
        throw SubnetException("Could not open the file");
    }

    dataFile.close();
}

/*
 * Function provides an interactive interface to help the user with
 * providing the initial data.
 */
void handleFromPrompt(Network & network) {
    string line;
    IPv4 addr, mask;

    // the default network range
    cout << "Enter total range (IPv4): ";
    getline(cin, line);
    setAddr(line, addr);

    // default mask
    cout << "Enter range netmask (IPv4): ";
    getline(cin, line);
    setAddr(line, mask);

    network.setRange(addr, mask);

    // loop for adding subnets
    cout << endl;
    cout << "Start entering subnets [subnet_name required_clients]\n";
    cout << "or interrupt by pressing <CTRL>+D / <CTRL>+Z" << endl;
    parseSubnets(cin, network);
}
