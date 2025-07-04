#include "qphase.h"
#include "globals.h"
#include "simpleGates.h"

// Phase gate on a single qubit [[1,0];[0,e^(i*x*s)]], where x is an n-qubit signed quantum register [-2^(n-1), 2^(n-1)-1] and is scaled as 1/2^(n-1),
// and s is a double which can be used for scaling the input phase
// Signed integers, LSB first
void qphase(std::vector<std::string> registerPhase, std::string qubit, std::vector<std::string> registerAncilla, double scale){

}

// Phase gate on a single qubit [[1,0];[0,e^(i*x)]], where x is an n-qubit signed quantum register [-2^(n-1), 2^(n-1)-1] and is scaled as 1/2^(n-1),
// Signed integers, LSB first
// idk if this is necessary
void qphase(std::vector<std::string> registerPhase, std::string qubit, std::vector<std::string> registerAncilla){
    qphase(registerPhase, qubit, registerAncilla, 1.0);
}

// Controlled-phase gate on a single qubit [[1,0];[0,e^(i*x*s)]], where x is an n-qubit signed quantum register [-2^(n-1), 2^(n-1)-1] and is scaled as 1/2^(n-1),
// and s is a double which can be used for scaling the input phase, with a single qubit control
// Signed integers, LSB first
void cqphase(std::vector<std::string> registerPhase, std::string qubit, std::vector<std::string> registerAncilla, double scale, std::string control){

}

// Controlled-phase gate on a single qubit [[1,0];[0,e^(i*x)]], where x is an n-qubit signed quantum register [-2^(n-1), 2^(n-1)-1] and is scaled as 1/2^(n-1),
// with a single qubit control
// Signed integers, LSB first
// idk if this is necessary
void cqphase(std::vector<std::string> registerPhase, std::string qubit, std::vector<std::string> registerAncilla, std::string control){
    cqphase(registerPhase, qubit, registerAncilla, 1.0, control);
}