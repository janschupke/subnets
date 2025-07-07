#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>

#include "functions.hpp"
#include "subnets.hpp"

// if compiling for windows
#define WINDOWS_COMPILATION

using namespace std;

void bye() {
    #ifdef WINDOWS_COMPILATION
        cout << "\nPress enter to exit..." << endl;
        cin.clear();
        cin.get();
    #endif
}

int main(int argc, char ** argv) {
    // all data will be stored in this object
    Network network;

    try {
        // help text requested
        // strcmp() returns 0 when successfull => need to negate
        if(argc == 2 && (
            !strcmp(argv[1],"-h") ||
            !strcmp(argv[1], "--help") ||
            !strcmp(argv[1], "--usage") )) {

            printHelp();
            return EXIT_SUCCESS;

        // if exactly one argument is provided,
        // try to load data from file
        } else if(argc == 2) {
            handleFromCLI(network, argv);

        // this amount of arguments is not allowed, print help text
        } else if(argc > 2) {
            printHelp();
            return EXIT_FAILURE;

        // otherwise provide interactive communication
        } else {
            handleFromPrompt(network);
        }

        // data loaded, calculate all values
        network.calculate();

    } catch(SubnetException e) {
        cerr << e << endl;
        bye();

        return EXIT_FAILURE;
    }

    // display output table
    cout << network << endl;
    bye();

    return EXIT_SUCCESS;
}
