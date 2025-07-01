#include "grover.h"
#include "globals.h"
#include "simpleGates.h"
#include "toffoli.h"
#include "math.h"
#include <iostream>

// Grover's algorithm

void grover(std::vector<std::string> workRegisters, std::vector<std::string> readoutRegisters,  std::vector<std::string> ancillaRegisters, std::function<void(std::vector<std::string>, std::vector<std::string>)> oracle){
    std::vector<std::string> workQubits = parseQregisterVector(workRegisters);
    std::vector<std::string> readoutBits = parseQregisterVector(readoutRegisters);
    int numWorkQubits = workQubits.size() - 1;
    std::string bellQubit = workQubits[workQubits.size()-1];
    std::vector<std::string> ancillaQubits = parseQregisterVector(ancillaRegisters);

    // State preparation
    for(int i = 0; i < numWorkQubits; i++){
        h(workQubits[i]);
    }
    x(bellQubit);
    h(bellQubit);

    int numIterations = (int)(pi/4)*std::sqrt(numWorkQubits);
    std::vector<std::string>::const_iterator first = workQubits.begin();
    std::vector<std::string>::const_iterator last = workQubits.begin() + numWorkQubits;
    std::vector<std::string> workQubitsSubVector(first, last);

    // Grover operator repeated applications
    for(int i = 0; i < numIterations; i++){
        oracle(workQubits, ancillaQubits);
        diffusion(workQubitsSubVector, ancillaQubits);
    }

    // Measurement
    for(int i = 0; i < numWorkQubits; i++){
        measureRegister(workQubits[i], readoutBits[i]);
    }
}

void diffusion(std::vector<std::string> workQubits, std::vector<std::string> ancillaQubits){
    for(int i = 0; i < workQubits.size() - 1; i++){
        h(workQubits[i]);
        x(workQubits[i]);
    }
    z(workQubits[workQubits.size() - 1]);

    // Toffoli
    std::vector<std::string>::const_iterator first = workQubits.begin();
    std::vector<std::string>::const_iterator last = workQubits.begin() + workQubits.size() - 1;
    std::vector<std::string> workQubitsSubVector(first, last);

    toffoli_n(workQubitsSubVector, ancillaQubits, workQubits[workQubits.size() - 1]);

    z(workQubits[workQubits.size() - 1]);

    for(int i = 0; i < workQubits.size() - 1; i++){
        x(workQubits[i]);
        h(workQubits[i]);
    }

}

