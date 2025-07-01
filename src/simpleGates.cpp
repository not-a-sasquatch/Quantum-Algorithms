#include "globals.h"
#include "simpleGates.h"
#include "toffoli.h"
#include <iostream>

// # define pi           3.14159265358979323846  /* pi */

// Built-in gates

// Single qubit U(x,y,z) gate (1/2)[[1+e^(ix),-ie^(iz)(1-e^(ix))];[ie^(iy)(1-e^(ix)),e^i(y+z)(1+e^(ix))]]
void u(double theta, double phi, double lambda, std::string qubit){
    addInstruction("U(" + std::to_string(theta) + ", " + std::to_string(phi) + ", " + std::to_string(lambda) + ") " + qubit);
}

// Controlled-U gate on a single qubit with single qubit control
void cu(double theta, double phi, double lambda, std::string control, std::string target){
    addInstruction("ctrl @ U(" + std::to_string(theta) + ", " + std::to_string(phi) + ", " + std::to_string(lambda) + ") " + control + ", " + target);
}

// Global phase [[e^(ix),0];[0,e^(ix)]]
void gphase(double theta){
    addInstruction("gphase " + std::to_string(theta));
}

// Pauli gates

// X gate on a single qubit [[0,1];[1,0]]
void x(std::string qubit){
    u(pi, -1*pi/2, pi/2, qubit);
}

// Controlled X gate on a single qubit with single qubit control
void cx(std::string control, std::string target){
    cu(pi, -1*pi/2, pi/2, control, target);
}

// Y gate on a single qubit [[0,-i];[i,0]]
void y(std::string qubit){
    u(pi, 0, 0, qubit);
}

// Controlled Y gate on a single qubit with single qubit control
void cy(std::string control, std::string target){
    cu(pi, 0, 0, control, target);
}

// Z gate on a single qubit [[1,0];[0,-1]]
void z(std::string qubit){
    u(0, 0, pi, qubit);
}

// Controlled Z gate on a single qubit with single qubit control
void cz(std::string control, std::string target){
    cu(0, 0, pi, control, target);
}

// Rotation gates

// Single-qubit rotation about x-axis [[cos(x/2), -isin(x/2)];[-isin(x/2), cos(x/2)]]
void rx(double theta, std::string qubit){
    u(theta, -1*pi/2, pi/2, qubit);
    gphase(-1*theta/2);
}

// Controlled-rx gate on a single qubit with single qubit control
void crx(double theta, std::string control, std::string target){
    cu(theta, -1*pi/2, pi/2, control, target);
    phase(-1*theta/2, control);
}

// Single-qubit rotation about y-axis [[cos(x/2), -sin(x/2)];[sin(x/2), cos(x/2)]]
void ry(double theta, std::string qubit){
    u(theta, 0, 0, qubit);
    gphase(-1*theta/2);
}

// Controlled-ry gate on a single qubit with single qubit control
void cry(double theta, std::string control, std::string target){
    cu(theta, 0, 0, control, target);
    phase(-1*theta/2, control);
}

// Single-qubit rotation about z-axis [[e^(-ix/2), 0];[0, e^(ix/2)]]
void rz(double theta, std::string qubit){
    u(0, theta, 0, qubit);
    gphase(-1*theta/2);
}

// Controlled-rz gate on a single qubit with single qubit control
void crz(double theta, std::string control, std::string target){
    cu(0, theta, 0, control, target);
    phase(-1*theta/2, control);
}

// Hadamard gate on a single qubit
void h(std::string qubit){
    u(pi/2, 0, pi, qubit);
    gphase(-1*pi/4);
}

// Hadamard gate on a single qubit with single qubit control
void ch(std::string control, std::string target){
    cu(pi/2, 0, pi, control, target);
    phase(-1*pi/4, control);
}

// Swap gate between two qubits
void swap(std::string qubit0, std::string qubit1){
    cx(qubit0, qubit1);
    cx(qubit1, qubit0);
    cx(qubit0, qubit1);
}

// Swap gate between two registers
void swapn(std::vector<std::string> register0, std::vector<std::string> register1){
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> reg1 = parseQregisterVector(register1);
    // check lengths are the same
    if(reg0.size() != reg1.size()){
        std::cout << "Beep boop - swapn error! Sizes dont match.\n";
        return;
    }
    for(int i = 0; i < reg0.size(); i++){
        swap(reg0[i], reg1[i]);
    }
}

// Controlled-swap gate between two qubits with a single qubit control
void cswap(std::string control, std::string target0, std::string target1){
    std::vector<std::string> cntrls = {control, target0};
    toffoli(cntrls, target1);
    cntrls = {control, target1};
    toffoli(cntrls, target0);
    cntrls = {control, target0};
    toffoli(cntrls, target1);

}

// Controlled-swap gate between two registers with a single qubit control
void cswapn(std::string control, std::vector<std::string> register0, std::vector<std::string> register1){
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> reg1 = parseQregisterVector(register1);
    // check lengths are the same
    if(reg0.size() != reg1.size()){
        std::cout << "Beep boop - cswapn error! Sizes dont match.\n";
        return;
    }
    for(int i = 0; i < reg0.size(); i++){
        cswap(control, reg0[i], reg1[i]);
    }
}

// Phase gate on a single qubit [[1,0];[0,e^(ix)]]
void phase(double theta, std::string qubit){
    u(0, 0, theta, qubit);
}

// Phase gate with a single qubit control
void cphase(double theta, std::string control, std::string target){
    phase(theta/2, control);
    cx(control, target);
    phase(-1*theta/2, target);
    cx(control, target);
    phase(theta/2, target);
}

// T gate on a single qubit
void t(std::string qubit){
    phase(pi/8, qubit);
}

// S gate on a single qubit
void s(std::string qubit){
    phase(pi/2, qubit);
}

// Copy register0 into register1 of 0-initialized qubit's
void copy(std::vector<std::string> register0, std::vector<std::string> register1){
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> reg1 = parseQregisterVector(register1);
    // check lengths are the same
    if(reg0.size() > reg1.size()){
        std::cout << "Beep boop - copy error! Sizes mismatch.\n";
        return;
    }
    for(int i = 0; i < reg0.size(); i++){
        cx(reg0[i], reg1[i]);
    }
}

// Copy register0 into register1 of 0-initialized qubit's with single qubit control
void ccopy(std::vector<std::string> register0, std::vector<std::string> register1, std::string control){
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> reg1 = parseQregisterVector(register1);
    // check lengths are the same
    if(reg0.size() > reg1.size()){
        std::cout << "Beep boop - ccopy error! Sizes dont match.\n";
        return;
    }
    for(int i = 0; i < reg0.size(); i++){
        std::vector<std::string> controls = {control, reg0[i]};
        toffoli(controls, reg1[i]);

    }
}

