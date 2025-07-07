#include <iostream>

#include "subnets.hpp"
#include "functions.hpp"

using namespace std;

/*
 * Function takes member octets and parses their values into one
 * binary member string to represent the full IPv4 address.
 */
void IPv4::parseOctets() {
    string bin, temp;

    address.clear();

    // there are four octets
    for(int i = 0; i < 4; i++) {
        bin = decToBin(octet[i]);

        // add leading zeros until the string is 8 characters long
        if(bin.length() < 8) {
            temp = bin;
            bin.clear();

            for(int j = 0; j < (8 - (int)temp.length()); j++) {
                bin += "0";
            }

            bin += temp;
        }

        address += bin;
    }
}

/*
 * Function updates octet values to match current string representation.
 */
void IPv4::updateOctets() {
    string::iterator it;
    it = address.begin();

    string temp;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 8; j++) {
            temp += (*it);
            it++;
        }

        octet[i] = binToDec(temp);
        temp.clear();
    }
}

/*
 * Prints out the address in decimal octet format.
 */
ostream & operator << (ostream & os, IPv4 & ip) {
    os << ip.octet[0] << "." << ip.octet[1] << "." << ip.octet[2] << "." << ip.octet[3];

    return os;
}
