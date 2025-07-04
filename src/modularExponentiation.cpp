#include "modularExponentiation.h"
#include "globals.h"
#include "simpleGates.h"

// Quantum exponential value algorithm modulus a quantum number
// result = register0 ^ register1 modulus registerN
// Includes an upper cuttoff input which is the maximum possible value of the exponent (quantum halting problem?)
// Unsigned integers, LSB first
void expMod(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerN, std::vector<std::string> registerResult, std::vector<std::string> registerAncilla, int cutoff){

}



// Quantum exponential value algorithm modulus a classical number (C++ input)
// result = register0 ^ register1 modulus N
// Includes an upper cuttoff input which is the maximum possible value of the exponent (quantum halting problem?)
// Unsigned integers, LSB first
void expModval2(std::vector<std::string> register0, std::vector<std::string> register1, int N, std::vector<std::string> registerResult, std::vector<std::string> registerAncilla){

}

// Quantum exponential value algorithm with classical exponent (C++ input) modulus a quantum number
// result = registerq ^ power modulus registerN
// Unsigned integers, LSB first
void expModval(std::vector<std::string> registerq, int power, std::vector<std::string> registerN, std::vector<std::string> registerResult, std::vector<std::string> registerAncilla){

}

// Quantum exponential value algorithm with classical exponent (C++ input) modulus a classical number (C++ input)
// result = registerq ^ power modulus N
// Unsigned integers, LSB first
void expModval(std::vector<std::string> registerq, int power, int N, std::vector<std::string> registerResult, std::vector<std::string> registerAncilla){

}