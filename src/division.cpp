#include "division.h"
#include "globals.h"
#include "simpleGates.h"
#include "arithmetic.h"
#include "toffoli.h"
#include "math.h"
#include <iostream>

// Quantum divider, out-of-place division
// Calculates q/N, for Quantum q (m qubits) and classical N (n bits)
// results: registerModulus = q mod N, registerQuotient = floor(q/N)
// Signed integers, LSB first
// Modified from algorithm in https://arxiv.org/pdf/1801.01081
void div_s(std::vector<std::string> registerq, std::vector<std::string> registerN, std::vector<std::string> registerAncillas, std::vector<std::string> registerQuotient, std::vector<std::string> registerModulus){
    std::vector<std::string> regq = parseQregisterVector(registerq); // m qubits
    std::vector<std::string> regN = parseQregisterVector(registerN); // n qubits
    std::vector<std::string> quotient = parseQregisterVector(registerQuotient);  // m qubits
    std::vector<std::string> modulus = parseQregisterVector(registerModulus);  // m qubits
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas); 
    std::vector<std::string> result(modulus.begin(), modulus.end());
    result.insert(result.end(), quotient.begin(), quotient.end());
    //
    // need to check sizes of quotient/modulus/result/regq
    //
    int n = regN.size();
    int m = regq.size();
    if (m <= 0){
        std::cout << "Beep boop - divval_s error! Size mismatch.\n";
    } else if(n < m){
        std::cout << "Beep boop - divval_s error! Registerq size must be >= registerN size.\n";
    }
    std::string ancilla0 = ancillas[0]; // regq sign bit
    std::string ancilla1 = ancillas[1]; // regN sign bit
    std::string ancillaAdder = ancillas[2];
    std::string ancillaQTC = ancillas[2];
    std::vector<std::string> ancillasShift(ancillas.begin()+3, ancillas.begin()+3+m); // Ancillas for qubit packing
    // Store regq sign bit in ancilla0
    swap(regq[regq.size()-1], ancilla0);
    // Take two's complement if regq negative
    std::vector<std::string> dataRegq(result.begin(), result.end() - 1);
    cQTC(dataRegq, ancilla0, ancillaQTC);
    // Store regN sign bit in ancilla1
    swap(regN[regN.size()-1], ancilla1);
    // Take two's complement if regq negative
    std::vector<std::string> dataRegN(regN.begin(), regN.end() - 1);
    cQTC(dataRegN, ancilla1, ancillaQTC);
    // Copy |regq| to LSB's of result
    for(int i = 0; i < regq.size()-1; i++){
        cx(regq[i], result[i]);
    }
    // Division algorithm 
    // Multiply divisor by 2^i / append 0-qubits in front, perform trial subtraction and conditional re-addition
    for(int i = m-1; i>=0; i--){
        // Pack 0's in front of divisor
        std::vector<std::string> divValue;
        for(int j = 0; j < i; j++){
            divValue.push_back(ancillasShift[j]);
        }
        divValue.insert(divValue.end(), regN.begin(), regN.end());
        // Make subarray of qubits
        std::vector<std::string> subRegq(result.begin(), result.end() - (m - 1 - i));
        // Trial subtraction
        subQFT_s(subRegq, divValue);
        // Controlled re-addition
        std::vector<std::string> subReg2(result.begin(), result.end() - (m - i));
        caddQFT_s(subReg2, divValue, subRegq[subRegq.size() - 1], ancillaAdder);
    } 
    // if regq > 0 & regN < 0 or regq < 0 & regN > 0, flip sign bits on quotient and modulus
    std::vector<std::string> dataQuotient(quotient.begin(), quotient.end()-2);
    std::vector<std::string> dataModulus(modulus.begin(), modulus.end()-2);
    std::vector<std::string> controls = {ancilla0, ancilla1};
    x(ancilla0);
    toffoli(controls, quotient[quotient.size()-1]);
    toffoli(controls, modulus[modulus.size()-1]); 
    x(ancilla0);
    x(ancilla1);
    toffoli(controls, quotient[quotient.size()-1]);
    toffoli(controls, modulus[modulus.size()-1]);
    x(ancilla1);
    cQTC(dataQuotient, quotient[quotient.size()-1], ancillaQTC);
    cQTC(dataModulus, modulus[modulus.size()-1], ancillaQTC);
    // Add sign to regq
    swap(regq[regq.size()-1], ancilla0);
    // Add sign to regN
    cQTC(dataRegN, ancilla1, ancillaQTC);
    swap(regN[regN.size()-1], ancilla1);
    // at this point, registerModulus[0:m-1] = regq mod N, registerQuotient[0:m-1] = floor(input/N)
}

// Quantum divider, out-of-place division, with single qubit control
// Calculates q/N, for Quantum q (m qubits) and classical N (n bits)
// results: registerModulus = q mod N, registerQuotient = floor(q/N)
// Signed integers, LSB first
// Modified from algorithm in https://arxiv.org/pdf/1801.01081
void cdiv_s(std::vector<std::string> registerq, std::vector<std::string> registerN, std::vector<std::string> registerAncillas, std::vector<std::string> registerQuotient, std::vector<std::string> registerModulus, std::string control){
    std::vector<std::string> regq = parseQregisterVector(registerq); // m qubits
    std::vector<std::string> regN = parseQregisterVector(registerN); // n qubits
    std::vector<std::string> quotient = parseQregisterVector(registerQuotient);  // m qubits
    std::vector<std::string> modulus = parseQregisterVector(registerModulus);  // m qubits
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas); 
    std::vector<std::string> result(modulus.begin(), modulus.end());
    result.insert(result.end(), quotient.begin(), quotient.end());
    //
    // need to check sizes of quotient/modulus/result/regq
    //
    int n = regN.size();
    int m = regq.size();
    if (m <= 0){
        std::cout << "Beep boop - divval_s error! Size mismatch.\n";
    } else if(n < m){
        std::cout << "Beep boop - divval_s error! Registerq size must be >= registerN size.\n";
    }
    std::string ancilla0 = ancillas[0]; // regq sign bit
    std::string ancilla1 = ancillas[1]; // regN sign bit
    std::string ancillaAdder = ancillas[2];
    std::string ancillaQTC = ancillas[2];
    std::string flag = ancillas[3];
    std::vector<std::string> ancillasShift(ancillas.begin()+4, ancillas.begin()+3+m); // Ancillas for qubit packing
    // Store regq sign bit in ancilla0
    cswap(control, regq[regq.size()-1], ancilla0);
    // Take two's complement if regq negative
    std::vector<std::string> dataRegq(result.begin(), result.end() - 1);
    toffoli({ancilla0, control}, flag);
    cQTC(dataRegq, flag, ancillaQTC);
    toffoli({ancilla0, control}, flag);
    // Store regN sign bit in ancilla1
    cswap(control, regN[regN.size()-1], ancilla1);
    // Take two's complement if regq negative
    std::vector<std::string> dataRegN(regN.begin(), regN.end() - 1);
    toffoli({ancilla1, control}, flag);
    cQTC(dataRegN, flag, ancillaQTC);
    toffoli({ancilla1, control}, flag);
    // Copy |regq| to LSB's of result
    for(int i = 0; i < regq.size()-1; i++){
        toffoli({regq[i], control}, result[i]);
        //cx(regq[i], result[i]);
    }
    // Division algorithm 
    // Multiply divisor by 2^i / append 0-qubits in front, perform trial subtraction and conditional re-addition
    for(int i = m-1; i>=0; i--){
        // Pack 0's in front of divisor
        std::vector<std::string> divValue;
        for(int j = 0; j < i; j++){
            divValue.push_back(ancillasShift[j]);
        }
        divValue.insert(divValue.end(), regN.begin(), regN.end());
        // Make subarray of qubits
        std::vector<std::string> subRegq(result.begin(), result.end() - (m - 1 - i));
        // Trial subtraction
        csubQFT_s(subRegq, divValue, control, ancillaAdder);
        // Controlled re-addition
        std::vector<std::string> subReg2(result.begin(), result.end() - (m - i));
        toffoli({subRegq[subRegq.size() - 1], control}, flag);
        caddQFT_s(subReg2, divValue, subRegq[subRegq.size() - 1], ancillaAdder);
        toffoli({subRegq[subRegq.size() - 1], control}, flag);
    } 
    // if regq > 0 & regN < 0 or regq < 0 & regN > 0, flip sign bits on quotient and modulus
    std::vector<std::string> dataQuotient(quotient.begin(), quotient.end()-2);
    std::vector<std::string> dataModulus(modulus.begin(), modulus.end()-2);
    std::vector<std::string> controls = {control, ancilla0, ancilla1};
    cx(control, ancilla0);
    toffoli_n(controls, {ancillaAdder}, quotient[quotient.size()-1]);
    toffoli_n(controls, {ancillaAdder}, modulus[modulus.size()-1]); 
    cx(control, ancilla0);
    cx(control, ancilla1);
    toffoli_n(controls, {ancillaAdder}, quotient[quotient.size()-1]);
    toffoli_n(controls, {ancillaAdder}, modulus[modulus.size()-1]);
    cx(control, ancilla1);
    toffoli({quotient[quotient.size()-1], control}, flag);
    cQTC(dataQuotient, flag, ancillaQTC);
    toffoli({quotient[quotient.size()-1], control}, flag);
    toffoli({modulus[modulus.size()-1], control}, flag);
    cQTC(dataModulus, flag, ancillaQTC);
    toffoli({modulus[modulus.size()-1], control}, flag);
    // Add sign to regq
    cswap(control, regq[regq.size()-1], ancilla0);
    // Add sign to regN
    toffoli({ancilla1, control}, flag);
    cQTC(dataRegN, flag, ancillaQTC);
    toffoli({ancilla1, control}, flag);
    cswap(control, regN[regN.size()-1], ancilla1);
    // at this point, registerModulus[0:m-1] = regq mod N, registerQuotient[0:m-1] = floor(input/N)
}


// Quantum divider with classical divisor (c++ input), out-of-place division
// Calculates q/N, for Quantum q (n+m qubits) and classical N (n bits)
// results: registerModulus = q mod N, registerQuotient = floor(q/N)
// Signed integers, LSB first
// Modified from algorithm in https://arxiv.org/pdf/1801.01081
void divval_s(std::vector<std::string> registerq, int N, std::vector<std::string> registerAncillas, std::vector<std::string> registerQuotient, std::vector<std::string> registerModulus){
    int negDivisor = 0;
    if (N == 0){
        std::cout << "Beep boop! divval_s error - N must be non-zero.\n";
    } else if(N < 0){
        N = -1 * N;
        negDivisor = 1;
    }
    std::vector<std::string> regq = parseQregisterVector(registerq);
    std::vector<std::string> quotient = parseQregisterVector(registerQuotient);
    std::vector<std::string> modulus = parseQregisterVector(registerModulus);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas);
    std::vector<std::string> result(modulus.begin(), modulus.end());
    result.insert(result.end(), quotient.begin(), quotient.end());
    // check sizes of quotient/modulus/result/regq
    for(int i = 0; i < regq.size(); i++){
        cx(regq[i], result[i]);
    }
    int n = std::ceil(std::log2(N)) + 1; // +1 for sign bit
    int m = regq.size() - n;
    if (m <= 0){
        std::cout << "Beep boop - divval_s error! Size mismatch.\n";
    }
    std::string ancillaSign = ancillas[0];
    std::string ancillaQTC = ancillas[1];
    std::string ancillaAdd = ancillas[1];

    // Store regq sign bit in ancilla
    swap(result[result.size()-1], ancillaSign);
    // Take two's complement if regq negative
    std::vector<std::string> dataRegq(result.begin(), result.end() - 1);
    cQTC(dataRegq, ancillaSign, ancillaQTC);

    // Division stage
    for(int i = m-1; i>=0; i--){
        int multValue = N*pow(2,i);
        // Trial subtraction
        //std::vector<int> binArr = intToBinArray(multValue, n+i+1);
        // Make subarray of qubits
        std::vector<std::string> subRegq(result.begin(), result.end() - (m - 1 - i));
        // Flip sign bit to make addition into subtraction
        subQFTval(subRegq, multValue);
        // Controlled re-addition
        //std::vector<int> binArr2 = intToBinArray(multValue, n+i);
        std::vector<std::string> subReg2(result.begin(), result.end() - (m - i));
        caddQFTval(subReg2, subRegq[subRegq.size() - 1], ancillaAdd, multValue);
    } 
    // Correct sign bit
    if(negDivisor){
        x(ancillaSign);
        swap(quotient[quotient.size()-1], ancillaSign);
    } else {
        swap(quotient[quotient.size()-1], ancillaSign);
    }
    std::vector<std::string> dataQuotient(quotient.begin()+n, quotient.end()-2);
    cQTC(dataQuotient, quotient[quotient.size()-1], ancillaSign);
    // at this point, registerModulus[0:n-1] = result[0:n-1] = input mod N, registerQuotient[0:m-1] = result[n:n+m-1] = floor(input/N) (quotient)
}

// Quantum divider with classical divisor (c++ input), in-place division
// Calculates q/N, for Quantum q (n+m qubits) and classical N (n bits)
// results: regq[0:m-1] = q mod N, q[m:n+m-1] = floor(q/N)
// Signed integers, LSB first
// Modified from algorithm in https://arxiv.org/pdf/1801.01081
void divvalIP_s(std::vector<std::string> registerq, int N, std::vector<std::string> registerAncillas){
    int negDivisor = 0;
    if (N == 0){
        std::cout << "Beep boop! divvalIP_s error - N must be non-zero.\n";
    } else if(N < 0){
        N = -1 * N;
        negDivisor = 1;
    }
    std::vector<std::string> regq = parseQregisterVector(registerq);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas);
    int n = std::ceil(std::log2(N)) + 1; // +1 for sign bit
    int m = regq.size() - n;
    if (m <= 0){
        std::cout << "Beep boop - divvalIP_s error! Size mismatch.\n";
    }

    std::string ancillaSign = ancillas[0];
    std::string ancillaQTC = ancillas[1];
    std::string ancillaAdd = ancillas[1];
    // Store regq sign bit in ancilla
    swap(regq[regq.size()-1], ancillaSign);
    // Take two's complement if regq negative
    std::vector<std::string> dataRegq(regq.begin(), regq.end() - 1);
    cQTC(dataRegq, ancillaSign, ancillaQTC);

    // Division stage
    for(int i = m-1; i>=0; i--){
        int multValue = N*pow(2,i);
        // Trial subtraction
        //std::vector<int> binArrayN = intToBinArray(multValue, n+i+1);
        // Make subarray of qubits
        std::vector<std::string> subRegq(regq.begin(), regq.end() - (m - 1 - i));
        // Flip sign bit to make addition into subtraction
        subQFTval(subRegq, multValue);
        // Controlled re-addition
        //std::vector<int> binArr2 = intToBinArray(multValue, n+i);
        std::vector<std::string> subReg2(regq.begin(), regq.end() - (m - i));
        caddQFTval(subReg2, subRegq[subRegq.size() - 1], ancillaAdd, multValue);
    } 
    // Correct sign bit
    if(negDivisor){
        x(ancillaSign);
        swap(regq[regq.size()-1], ancillaSign);
    } else {
        swap(regq[regq.size()-1], ancillaSign);
    }
    std::vector<std::string> subReg3(regq.begin()+n, regq.end()-2);
    cQTC(subReg3, regq[regq.size()-1], ancillaQTC);
    // at this point, registerq[0:n-1] = input mod N, registerq[n:n+m-1] = input/N (quotient)
}

// Quantum divider with classical divisor (openQASM input), in-place division
// Calculates q/N, for Quantum q (n+m qubits) and classical N (n bits)
// results: regq[0:m-1] = q mod N, q[m:n+m-1] = floor(q/N)
// Signed integers, LSB first
// Modified from algorithm in https://arxiv.org/pdf/1801.01081
void divcIP_s(std::vector<std::string> registerq, std::vector<std::string> registerc, std::vector<std::string> registerAncillas){
    
}

// Uncompute quotient from the output from divval_s
// This assumes N > 0 (?)
// Does this handle signs correctly
void uncomputeQuotientval_s(std::vector<std::string> registerq, std::vector<std::string> registerQuotient, std::vector<std::string> registerModulus, int N, std::vector<std::string> registerAncillas){
    std::vector<std::string> regq = parseQregisterVector(registerq);
    std::vector<std::string> quotient = parseQregisterVector(registerQuotient);
    std::vector<std::string> modulus = parseQregisterVector(registerModulus);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas);
    int n = std::ceil(std::log2(std::abs(N))) + 1; // +1 for sign bit two's complement
    std::vector<int> binArrayN = intToBinArray(N, std::ceil(std::log2(N)) + 1);
    /////////////////////
    // need to check input sizes
    /////////////////////
    // quotient*N
    //multiplyIPqcMAC(quotient, binArrayN, ancillas);
    mulIPMACval(quotient, N, ancillas);
    // quotient*N + modulus (input mod N) = input
    addQFT(quotient, modulus);
    // subtract regq from quotient
    subQFT(quotient, regq);
}

// Uncompute quotient from the output from div_s
// Are signs handled correctly here?
void uncomputeQuotient_s(std::vector<std::string> registerq, std::vector<std::string> registerN, std::vector<std::string> registerQuotient, std::vector<std::string> registerModulus, std::vector<std::string> registerAncillas){
    std::vector<std::string> regq = parseQregisterVector(registerq);
    std::vector<std::string> regN = parseQregisterVector(registerN);
    std::vector<std::string> quotient = parseQregisterVector(registerQuotient);
    std::vector<std::string> modulus = parseQregisterVector(registerModulus);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas); 
    /////////////////////
    // need to check input sizes
    /////////////////////
    // quotient*N
    mulIPMAC(quotient, regN, ancillas);
    // quotient*N + modulus (input mod N) = input
    addQFT(quotient, modulus);
    // subtract regq from quotient
    subQFT(quotient, regq);
}

// Uncompute quotient from the output from divval_s
// Signed integers, LSB first
void uncomputeModulusval_s(std::vector<std::string> registerq, std::vector<std::string> registerQuotient, std::vector<std::string> registerModulus, int N, std::vector<std::string> registerAncillas){
    std::vector<std::string> regq = parseQregisterVector(registerq);
    std::vector<std::string> quotient = parseQregisterVector(registerQuotient);
    std::vector<std::string> modulus = parseQregisterVector(registerModulus);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas); 
    int n = std::ceil(std::log2(N)) + 1; // +1 for sign bit two's complement
    std::vector<int> binArrayN = intToBinArray(N, std::ceil(std::log2(N)) + 1);
    /////////////////////
    std::vector<std::string> ancillaMult(ancillas.begin(), ancillas.begin() + n); //(?)
    std::vector<std::string> ancillaResult(ancillas.begin(), ancillas.begin() + n); //(?)
    /////////////////////
    // need to check input sizes
    /////////////////////
    // Calculate floor(q/N) * N, store in ancillaResult
    mulMACval(quotient, N, ancillaMult, ancillaResult);
    // Add result to modulus, q mod N + floor(q/N) * N = q
    addQFT(modulus, ancillaResult);
    // Subtract q
    subQFT(modulus, regq);
    // Uncompute ancillaResult
    inverse(mulMACval, quotient, N, ancillaMult, ancillaResult);
}

// Uncompute quotient from the output from div_s
// Signed integers, LSB first
void uncomputeModulus_s(std::vector<std::string> registerq, std::vector<std::string> registerN, std::vector<std::string> registerQuotient, std::vector<std::string> registerModulus, std::vector<std::string> registerAncillas){
    std::vector<std::string> regq = parseQregisterVector(registerq);
    std::vector<std::string> regN = parseQregisterVector(registerN);
    std::vector<std::string> quotient = parseQregisterVector(registerQuotient);
    std::vector<std::string> modulus = parseQregisterVector(registerModulus);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas); 
    int n = regN.size();
    /////////////////////
    std::vector<std::string> ancillaMult(ancillas.begin(), ancillas.begin() + n); //(?)
    std::vector<std::string> ancillaResult(ancillas.begin() + n, ancillas.end()); //(?)
    /////////////////////
    // need to check input sizes
    /////////////////////
    // Calculate floor(q/N) * N, store in ancillaResult
    mulMAC(quotient, regN, ancillaMult, ancillaResult);
    // Add result to modulus, q mod N + floor(q/N) * N = q
    addQFT(modulus, ancillaResult);
    // Subtract q
    subQFT(modulus, regq);
    // Uncompute ancillaResult
    inverse(mulMAC, quotient, regN, ancillaMult, ancillaResult);
}

// Uncompute quotient from the output from div_s, with single qubit control
// Signed integers, LSB first
void cuncomputeModulus_s(std::vector<std::string> registerq, std::vector<std::string> registerN, std::vector<std::string> registerQuotient, std::vector<std::string> registerModulus, std::vector<std::string> registerAncillas, std::string control){
    std::vector<std::string> regq = parseQregisterVector(registerq);
    std::vector<std::string> regN = parseQregisterVector(registerN);
    std::vector<std::string> quotient = parseQregisterVector(registerQuotient);
    std::vector<std::string> modulus = parseQregisterVector(registerModulus);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas); 
    int n = regN.size();
    /////////////////////
    std::vector<std::string> ancillaMult(ancillas.begin(), ancillas.begin() + n + 1); //(?)
    std::vector<std::string> ancillaResult(ancillas.begin() + n + 1, ancillas.end()); //(?)
    /////////////////////
    // need to check input sizes
    /////////////////////
    // Calculate floor(q/N) * N, store in ancillaResult
    cmulMAC(quotient, regN, ancillaMult, ancillaResult, control);
    // Add result to modulus, q mod N + floor(q/N) * N = q
    caddQFT(modulus, ancillaResult, control, ancillaMult[0]);
    // Subtract q
    csubQFT(modulus, regq, control, ancillaMult[0]);
    // Uncompute ancillaResult
    cmulMAC(quotient, regN, ancillaMult, ancillaResult, control);
}

// Quantum quotient, computes floor(q/N)
// Signed integers, LSB first
// Incomplete
void quotient_s(std::vector<std::string> registerq, std::vector<std::string> registerN, std::vector<std::string> registerResult, std::vector<std::string> registerAncillas){
    std::vector<std::string> regq = parseQregisterVector(registerq);
    std::vector<std::string> regN = parseQregisterVector(registerN);
    std::vector<std::string> result = parseQregisterVector(registerResult);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas); 
    int m = regq.size();
    int n = regN.size();
    /////////////////////
    // need to check input sizes
    /////////////////////
    std::vector<std::string> quotient(result.begin(), result.end()); //(?)
    std::vector<std::string> modulus(ancillas.begin(), ancillas.begin() + n); //(?)
    std::vector<std::string> ancillasDiv(ancillas.begin() + n, ancillas.end()); //(?)
    div_s(regq, regN, ancillasDiv, quotient, modulus);
    uncomputeModulus_s(regq, regN, quotient, modulus, ancillasDiv);
}

// Quantum quotient, computes floor(q/N), with single qubit control
// Signed integers, LSB first
// Incomplete
void cquotient_s(std::vector<std::string> registerq, std::vector<std::string> registerN, std::vector<std::string> registerResult, std::vector<std::string> registerAncillas, std::string control){
    std::vector<std::string> regq = parseQregisterVector(registerq);
    std::vector<std::string> regN = parseQregisterVector(registerN);
    std::vector<std::string> result = parseQregisterVector(registerResult);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas); 
    int m = regq.size();
    int n = regN.size();
    /////////////////////
    // need to check input sizes
    /////////////////////
    std::vector<std::string> quotient(result.begin(), result.end()); //(?)
    std::vector<std::string> modulus(ancillas.begin(), ancillas.begin() + n); //(?)
    std::vector<std::string> ancillasDiv(ancillas.begin() + n, ancillas.end()); //(?)
    cdiv_s(regq, regN, ancillasDiv, quotient, modulus, control);
    cuncomputeModulus_s(regq, regN, quotient, modulus, ancillasDiv, control);
}


// Computes modulus q mod N
// Signed integers, LSB first
// Incomplete
void modulus_s(std::vector<std::string> registerq, std::vector<std::string> registerN, std::vector<std::string> registerResult, std::vector<std::string> registerAncillas){
    std::vector<std::string> regq = parseQregisterVector(registerq);
    std::vector<std::string> regN = parseQregisterVector(registerN);
    std::vector<std::string> result = parseQregisterVector(registerResult);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas); 
    int m = regq.size();
    int n = regN.size();
    /////////////////////
    // need to check input sizes
    /////////////////////
    std::vector<std::string> modulus(result.begin(), result.end()); //(?)
    std::vector<std::string> quotient(ancillas.begin(), ancillas.begin() + n); //(?)  
    std::vector<std::string> ancillasDiv(ancillas.begin() + n, ancillas.end()); //(?)
    div_s(regq, regN, ancillasDiv, quotient, modulus);
    uncomputeQuotient_s(regq, regN, quotient, modulus, ancillasDiv);
}

// Computes quotient floor(q/N) for classical N (C++ input)
// Signed integers, LSB first
// Incomplete
void quotientval_s(std::vector<std::string> registerq, int N, std::vector<std::string> registerResult, std::vector<std::string> registerAncillas){
    std::vector<std::string> regq = parseQregisterVector(registerq);
    //std::vector<std::string> regN = parseQregisterVector(registerN);
    std::vector<std::string> result = parseQregisterVector(registerResult);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas); 
    int m = regq.size();
    //int n = regN.size();
    int n = std::ceil(std::log2(N)) + 1; // +1 for sign bit
    /////////////////////
    // need to check input sizes
    /////////////////////
    std::vector<std::string> quotient(result.begin(), result.end()); //(?)
    std::vector<std::string> modulus(ancillas.begin(), ancillas.begin() + n); //(?)
    std::vector<std::string> ancillasDiv(ancillas.begin() + n, ancillas.end()); //(?)
    
    divval_s(regq, N, ancillasDiv, quotient, modulus);
    uncomputeModulusval_s(regq, quotient, modulus, N, ancillasDiv);
}

// Computes modulus q mod N  for classical N (C++ input)
// Signed integers, LSB first
// Incomplete
void modulusval_s(std::vector<std::string> registerq, int N, std::vector<std::string> registerResult, std::vector<std::string> registerAncillas){
    std::vector<std::string> regq = parseQregisterVector(registerq);
    //std::vector<std::string> regN = parseQregisterVector(registerN);
    std::vector<std::string> result = parseQregisterVector(registerResult);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas); 
    int m = regq.size();
    //int n = regN.size();
    int n = std::ceil(std::log2(N)) + 1; // +1 for sign bit
    /////////////////////
    // need to check input sizes
    /////////////////////
    std::vector<std::string> modulus(result.begin(), result.end()); //(?)
    std::vector<std::string> quotient(ancillas.begin(), ancillas.begin() + n); //(?)  
    std::vector<std::string> ancillasDiv(ancillas.begin() + n, ancillas.end()); //(?)
    
    divval_s(regq, N, ancillasDiv, quotient, modulus);
    uncomputeQuotientval_s(regq, quotient, modulus, N, ancillasDiv);
}

