#include "qft.h"
#include "globals.h"
#include "simpleGates.h"
#include "math.h"
#include "toffoli.h"
#include <iostream>

// Quantum Fourier transform over Z_2^n, where n is the length of the input quantum register
// LSB first
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

// Quantum Fourier transform, where n is the length of the input quantum register, with single qubit control
// LSB first
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

// Quantum Fourier transform over 2^n, where n is the length of the input quantum register, approximated to k-qubit accuracy
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

// Quantum Fourier transform over Z_N
// LSB first
void qftn(std::vector<std::string> registers, int N){
    
}

// Quantum Fourier transform over Z_N, with single qubit control
// LSB first
void cqftn(std::vector<std::string> registers, int N, std::string control){
    
}