#include "qrotation.h"
#include "globals.h"
#include "simpleGates.h"


// Single-qubit rotation about X-axis [[cos(x*s/2), -isin(x*s/2)];[-isin(x*s/2), cos(x*s/2)]], where x is an n-qubit signed quantum register [-2^(n-1), 2^(n-1)-1] and is scaled as 1/2^(n-1),
// and s is a double which can be used for scaling the input phase
// Signed integers, LSB first
void qrx(std::vector<std::string> registerPhase, std::string qubit, std::vector<std::string> registerAncilla, double scale){

}

// Single-qubit rotation about X-axis [[cos(x/2), -isin(x/2)];[-isin(x/2), cos(x/2)]], where x is an n-qubit signed quantum register [-2^(n-1), 2^(n-1)-1] and is scaled as 1/2^(n-1)
// Signed integers, LSB first
void qrx(std::vector<std::string> registerPhase, std::string qubit, std::vector<std::string> registerAncilla){
    qrx(registerPhase, qubit, registerAncilla, 1.0);
}

// Single-qubit rotation about X-axis [[cos(x*s/2), -isin(x*s/2)];[-isin(x*s/2), cos(x*s/2)]], where x is an n-qubit signed quantum register [-2^(n-1), 2^(n-1)-1] and is scaled as 1/2^(n-1),
// and s is a double which can be used for scaling the input phase, with single qubit control
// Signed integers, LSB first
void cqrx(std::vector<std::string> registerPhase, std::string qubit, std::vector<std::string> registerAncilla, double scale, std::string control){

}

// Single-qubit rotation about Y-axis [[cos(x*s/2), -sin(x*s/2)];[sin(x*s/2), cos(x*s/2)]], where x is an n-qubit signed quantum register [-2^(n-1), 2^(n-1)-1] and is scaled as 1/2^(n-1),
// and s is a double which can be used for scaling the input phase
// Signed integers, LSB first
void qry(std::vector<std::string> registerPhase, std::string qubit, std::vector<std::string> registerAncilla, double scale){

}

// Single-qubit rotation about Y-axis [[cos(x*s/2), -sin(x*s/2)];[sin(x*s/2), cos(x*s/2)]], where x is an n-qubit signed quantum register [-2^(n-1), 2^(n-1)-1] and is scaled as 1/2^(n-1),
// and s is a double which can be used for scaling the input phase, with single qubit control
// Signed integers, LSB first
void cqry(std::vector<std::string> registerPhase, std::string qubit, std::vector<std::string> registerAncilla, double scale, std::string control){

}

// Single-qubit rotation about z-axis [[e^(-ix*s/2), 0];[0, e^(ix*s/2)]], where x is an n-qubit signed quantum register [-2^(n-1), 2^(n-1)-1] and is scaled as 1/2^(n-1),
// and s is a double which can be used for scaling the input phase
// Signed integers, LSB first
void qrz(std::vector<std::string> registerPhase, std::string qubit, std::vector<std::string> registerAncilla, double scale, std::string control){

}

// Single-qubit rotation about z-axis [[e^(-ix*s/2), 0];[0, e^(ix*s/2)]], where x is an n-qubit signed quantum register [-2^(n-1), 2^(n-1)-1] and is scaled as 1/2^(n-1),
// and s is a double which can be used for scaling the input phase, with single qubit control
// Signed integers, LSB first
void cqrz(std::vector<std::string> registerPhase, std::string qubit, std::vector<std::string> registerAncilla, double scale, std::string control){

}