
#include "distribution.h"
#include "globals.h"
#include "simpleGates.h"
#include "toffoli.h"
#include "math.h"

// Algorithms for preparing quantum registers into various superposition states

// Prepare a quantum register into a uniform superposition over Z_N (integers mod N), where N is a classical value (C++ input)
void uniformval(std::vector<std::string> registerq, int N){
    std::vector<std::string> regq = parseQregisterVector(registerq);

    int n = ceil(log2(N));

    
}