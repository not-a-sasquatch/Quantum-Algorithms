#include "multiplication.h"
#include "globals.h"
#include "simpleGates.h"
#include "toffoli.h"
#include "arithmetic.h"
#include "euclidean.h"
#include <iostream>
#include "math.h"


////////////////////////////////////////
// Multiplication for unsigned integers
////////////////////////////////////////
// Multiply 2 quantum registers using multiply-accumulate method
// Unsigned integers, LSB first
// 'out-of-place' multiplication (product stored in result)
void mulMAC(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerAncillas, std::vector<std::string> registerResult){
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> reg1 = parseQregisterVector(register1);
    std::vector<std::string> result = parseQregisterVector(registerResult);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas); 
    // Check input sizes
    if(result.size() < reg0.size() + reg1.size()){
        std::cout << "Beep boop! mulMAC error - result size must be >= reg0 size + reg1 size.\n";
        return;
    } else if(ancillas.size() < reg0.size() + reg1.size()){
        std::cout << "Beep boop! mulMAC error - ancillas size must be >= reg0 size + reg1 size.\n";
        return;
    }
    for(int i = 0; i < reg0.size(); i++){
        // multiply reg0[i] and reg1 and store value in ancilla with zero-padding where necessary
        for(int j = 0; j < reg1.size(); j++){
            std::vector<std::string> controls = {reg0[i], reg1[j]};
            toffoli(controls, ancillas[i+j]);
        }
        // add ancilla to result (accumulator)
        addQFT(result, ancillas);
        // decompute ancilla
        for(int j = 0; j < reg1.size(); j++){
            std::vector<std::string> controls = {reg0[i], reg1[j]};
            toffoli(controls, ancillas[i+j]);
        }
    }
}
// Multiply 2 quantum registers using multiply-accumulate method with single qubit control
// Unsigned integers, LSB first
// 'out-of-place' multiplication (product stored in result)
void cmulMAC(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerAncillas, std::vector<std::string> registerResult, std::string control){
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> reg1 = parseQregisterVector(register1);
    std::vector<std::string> result = parseQregisterVector(registerResult);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas); 

    std::string ancillaQubit = ancillas[0];
    std::vector<std::string> ancillasMAC(ancillas.begin() + 1, ancillas.end());
    // check input sizes
    if(result.size() < reg0.size() + reg1.size()){
        std::cout << "Beep boop! cmulMAC error - result size must be >= reg0 size + reg1 size.\n";
        return;
    } else if(ancillas.size() < reg0.size() + reg1.size() + 1){
        std::cout << "Beep boop! cmulMAC error - ancillas size must be >= reg0 size + reg1 size + 1.\n";
        return;
    }
    int m0 = reg0.size();
    int m1 = reg1.size();
    for(int i = 0; i < m0; i++){
        // multiply reg0[i] and reg1 and store value in ancilla with zero-padding where necessary
        for(int j = 0; j < m1; j++){
            std::vector<std::string> controls = {control, reg0[i], reg1[j]};
            toffoli_n(controls, {ancillaQubit}, ancillasMAC[i+j]);
        }
        // add ancilla to result (accumulator)
        caddQFT(result, ancillasMAC, control, ancillaQubit);
        // decompute ancilla
        for(int j = 0; j < m1; j++){
            std::vector<std::string> controls = {control, reg0[i], reg1[j]};
            toffoli_n(controls, {ancillaQubit}, ancillasMAC[i+j]);
        }
    }
}
// Multiply a quantum register and classical register (C++ input) using multiply-accumulate method
// Unsigned integers, LSB first
// 'out-of-place' multiplication (product stored in result)
void mulMACval(std::vector<std::string> registerq, int N, std::vector<std::string> registerAncillas, std::vector<std::string> registerResult){
    std::vector<std::string> regq = parseQregisterVector(registerq);
    std::vector<std::string> result = parseQregisterVector(registerResult);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas); 
    
    int m = regq.size();
    int n = std::ceil(std::log2(N));
    std::vector<int> binArrayN = intToBinArray(N, n);
    // Check input sizes
    if(result.size() < m + n){
        std::cout << "Beep boop! mulMAC error - result size must be >= regq size + regN size.\n";
        return;
    } else if(ancillas.size() < m + n){
        std::cout << "Beep boop! mulMAC error - ancillas size must be >= regq size + regN size.\n";
        return;
    }
    for(int i = 0; i < n; i++){
        // multiply binArrayN[i] and regq and store value in ancilla with zero-padding where necessary
        if(binArrayN[i] > 0){
            for(int j = 0; j < m; j++){
                cx(regq[j], ancillas[i+j]);
            }
        }
        // add ancilla to result (accumulator)
        addQFT(result, ancillas);
        // decompute ancilla
        if(binArrayN[i] > 0){
            for(int j = 0; j < m; j++){
                cx(regq[j], ancillas[i+j]);
            }
        }
    }
}
// Multiply 2 quantum registers using multiply-accumulate method
// Unsigned integers, LSB first
// 'in-place' multiplication (product stored in result)
// Currnetly only works when gcd(register1, 2^n-1) = 1
void mulIPMAC(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerAncillas){
    // need modular inversion algorithm for quantum 
    // https://arxiv.org/pdf/quant-ph/0301141
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> reg1 = parseQregisterVector(register1);
    std::vector<std::string> ancillaReg = parseQregisterVector(registerAncillas);
    
    int m = reg0.size();
    int n = reg1.size();
    // Check input sizes
    if(ancillaReg.size() < 2*(m + n)){
        std::cout << "Beep boop! mulIPMACval error - ancillas size must be >= 2*(regq size + regN size).\n";
        return;
    }
    std::vector<std::string> result(ancillaReg.begin(), ancillaReg.begin()+ n + m);
    std::vector<std::string> reg1Inv(ancillaReg.begin()+ n + m, ancillaReg.end());
    std::vector<std::string> ancillaMAC(ancillaReg.begin()+ n + m, ancillaReg.end());
    std::vector<std::string> ancillaInv(ancillaReg.begin()+ n + m, ancillaReg.end());
    std::vector<std::string> ancillaMod(ancillaReg.begin()+ n + m, ancillaReg.end());
    // using QFT multiplication - which is multiplication mod 2^n-1 for n-qubit QFT
    mulMAC(reg0, reg1, ancillaMAC, result);
    // Put result in input register
    swapn(reg0, result);
    // using euclidean algorithm to calculate N^-1 mod 2^n - 1
    // Intialize ancillaMod as all 1's so eucilidean inverse is mod 2^n
    for(int i = 0; i < ancillaMod.size(); i++){
        x(ancillaMod[i]);
    }
    // Not sure if this works since reg1 may not have an inverse mod 2^n 
    modularInverse(reg1, ancillaMod, reg1Inv, ancillaInv);
    // Decompute result (input) using inverse
    //mulMAC(reg0, ancillaInv, ancillaMAC, result);
    inverse(mulMAC, reg0, reg1Inv, ancillaMAC, result);
    // Uncompute reg1Inv
    modularInverse(reg1, ancillaMod, reg1Inv, ancillaInv);
    for(int i = 0; i < ancillaMod.size(); i++){
        x(ancillaMod[i]);
    }

}
// Multiply a quantum register and classical register (C++ input) using multiply-accumulate method
// Unsigned integers, LSB first
// 'in-place' multiplication (product stored in registerq)
void mulIPMACval(std::vector<std::string> registerq, int N, std::vector<std::string> registerAncillas){
    std::vector<std::string> regq = parseQregisterVector(registerq);
    std::vector<std::string> ancillaReg = parseQregisterVector(registerAncillas);
    
    int m = regq.size();
    int n = std::ceil(std::log2(N));
    // Check input sizes
    if(ancillaReg.size() < 2*(m + n)){
        std::cout << "Beep boop! mulIPMACval error - ancillas size must be >= 2*(regq size + regN size).\n";
        return;
    }
    std::vector<std::string> result(ancillaReg.begin(), ancillaReg.begin()+ n + m);
    std::vector<std::string> ancillaMAC(ancillaReg.begin()+ n + m, ancillaReg.end());
    // using QFT multiplication - which is multiplication mod 2^n-1 for n-qubit QFT
    mulMACval(regq, N, ancillaMAC, result);
    // Put result in input register
    swapn(regq, result);
    // using euclidean algorithm to calculate N^-1 mod 2^n - 1
    int N_inv = modInvPowval2(N, n);
    // Decompute result (input) using inverse
    //mulMACval(regq, N_inv, ancillaMAC, result);
    inverse(mulMACval, regq, N_inv, ancillaMAC, result);
}
// Modular inverse
// return modular inverse of classical input with modulus pow(2,n) - 1
int modInvPowval2(int N, int n){

}
////////////////////////////////////////
// Multiplication for signed integers
////////////////////////////////////////

// Multiply 2 quantum registers using multiply-accumulate method
// Signed integers, LSB first
// 'out-of-place' multiplication (product stored in result)
void mulMAC_s(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerAncillas, std::vector<std::string> registerResult){
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> reg1 = parseQregisterVector(register1);
    std::vector<std::string> result = parseQregisterVector(registerResult);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas);
    int n = reg0.size();
    int m = reg1.size();
    if(result.size() < n + m - 1){
        std::cout << "Beep boop - mulMAC_s error! Result size must be < register0 size + register1 size - 1.\n";
    } else if(ancillas.size() < n + m){
        std::cout << "Beep boop - mulMAC_s error! Ancilla size must be >= register0 size + register1 size - 1.\n";
        return;
    }

    std::string ancillaQTC = ancillas[0];
    std::vector<std::string> ancillaTemp(ancillas.begin()+1, ancillas.end());
    // Convert inputs to absolute value
    std::vector<std::string> reg0Data(reg0.begin(), reg0.end()-1);
    cQTC(reg0Data, reg0[n-1], ancillaQTC);
    std::vector<std::string> reg1Data(reg1.begin(), reg1.end()-1);
    cQTC(reg1Data, reg1[m-1], ancillaQTC);
    for(int i = 0; i < n-1; i++){
        // multiply reg0[i] and reg1 and store value in ancilla with zero-padding where necessary
        for(int j = 0; j < m-1; j++){
            std::vector<std::string> controls = {reg0[i], reg1[j]};
            toffoli(controls, ancillaTemp[i+j]);
        }
        // add ancilla to result (accumulator)
        addQFT_s(result, ancillaTemp);
        // decompute ancilla
        for(int j = 0; j < m-1; j++){
            std::vector<std::string> controls = {reg0[i], reg1[j]};
            toffoli(controls, ancillaTemp[i+j]);
        }
    }
    // Correct result sign qubit. Perform QTC if negative
    cx(reg0[n-1], result[result.size()-1]);
    cx(reg1[m-1], result[result.size()-1]);
    std::vector<std::string> resultData(result.begin(), result.end()-1);
    cQTC(resultData, result[result.size()-1], ancillaQTC);
    // Reverse absolute value
    //std::vector<std::string> reg0Data(reg0.begin(), reg0.end()-1);
    cQTC(reg0Data, reg0[n-1], ancillaQTC);
    //std::vector<std::string> reg1Data(reg1.begin(), reg1.end()-1);
    cQTC(reg1Data, reg1[m-1], ancillaQTC);
}

// Multiply 2 quantum registers using multiply-accumulate method with single qubit control
// Signed integers, LSB first
// 'out-of-place' multiplication (product stored in result)
void cmulMAC_s(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerAncilla, std::vector<std::string> registerResult, std::string control){
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> reg1 = parseQregisterVector(register1);
    std::vector<std::string> result = parseQregisterVector(registerResult);
    std::vector<std::string> ancilla = parseQregisterVector(registerAncilla);
    int n = reg0.size();
    int m = reg1.size();
    if(result.size() < n + m - 1){
        std::cout << "Beep boop - mulMAC_s error! Result size must be < register0 size + register1 size - 1.\n";
    } else if(ancilla.size() < n + m + 1){
        std::cout << "Beep boop - mulMAC_s error! Ancilla size must be >= register0 size + register1 size + 1.\n";
        return;
    }
    
    std::string ancillaQubit = ancilla[0];
    std::string ancillaQTC = ancilla[1];
    std::vector<std::string> ancillaMAC(ancilla.begin() + 2, ancilla.end());

    // Convert inputs to absolute value
    std::vector<std::string> reg0Data(reg0.begin(), reg0.end()-1);
    toffoli({control, reg0[n-1]}, ancillaQubit);
    cQTC(reg0Data, ancillaQubit, ancillaQTC);
    toffoli({control, reg0[n-1]}, ancillaQubit);
    std::vector<std::string> reg1Data(reg1.begin(), reg1.end()-1);
    toffoli({control, reg1[m-1]}, ancillaQubit);  
    cQTC(reg1Data, ancillaQubit, ancillaQTC);
    toffoli({control, reg1[m-1]}, ancillaQubit);
    for(int i = 0; i < n-1; i++){
        // multiply reg0[i] and reg1 and store value in ancillaMAC with zero-padding where necessary
        for(int j = 0; j < m-1; j++){
            std::vector<std::string> controls = {control, reg0[i], reg1[j]};
            toffoli_n(controls, {ancillaQubit}, ancillaMAC[i+j]);
        }
        // add ancillaMAC to result (accumulator)
        caddQFT_s(result, ancillaMAC, control, ancillaQubit);
        // decompute ancillaMAC
        for(int j = 0; j < m-1; j++){
            std::vector<std::string> controls = {control, reg0[i], reg1[j]};
            toffoli_n(controls, {ancillaQubit}, ancillaMAC[i+j]);
        }
    }
    // Correct result sign qubit. Perform QTC if negative
    //cx(reg0[n-1], result[result.size()-1]);
    //cx(reg1[m-1], result[result.size()-1]);
    toffoli({control, reg0[n-1]}, result[result.size()-1]);
    toffoli({control, reg1[m-1]}, result[result.size()-1]);

    std::vector<std::string> resultData(result.begin(), result.end()-1);
    toffoli({control, result[result.size()-1]}, ancillaQubit);
    cQTC(resultData, ancillaQubit, ancillaQTC);
    toffoli({control, result[result.size()-1]}, ancillaQubit);
    // Reverse absolute value
    //std::vector<std::string> reg0Data(reg0.begin(), reg0.end()-1);
    toffoli({control, reg0[n-1]}, ancillaQubit);
    cQTC(reg0Data, ancillaQubit, ancillaQTC);
    toffoli({control, reg0[n-1]}, ancillaQubit);
    //std::vector<std::string> reg1Data(reg1.begin(), reg1.end()-1);
    toffoli({control, reg1[m-1]}, ancillaQubit);
    cQTC(reg1Data, reg1[m-1], ancillaQTC);
    toffoli({control, reg1[m-1]}, ancillaQubit);
}

// Multiply a quantum register by a classical value (C++ input) using multiply-accumulate method
// Signed integers, LSB first
// 'out-of-place' multiplication (product stored in result)
void mulMACval_s(std::vector<std::string> registerq, int N, std::vector<std::string> registerAncillas, std::vector<std::string> registerResult){
    std::vector<std::string> regq = parseQregisterVector(registerq);
    std::vector<std::string> result = parseQregisterVector(registerResult);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas);
    int n = std::ceil(std::log2(std::abs(N))) + 1; // +1 for sign bit two's complement
    std::vector<int> binArrayN = intToBinArray(N, std::ceil(std::log2(N)) + 1);
    int m = regq.size();
    // check size result = n+m-1
    if(result.size() < n + m - 1){
        std::cout << "Beep boop - mulMAC_s error! Result size must be >= register0 size + register1 size - 1.\n";
    } else if(ancillas.size() < n + m){
        std::cout << "Beep boop - mulMAC_s error! Ancilla size must be >= register0 size + register1 size - 1.\n";
    }

    std::string ancillaQTC = ancillas[0];
    std::vector<std::string> ancillaTemp(ancillas.begin()+1, ancillas.end());

    // Convert inputs to absolute value
    std::vector<std::string> regqData(regq.begin(), regq.end()-1);
    cQTC(regqData, regq[n-1], ancillaQTC);
    for(int i = 0; i < m-1; i++){
        // multiply reg0[i] and reg1 and store value in ancilla with zero-padding where necessary
        for(int j = 0; j < n-1; j++){
            if(binArrayN[j] > 0){
                cx(regq[i], ancillaTemp[i+j]);
            }
        }
        // add ancilla to result (accumulator)
        addQFT_s(result, ancillaTemp);
        // decompute ancilla
        for(int j = 0; j < m-1; j++){
            if(binArrayN[j] > 0){
                cx(regq[i], ancillaTemp[i+j]);
            }
        }
    }
    // Correct result sign qubit. Perform QTC if negative
    if(binArrayN[n-1]){
        x(regq[n-1]);
        cx(regq[n-1], result[result.size()-1]);
        x(regq[n-1]);
    }
    std::vector<std::string> resultData(result.begin(), result.end()-1);
    cQTC(resultData, result[result.size()-1], ancillaQTC);
    // Reverse absolute value
    cQTC(regqData, regq[n-1], ancillaQTC);
}
