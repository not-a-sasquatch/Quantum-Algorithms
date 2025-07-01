#include "qft.h"
#include "globals.h"
#include "simpleGates.h"
#include "math.h"
#include "toffoli.h"
#include <iostream>

// Quantum Fourier transform
// LSB first
// The significance-order of the bits is reversed in the outputs (?)
void qft(std::vector<std::string> registers){
    std::vector<std::string> qubits = parseQregisterVector(registers);
    int n = qubits.size();
    for(int i = n-1; i >= 0; i--){
        h(qubits[i]);
        for(int j = i-1; j >= 0; j--){
            double theta = pi / pow(2,i-j);
            cphase(theta, qubits[j], qubits[i]);
        }
    }
}

// Quantum Fourier transform
// LSB first
// The significance-order of the bits is reversed in the outputs (?)
void cqft(std::vector<std::string> registers, std::string control, std::string ancilla){
    std::vector<std::string> qubits = parseQregisterVector(registers);
    int n = qubits.size();
    for(int i = n-1; i >= 0; i--){
        ch(control, qubits[i]);
        for(int j = i-1; j >= 0; j--){
            double theta = pi / pow(2,i-j);
            std::vector<std::string> controls = {control, qubits[j]};
            toffoli(controls, ancilla);
            cphase(theta, ancilla, qubits[i]);
            toffoli(controls, ancilla);
        }
    }
}

// Quantum Fourier transform approximated to k-qubit accuracy
void qftApprox(std::vector<std::string> registers, int k){
    std::vector<std::string> qubits = parseQregisterVector(registers);
    int n = qubits.size();
    for(int i = n-1; i >= 0; i--){
        h(qubits[i]);
        for(int j = i-1; j >= 0; j--){
            if(i - j < k){
                double theta = pi / pow(2,i-j);
                cphase(theta, qubits[j], qubits[i]);
            }
            
        }
    }
}