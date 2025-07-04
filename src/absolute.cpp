#include "absolute.h"
#include "globals.h"
#include "simpleGates.h"
#include "arithmetic.h"
#include "toffoli.h"
#include <iostream>

// Quantum absolute value
// Converts input quantum register into absolute value. Original sign qubit is transferred to sign qubit.
// Signed integers
// LSB first. Sign qubit is MSB.
// Source: https://arxiv.org/pdf/2005.00443
void QABS(std::vector<std::string> registerq, std::string signQubit, std::string ancilla){
    std::vector<std::string> regq = parseQregisterVector(registerq);
    int n = regq.size();

    cx(regq[n-1], signQubit);
    std::vector<std::string> subRegq(regq.begin(), regq.end() - 1);
    cQTC(subRegq, regq[n-1], ancilla);
    cx(signQubit, regq[n-1]);
}

// Quantum absolute value with single qubit control
// Converts input quantum register into absolute value. Original sign qubit is transferred to sign qubit.
// Signed integers
// LSB first. Sign qubit is MSB.
void cQABS(std::vector<std::string> registerq, std::string signQubit, std::string ancilla, std::string control){
    std::vector<std::string> regq = parseQregisterVector(registerq);
    int n = regq.size();

    std::vector<std::string> controls = {regq[n-1], control};
    toffoli(controls, signQubit);
    std::vector<std::string> subRegq(regq.begin(), regq.end() - 1);
    cQTC(subRegq, signQubit, ancilla);
    cx(signQubit, regq[n-1]);
}