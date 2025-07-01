#include "exponentiation.h"
#include "globals.h"
#include "simpleGates.h"
#include "arithmetic.h"
#include "toffoli.h"


// Quantum exponential value algorithm
// Includes an upper cuttoff input which is the maximum possible value of the exponent (quantum halting problem?)
// Unsigned integers, LSB first
void exp(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerResult, std::vector<std::string> registerAncilla, int cutoff){
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> reg1 = parseQregisterVector(register1);
    std::vector<std::string> result = parseQregisterVector(registerResult);
    std::vector<std::string> ancilla = parseQregisterVector(registerAncilla);
    int n = reg0.size();
    int m = reg1.size();

    std::string flag = ancilla[0];
    std::vector<std::string> garbage(ancilla.begin()+1, ancilla.begin()+n+m);
    std::vector<int> subArray = {1};
    for(int i = 1; i < m: i++){
        subArray.push_back(0);
    }
    
    // Copy register1 into ancillas
    std::vector<std::string> copy1(ancilla.begin()+n+m, ancilla.begin()+n+2*m);
    copy(reg1, copy1);
    // Initialize result as 1
    x(result[0]);
    // Flag is flipped when copy1 is zero
    for(int i = 0; i < m; i++){
        x(copy1[i]);
    }
    std::vector<std::string> ancillaToffoli(ancilla.begin()+1,ancilla.begin()+m-1);
    toffoli(copy,ancillaToffoli,flag);
    for(int i = 0; i < m; i++){
        x(copy1[i]);
    }
    std::vector<std::string> ancillaMult(ancilla.begin()+1,ancilla.begin()+m-1); (?)
    // Quantum multiplication loop, multiplies reg0 into result, subtracts 1 from copy1, repeats until copy1 equals 0.
    // Repeats up to cutoff (maximum possible exponent)
    for(int j = 0; j <= cutoff; j++){
        x(flag);
        cmulQFT(result, reg0, ancillaMult, flag);
        // Subtract 1 from copy1
        // need to implement this function still
        csubQFTval(copy1, subArray, flag);
        x(flag);
        // Flag is flipped when copy1 is zero
        for(int i = 0; i < m; i++){
            x(copy1[i]);
        }
        toffoli(copy,ancillaToffoli,flag);
        for(int i = 0; i < m; i++){
            x(copy1[i]);
        }
    }
}

// Quantum exponential value algorithm with classical exponent (C++ input)
// Unsigned integers, LSB first
void expval(std::vector<std::string> registerq, int power, std::vector<std::string> registerResult, std::vector<std::string> registerAncilla){
    std::vector<std::string> regq = parseQregisterVector(registerq);
    std::vector<std::string> result = parseQregisterVector(registerResult);
    std::vector<std::string> ancilla = parseQregisterVector(registerAncilla);
    int n = regq.size();
    int m = power;
    // check input sizes
    if(result.size() < n){
        std::cout << "Beep boop! expval error - result size must be >= regq size.\n";
    }
    // Initialize result as 1
    x(result[0]);
    std::vector<std::string> ancillaMult(ancilla.begin()+1,ancilla.begin()+m-1); (?)
    // Quantum multiplication loop, multiplies regq into result, repeats m times
    for(int j = 0; j < m; j++){
        std::vector<int> bitarray = intToBinArray(m, n);
        mulQFT(result, regq, ancillaMult);
    }
}