#ifndef SUBNETS_HPP
    #define SUBNETS_HPP

#include <iostream>
#include <vector>

using namespace std;

class SubnetException {
    string cause;

    public:
        // basic initialization with just the error text
        SubnetException(string cause) : cause(cause) {}

        // append contents of the incorrect line
        SubnetException(string cause, string line) {
            this->cause = cause;
            this->cause += ":\n>>> ";
            this->cause += line;
            this->cause += "\n";
        }

        friend ostream & operator << (ostream & os, const SubnetException & e);
};

class IPv4 {
        string address;
        int octet[4];

    public:
        IPv4() {}

        // initialize the address by providing decimal octet values
        // calls set() function that converts octets into string
        IPv4(int a, int b, int c, int d) { set(a, b, c, d); }

        // initialize the address by providing binary representation
        IPv4(string addr) : address(addr) { updateOctets(); }

        // populate octets and then calculate binary value for the address
        void set(int a, int b, int c, int d) {
            octet[0] = a;
            octet[1] = b;
            octet[2] = c;
            octet[3] = d;

            parseOctets();
        }

        // defined in struct.cpp
        void parseOctets();
        void updateOctets();

        // set new binary value and update octet values accordingly
        void set(string addr) {
            address = addr;
            updateOctets();
        }

        int getOctet(int val) const { return octet[val]; }
        string getAddress() const { return address; }

        friend ostream & operator << (ostream & os, IPv4 & ip);
};

typedef struct Subnet {
    string name;
    int requiredClients;
    int clients;
    IPv4 netIP;
    IPv4 netMask;
    IPv4 broadcast;

    Subnet(string name, int rc) : name(name), requiredClients(rc) {
        clients = rc;
        netIP.set(0, 0, 0, 0);
        netMask.set(0, 0, 0, 0);
        broadcast.set(0, 0, 0, 0);
    }

    bool operator < (const Subnet & subnet) const {
        return (requiredClients > subnet.requiredClients);
    }

} Subnet;

class Network {
    IPv4 range;
    IPv4 rangeMask;
    vector<Subnet> subnets;

    public:
        void addSubnet(Subnet subnet);
        bool calculateFreeSpace(int & maxHosts, int & reqHosts);
        bool verifyNetMask();
        void calculate();
        void setRange(IPv4 net, IPv4 mask);

        friend ostream & operator << (ostream & os, Network & network);
};

#endif // SUBNETS_HPP
