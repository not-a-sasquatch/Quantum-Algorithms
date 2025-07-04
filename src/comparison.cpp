#include "comparison.h"
#include "globals.h"
#include "simpleGates.h"
#include "toffoli.h"
#include "addition.h"
#include <iostream>


// Quantum comparator between signed integers
// LSB first
// lessThan is flipped if register0 < register1
// greaterThan is flipped if register0 > register1
// equality is flipped if register0 = register1
// Source: https://arxiv.org/pdf/2005.00443
void QComp(std::vector<std::string> register0, std::vector<std::string> register1, std::string lessThan, std::string greaterThan, std::string equality, std::vector<std::string> ancillas){
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> reg1 = parseQregisterVector(register1);
    std::vector<std::string> ancillaQubits = parseQregisterVector(ancillas);
    int n = reg0.size();
    if(n < reg1.size()){
        std::cout << "Beep boop! QComp error - register0 size must be >= register1 size.\n";
    } else if(n > ancillaQubits.size() + 4){
        std::cout << "Beep boop! QComp error - register0 size must be <= ancillas size + 4.\n";
    }
    // Qubits storing result of comparator
    //std::string lessThan = ancillas[0];
    //std::string greaterThan = ancillas[1];
    //std::string equality = ancillas[2];
    // Overflow/sign bits used in signed quantum subtraction/addition procedures
    //std::string ancilla0 = ancillas[3];
    //std::string ancilla1 = ancillas[4];
    std::string ancilla0 = ancillas[0];
    std::string ancilla1 = ancillas[1];
    // Need n-1 ancillas for (n+1)-qubit toffoli gate
    //std::vector<std::string> ancillasToffoli(ancillas.begin() + 5, ancillas.end());
    std::vector<std::string> ancillasToffoli(ancillas.begin() + 2, ancillas.end());
    // Subtract reg1 from reg0, output stored in reg0, ancilla0 is now MSB of new reg0
    subQFT_s(reg0, ancilla0, reg1);
    // Compute comparator outputs
    cx(ancilla0, lessThan);
    x(ancilla0);
    cx(ancilla0, greaterThan);
    x(ancilla0);
    reg0.push_back(ancilla0);
    for(int i = 0; i < n+1; i++){
        x(reg0[i]);
    }
    toffoli_n(reg0, ancillasToffoli, greaterThan);
    toffoli_n(reg0, ancillasToffoli, equality);
    for(int i = 0; i < n+1; i++){
        x(reg0[i]);
    }
    // Reverse computation
    // Add reg1 to new reg0, output stored in new reg0, ancilla1 is now MSB of new reg0
    addQFT_s(reg0, ancilla1, reg1);
    // Return sign bit of reg0 to original qubit
    swap(reg0[n-1], ancilla1);
}