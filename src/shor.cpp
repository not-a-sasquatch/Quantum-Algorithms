#include "shor.h"
#include "globals.h"
#include "simpleGates.h"
#include "toffoli.h"
#include "arithmetic.h"
#include "exponentiation.h"
#include "modularMultiplication.h"
#include "euclidean.h"
#include "distribution.h"
#include "qft.h"
#include <iostream>


void discreteLog(std::vector<std::string> registerx, std::vector<std::string> registerg, int N, std::vector<std::string> registerAncillas, std::vector<std::string> registerResult, std::vector<std::string> registerAncillaClassical){
    std::vector<std::string> regx = parseQregisterVector(registerx);
    std::vector<std::string> regg = parseQregisterVector(registerg);
    std::vector<std::string> result = parseQregisterVector(registerResult);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas); 
    std::vector<std::string> regc = parseCregisterVector(registerAncillaClassical);

    // check input sizes

    int n = regx.size();
    std::vector<std::string> reg0(ancillas.begin(), ancillas.begin() + n);
    std::vector<std::string> reg1(ancillas.begin() + n, ancillas.begin() + 2*n);
    std::vector<std::string> reg2(ancillas.begin() + 2*n, ancillas.begin() + 3*n);

    std::vector<std::string> reggInv;
    std::vector<std::string> regExpTemp0;
    std::vector<std::string> regExpTemp1;
    std::vector<std::string> reg1Inv;
    std::vector<std::string> regModMulTemp;

    std::vector<std::string> ancillaModInv;
    std::vector<std::string> ancillaExp;
    std::vector<std::string> ancillaModMul;
    int cutoff;

    // Prepare uniform superposition over Zn x Zn
    uniformval(reg0, N);
    uniformval(reg1, N);

    // Calculate g^-1
    modularInverseVal(regg, N, reggInv, ancillaModInv);

    // Calculate (1/N)*sum_a,b (a,b,f(a,b)) with f(a,b) = x^a * g^-b
    copy(regx, reg2);
    // Does this need to be modular exponentiation?
    // Calculate x^a
    exp(regx, reg0, regExpTemp0, ancillaExp, cutoff);
    // Calculate (g^-1)^b
    exp(reggInv, reg1, regExpTemp1, ancillaExp, cutoff);
    // Calculate f(a,b) = x^a * g^-b
    modMulval_s(regExpTemp0, regExpTemp1, N, reg2, ancillaModMul);
    // Uncompute x^a, (g^-1)^b temporary registers
    inverse(exp, regx, reg0, regExpTemp0, ancillaExp, cutoff);
    inverse(exp, reggInv, reg1, regExpTemp1, ancillaExp, cutoff);
    // Uncompute g^-1
    inverse(modularInverseVal, regg, N, reggInv, ancillaModInv);

    // Measure 3rd register
    measureRegister(reg2, regc);

    // Clear 3rd register and clear regc
    for(int i = 0; i < reg2.size(); i++){
        qasmIf(regc[i] + " == 1");
            x(reg2[i]);
            setCregister(regc[i], 0);
        qasmEndIf();
    }

    // Perform QFT over Zn x Zn
    qftn(reg0, N);
    qftn(reg1, N);

    // Perform cleanup division
    // Compute v^-1
    modularInverseVal(reg1, N, reg1Inv, ancillaModInv);
    // Compute v^-1 * (-v*log_g(x)) = -log_g(x)
    modMulval_s(reg0, reg1Inv, N, result, ancillaModMul);
    // Uncompute -v*log_g(x)
    inverse(modMulval_s, result, reg1, N, reg0, ancillaModMul);
    // Flip sign of result -log_g(x) -> log_g(x)
    x(result[result.size()-1]);
    std::vector<std::string> resultData(result.begin(), result.end()-1);
    QTC(resultData);
    // Or should this be log_g(x) = N - (-log_g(x)) mod N

    // Uncompute v^-1
    inverse(modularInverseVal, reg1, N, reg1Inv, ancillaModInv);

    // Measure 2nd register
    measureRegister(reg1, regc);

    // Clear 2nd register and clear regc
    for(int i = 0; i < reg1.size(); i++){
        qasmIf(regc[i] + " == 1");
            x(reg1[i]);
            setCregister(regc[i], 0);
        qasmEndIf();
    }
}