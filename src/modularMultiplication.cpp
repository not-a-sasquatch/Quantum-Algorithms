#include "modularMultiplication.h"
#include "globals.h"
#include "simpleGates.h"
#include "toffoli.h"
#include "arithmetic.h"
#include "math.h"
#include <iostream>

// Modular shift and reduce circuit, with classical modulus N (C++ input)
// Unsigned integers, LSB first (?)
// Source: High Performance Quantum Modular Multipliers, R. Rines, I. Chuang, https://arxiv.org/pdf/1801.01081
// TO DO: Verify LSB first
// Incomplete
void modSARval(std::vector<std::string> registerq, std::vector<std::string> registerAncillas, int N){
    std::vector<std::string> regq = parseQregisterVector(registerq);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas);
    std::string ancilla0 = ancillas[0];
    std::string ancilla1 = ancillas[1];
    std::string ancillaAdd = ancillas[2];

    /////////////////////
    // need to check input sizes
    /////////////////////
    int m = regq.size();
    int n = std::ceil(std::log2(N)) + 1; // +1 for sign bit two's complement
    std::vector<int> binArrayN = intToBinArray(N, std::ceil(std::log2(N)) + 1);

    regq.push_back(ancilla0);
    // Subtract N
    addQFTval(regq, N);
    // Flip control qubit
    cx(regq[0], ancilla1);
    // Controlled reverse subtract N
    //binArrayN[binArryN.size()-1] = 1; // does this work?
    csubQFTval(regq, ancilla1, ancillaAdd, N);
    // Clear control qubit
    cx(ancilla0, ancilla1);
    // Cleanup swap
    swap(ancilla0, regq[0]);

}

////////////////////////////////////////
// Modular multiplication for signed integers
////////////////////////////////////////
// Multiply a quantum number by another quantum number modulus a quantum number
// Signed integers, LSB first
// Modifed from https://arxiv.org/pdf/1801.01081
// Incomplete
void modMul_s(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerN, std::vector<std::string> registerResult, std::vector<std::string> registerAncillas){

}



// Multiply a quantum number by another quantum number modulus a classical number N (C++ input)
// Signed integers, LSB first
// Modifed from https://arxiv.org/pdf/1801.01081
// Incomplete
void modMulval_s(std::vector<std::string> register0, std::vector<std::string> register1, int N, std::vector<std::string> registerResult, std::vector<std::string> registerAncillas){
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> reg1 = parseQregisterVector(register1);
    std::vector<std::string> result = parseQregisterVector(registerResult);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas); 

    /////////////////////
    // need to check input sizes
    /////////////////////
    int m0 = reg0.size();
    int m1 = reg1.size();
    int n = std::ceil(std::log2(std::abs(N))) + 1; // +1 for sign bit two's complement
    std::vector<int> binArrayN = intToBinArray(N, n);
    // check input sizes
    if(m0 < m1){

    } else if(m0 < n){

    } else if(ancillas.size() < 2*(m0 + n - 1)){

    } else if(result.size() < m0 + n - 1){

    }

    std::vector<std::string> ancillasTemp(ancillas.begin(), ancillas.begin() + n + m1);
    std::vector<std::string> ancillasCalc(ancillas.begin() + n + m1, ancillas.end());

    // Multiplication stage
    mulMAC_s(reg0, reg1, ancillasCalc, ancillasTemp);
    // Division/Modulus stage
    modulusval_s(ancillasTemp, N, result, ancillasCalc);
    // Uncompute multiplication ancillas
    inverse(mulMAC_s, reg0, reg1, ancillasCalc, ancillasTemp);
}

// Multiply a quantum number by a classical number val (C++ input) modulus a quantum number N
// Signed integers, LSB first
// Modifed from https://arxiv.org/pdf/1801.01081
// Incomplete
void modMulval2_s(std::vector<std::string> registerq, int val, std::vector<std::string> registerN, std::vector<std::string> registerResult, std::vector<std::string> registerAncillas){
    std::vector<std::string> regq = parseQregisterVector(registerq);
    std::vector<std::string> regN = parseQregisterVector(registerN);
    std::vector<std::string> result = parseQregisterVector(registerResult);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas); 

    /////////////////////
    // need to check input sizes
    /////////////////////
    int m = regq.size();
    int n = regN.size();
    int v = std::ceil(std::log2(std::abs(val))) + 1; // +1 for sign bit two's complement
    std::vector<int> binArrayVal = intToBinArray(val, v);
    // check input sizes
    if(m < v){

    } else if(m < n){

    } else if(ancillas.size() < 2*(m + n - 1)){

    } else if(result.size() < m + n - 1){
        
    }
    std::vector<std::string> ancillasTemp(ancillas.begin(), ancillas.begin() + n);
    std::vector<std::string> ancillasCalc(ancillas.begin() + n, ancillas.end());

    // Multiplication stage
    mulMACval_s(regq, val, ancillasCalc, ancillasTemp);
    // Division/Modulus stage (modulus_s not yet implemented)
    modulus_s(ancillasTemp, regN, result, ancillasCalc);
    // Uncompute multiplication
    inverse(mulMACval_s, regq, val, ancillasCalc, ancillasTemp);

}

// Multiply a quantum number by a classical number val (C++ input) modulus a classical number N (C++ input)
// Signed integers, LSB first
// Modifed from https://arxiv.org/pdf/1801.01081
// Incomplete
void modMulval3_s(std::vector<std::string> registerq, int val, int N, std::vector<std::string> registerResult, std::vector<std::string> registerAncillas){
    std::vector<std::string> regq = parseQregisterVector(registerq);
    std::vector<std::string> result = parseQregisterVector(registerResult);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas); 

    /////////////////////
    // need to check input sizes
    /////////////////////
    int m = regq.size();
    int n = std::ceil(std::log2(std::abs(N))) + 1; // +1 for sign bit two's complement
    std::vector<int> binArrayN = intToBinArray(N, n);
    int v = std::ceil(std::log2(std::abs(val))) + 1; // +1 for sign bit two's complement
    std::vector<int> binArrayVal = intToBinArray(val, v);
    // check input sizes
    if(m < v){

    } else if(m < n){

    } else if(ancillas.size() < 2*(m + n - 1)){

    } else if(result.size() < m + n - 1){
        
    }
    std::vector<std::string> ancillasTemp(ancillas.begin(), ancillas.begin() + n);
    std::vector<std::string> ancillasCalc(ancillas.begin() + n, ancillas.end());

    // Multiplication stage
    mulMACval_s(regq, val, ancillasCalc, ancillasTemp);
    // Division/Modulus stage
    modulusval_s(ancillasTemp, N, result, ancillasCalc);
    // Uncompute multiplication
    inverse(mulMACval_s, regq, val, ancillasCalc, ancillasTemp);

}