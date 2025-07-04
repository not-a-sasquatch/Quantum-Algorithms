#include "qphase.h"
#include "globals.h"
#include "simpleGates.h"
#include "arithmetic.h"
#include "toffoli.h"
#include "math.h"

// Phase gate on a single qubit [[1,0];[0,e^(i*x*s)]], where x is an n-qubit signed quantum register [-2^(n-1), 2^(n-1)-1] and is scaled as 1/2^(n-1),
// and s is a double which can be used for scaling the input phase
// Signed integers, LSB first
void qphase(std::vector<std::string> registerPhase, std::string target, std::string ancilla, double scale){
    std::vector<std::string> regPhase = parseQregisterVector(registerPhase);
    
    // check input size

    int n = registerPhase.size();

    // Take twos complement of phase if necessary
    std::vector<std::string> phaseData(regPhase.begin(), regPhase.end() - 1);
    cQTC(phaseData, regPhase[n-1], ancilla);

    // If phase < 0
    for(int i = 0; i < n-1; i++){
        double theta = (-1 / pow(2, n-1-i)) * scale;
        std::vector<std::string> controls = {regPhase[n-1], phaseData[i]};
        toffoli(controls, ancilla);
        cphase(theta, ancilla, target);
        toffoli(controls, ancilla);
    }

    // If phase >= 0
    x(regPhase[n-1]);
    for(int i = 0; i < n-1; i++){
        double theta = (1 / pow(2, n-1-i)) * scale;
        std::vector<std::string> controls = {regPhase[n-1], phaseData[i]};
        toffoli(controls, ancilla);
        cphase(theta, ancilla, target);
        toffoli(controls, ancilla);
    }
    x(regPhase[n-1]);

    // Uncompute two's complement
    cQTC(phaseData, regPhase[n-1], ancilla);
}

// Controlled-phase gate on a single qubit [[1,0];[0,e^(i*x*s)]], where x is an n-qubit signed quantum register [-2^(n-1), 2^(n-1)-1] and is scaled as 1/2^(n-1),
// and s is a double which can be used for scaling the input phase, with a single qubit control
// Signed integers, LSB first
void cqphase(std::vector<std::string> registerPhase, std::string target, std::vector<std::string> registerAncillas, std::string control, double scale){
    std::vector<std::string> regPhase = parseQregisterVector(registerPhase);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas);

    // check input size

    int n = registerPhase.size();

    std::string ancillaToff = ancillas[0];
    std::string ancillaControl = ancillas[0];
    std::string ancillaQTC = ancillas[0];

    // Take twos complement of phase if necessary
    std::vector<std::string> phaseData(regPhase.begin(), regPhase.end() - 1);
    cQTC(phaseData, regPhase[n-1], ancillaQTC);

    // If phase < 0
    for(int i = 0; i < n-1; i++){
        double theta = (-1 / pow(2, n-1-i)) * scale;
        std::vector<std::string> controls = {regPhase[n-1], phaseData[i], control};
        toffoli_n(controls, {ancillaToff}, ancillaControl);
        cphase(theta, ancillaControl, target);
        toffoli_n(controls, {ancillaToff}, ancillaControl);
    }

    // If phase >= 0
    x(regPhase[n-1]);
    for(int i = 0; i < n-1; i++){
        double theta = (1 / pow(2, n-1-i)) * scale;
        std::vector<std::string> controls = {regPhase[n-1], phaseData[i], control};
        toffoli_n(controls, {ancillaToff}, ancillaControl);
        cphase(theta, ancillaControl, target);
        toffoli_n(controls, {ancillaToff}, ancillaControl);
    }
    x(regPhase[n-1]);

    // Uncompute two's complement
    cQTC(phaseData, regPhase[n-1], ancillaQTC);
}
