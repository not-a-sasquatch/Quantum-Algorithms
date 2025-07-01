#include "toffoli.h"
#include "globals.h"
#include "simpleGates.h"
#include <iostream>

// Toffoli gate with 2-qubit control
void toffoli(std::vector<std::string> controls, std::string target){
    std::vector<std::string> controlQubits = parseQregisterVector(controls);
    std::string control0 = controlQubits[0];
    std::string control1 = controlQubits[1];
    h(target);
    cx(control1, target);
    inverse(t, target);
    cx(control0, target);
    t(target);
    cx(control1, target);
    inverse(t, target);
    cx(control0, target);
    t(target);
    h(target);
    t(control1);
    cx(control0, control1);
    t(control0);
    inverse(t, control1);
    cx(control0, control1);
}

// Toffoli gate with n-qubit control
void toffoli_n(std::vector<std::string> controls, std::vector<std::string> ancilla, std::string target){
    toffoli_2n_3(controls, ancilla, target);
}

// Implementation of Toffoli gate with n-qubit control
// Uses 2n-3 (2-qubit) Toffoli gates and n-2 ancilla qubits
void toffoli_2n_3(std::vector<std::string> controls, std::vector<std::string> ancilla, std::string target){
    std::vector<std::string> controlQubits = parseQregisterVector(controls);
    std::vector<std::string> ancillaQubits = parseQregisterVector(ancilla);
    if(ancillaQubits.size() > controlQubits.size() - 2){
        std::cout << "Beep boop! # ancilla qubits required is (at least) n-2 for n control qubits.\n";
        return;// void;
    }
    int n = controlQubits.size();
    std::vector<std::string> toffoli2controls;
    for(int i = 0; i < n-1; i++){
        if(i == 0){
            toffoli2controls.push_back(controlQubits[0]);
            toffoli2controls.push_back(controlQubits[1]);
        } else {
            toffoli2controls.push_back(controlQubits[i+1]);
            toffoli2controls.push_back(ancillaQubits[i-1]);
        }
        toffoli(toffoli2controls, ancillaQubits[i]);
        toffoli2controls.clear();
    }
    toffoli2controls.push_back(controlQubits[n-1]);
    toffoli2controls.push_back(ancillaQubits[n-3]);
    toffoli(toffoli2controls, target);
    toffoli2controls.clear();
    for(int i = n-2; i >= 0; i--){
        if(i == 0){
            toffoli2controls.push_back(controlQubits[0]);
            toffoli2controls.push_back(controlQubits[1]);
        } else {
            toffoli2controls.push_back(controlQubits[i+1]);
            toffoli2controls.push_back(ancillaQubits[i-1]);
        }
        toffoli(toffoli2controls, ancillaQubits[i]);
        toffoli2controls.clear();
    }


}