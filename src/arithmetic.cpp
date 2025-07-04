#include "arithmetic.h"
#include "globals.h"
#include "simpleGates.h"
#include <iostream>

// Quantum two's complement circuit
// LSB first
void QTC(std::vector<std::string> registerq){
    for(int i = 0; i < registerq.size(); i++){
        x(registerq[i]);
    }
    // add 1
    addQFTval(registerq, 1);
}

// Quantum two's complement circuit with single qubit control
// LSB first
void cQTC(std::vector<std::string> registerq, std::string control){
    for(int i = 0; i < registerq.size(); i++){
        cx(control, registerq[i]);
    }
    // add 1
    caddQFTval(registerq, control, 1);
}

// Converts a classical integer value (C++) to a binary array
// Unsigned integer, LSB first
std::vector<int> intToBinArray(int value, int depth){
    
}

// Converts a classical integer value (C++) to a binary array
// Signed integer, LSB first
std::vector<int> intToBinArray_s(int value, int depth){
    
}