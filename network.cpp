#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

#include "subnets.hpp"
#include "functions.hpp"

using namespace std;

void Network::addSubnet(Subnet subnet) {
    subnets.push_back(subnet);
}

/*
 * Verify that the provided subnet mask is real and usable with
 * provided network range.
 */
bool Network::verifyNetMask() {
    string::iterator it1;
    string::iterator it2;

    bool maskIsOne = true;

    // scan the address and mask simultaneously
    for(it1 = range.getAddress().begin(), it2 = rangeMask.getAddress().begin();
        it1 < range.getAddress().end(); it1++, it2++) {

        // first zero detected, only zeros can follow
        if( (*it2) == '0' && maskIsOne ) maskIsOne = false;

        // skip for as long as there are no zeros
        if(maskIsOne) continue;

        // encountered 1, provided values are not valid
        if( (*it1) == '1' || (*it2) == '1' ) return false;
    }

    return true;
}

/*
 * Runs some checks to see if the provided range is sufficient.
 * Output parameters are just a hotfix to be able to print out the numbers
 * in the exception text.
 */
bool Network::calculateFreeSpace(int & maxHosts, int & reqHosts) {
    vector<Subnet>::iterator it;

    // calculate the highest possible amount of hosts
    // by negating the provided subnet mask
    maxHosts = binToDec( bitNot(rangeMask.getAddress()) );

    // make sum of required clients of all subnets in the list
    for(it = subnets.begin(); it < subnets.end(); it++) {
        reqHosts += (*it).clients;
    }

    // cannot continue if there is not enough space
    if(maxHosts >= reqHosts) return true;
    return false;
}

/*
 * The main subnetting calculator. Takes care of the computation of all
 * required values based on the data from input.
 */
void Network::calculate() {
    vector<Subnet>::iterator it;

    sort(subnets.begin(), subnets.end());

    // calculateFreeSpace() passes results into these (hotfix)
    int maxHosts = 1;
    int reqHosts = 0;

    // tools to form an exception output
    stringstream ss;
    string explanation;

    if(!calculateFreeSpace(maxHosts, reqHosts)) {
        // save the amount of required and max. available hosts
        // into the string to pass into the exception
        explanation = "Required ";
        ss << reqHosts;
        explanation += ss.str();

        explanation += ", available ";
        ss.str("");
        ss << maxHosts;
        explanation += ss.str();

        throw SubnetException("The range cannot support this many hosts", explanation);
    }

    // holds value of the current subnet address
    string netPortion = range.getAddress();

    // used to calculate broadcast
    string previousNet;
    string boundary, bc;
    int temp;

    // do the counting
    for(it = subnets.begin(); it < subnets.end(); it++) {
        // get the final host count
        (*it).clients = getNextBoundary( (*it).requiredClients );

        // calculate netmask
        (*it).netMask.set( numberToMask( (*it).clients ) );

        // calculate network address
        boundary = clientsToIP( (*it).clients );
        (*it).netIP.set(netPortion);

        // save the current net. value to calculate broadcast
        previousNet = netPortion;
        // merge to obtain following subnet address
        // practically bit or on provided values
        netPortion = mergeAddresses(previousNet, boundary);

        // calculate broadcast address
        // -1 to get the last address still withing the current subnet
        temp = binToDec(boundary) - 1;
        bc = clientsToIP( temp );
        bc = mergeAddresses(previousNet, bc);
        (*it).broadcast.set(bc);
    }
}

/*
 * Save the IP values and run a check to verify that they are real.
 */
void Network::setRange(IPv4 net, IPv4 mask) {
    this->range = net;
    this->rangeMask = mask;

    if(!verifyNetMask())
        throw SubnetException("Not a valid subnet mask");
}

/*
 * Provides formatted output of all the data in the object.
 */
ostream & operator << (ostream & os, Network & network) {
    #ifdef SYSTEM_CALLS
    system("clear");
    #endif

    // default range and its mask
    printLine(os);
    os << "Range:\t\t" << network.range << endl;
    os << "Range mask:\t" << network.rangeMask << endl;
    printLine(os);

    // subnet header
    os << "Name\t\tReq. Clients\tClients\t\tNet IP\t\tNet Mask\tBC Addr" << endl;
    os << endl;

    vector<Subnet>::iterator it;

    // iterates through all the subnets
    // dynamically adjusts indentation based on the values' length
    for(it = network.subnets.begin(); it < network.subnets.end(); it++) {
        // subnet name
        os << (*it).name << "\t";
        if((*it).name.length() < 8) os << "\t";

        // provided amount of clients
        os << (*it).requiredClients << "\t";
        if((*it).requiredClients < 10000000) os << "\t";

        // client boundary
        os << (*it).clients << "\t";
        if((*it).clients < 10000000) os << "\t";

        // calculated addresses
        os << (*it).netIP << "\t";
        os << (*it).netMask << "\t" << (*it).broadcast;

        os << endl;
    }

    printLine(os);

    return os;
}

ostream & operator << (ostream & os, const SubnetException & e) {
    os << e.cause;

    return os;
}
