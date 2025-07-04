#include "absolute.h"
#include "globals.h"
#include "simpleGates.h"
#include "arithmetic.h"
#include "toffoli.h"
#include <iostream>

// Quantum absolute value
// Converts input quantum register into absolute value. Sign qubit is transferred to ancilla qubit.
// Signed integers
// LSB first. Sign qubit is MSB.
// Source: https://arxiv.org/pdf/2005.00443
void QABS(std::vector<std::string> registerq, std::string ancilla){
    std::vector<std::string> regq = parseQregisterVector(registerq);
    int n = regq.size();
    cx(regq[n-1], ancilla);
    std::vector<std::string> subRegq(regq.begin(), regq.end() - 1);
    cQTC(subRegq, regq[n-1]);
    cx(ancilla, regq[n-1]);
}

// Quantum absolute value with single qubit control
// Converts input quantum register into absolute value. Sign qubit is transferred to ancilla qubit
// Signed integers
// LSB first. Sign qubit is MSB.
void cQABS(std::vector<std::string> registerq, std::string ancilla, std::string control){
    std::vector<std::string> regq = parseQregisterVector(registerq);
    int n = regq.size();
    std::vector<std::string> controls = {regq[n-1], control};
    toffoli(controls, ancilla);
    std::vector<std::string> subRegq(regq.begin(), regq.end() - 1);
    cQTC(subRegq, ancilla);
    cx(ancilla, regq[n-1]);
}