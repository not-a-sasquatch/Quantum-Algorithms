#include "globals.h"
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

// List of qubits with size
std::unordered_map<std::string, int> QregisterList;
// List of bits with size
std::unordered_map<std::string, int> CregisterList;
// List of string instructions
std::vector<std::string> algorithm;

void addCRegister(std::string name, int size){
    // check if register name exists already
    if((QregisterList.find(name) != QregisterList.end()) || (CregisterList.find(name) != CregisterList.end())){
        std::cout << "beep boop: register already exists\n";
    }
    CregisterList[name] = size;
}

void addQRegister(std::string name, int size){
    // check if register name exists already
    if((QregisterList.find(name) != QregisterList.end()) || (CregisterList.find(name) != CregisterList.end())){
        std::cout << "beep boop: register already exists\n";
    }
    QregisterList[name] = size;
}

void initAlgorithm(){
    std::string instruction = QASMVERSION;
    addInstruction(instruction);
}

void initRegister(std::string reg){
    if(QregisterList.find(reg) != QregisterList.end()){
        std::string instruction = "qubit";
        if(QregisterList[reg] > 1){
            instruction += "s[" + std::to_string(QregisterList[reg]) + "]";
        }
        instruction += " " + reg;
        addInstruction(instruction);
    } else if (CregisterList.find(reg) != CregisterList.end()){
        std::string instruction = "bit";
        if(CregisterList[reg] > 1){
            instruction += "s[" + std::to_string(CregisterList[reg]) + "]";
        }
        instruction += " " + reg;
        addInstruction(instruction);
    } else {
        std::cout << "beep boop: register doesn't exists\n";
    }
}

void initAllRegisters(){
    for (const auto& reg : QregisterList){
        std::string instruction = "qubit";
        if(reg.second > 1){
            instruction += "[" + std::to_string(reg.second) + "]";
        }
        instruction += " " + reg.first;
        addInstruction(instruction);
    }
    for (const auto& reg : CregisterList){
        std::string instruction = "bit";
        if(reg.second > 1){
            instruction += "[" + std::to_string(reg.second) + "]";
        }
        instruction += " " + reg.first;
        addInstruction(instruction);
    }
}

void resetRegister(std::string reg){
    if(auto search = QregisterList.find(reg); search != QregisterList.end()){
        std::string instruction = "reset " + reg + ";";
        addInstruction(instruction);
    } else {
        std::cout << "beep boop: register doesn't exists\n";
    }
}

void resetAllRegisters(){
    for (const auto& reg : QregisterList){
        std::string instruction = "reset " + reg.first + ";";
        addInstruction(instruction);
    }
}

void setCregister(std::string reg, int value){
    std::string instruction = reg + " = " + std::to_string(value) + ";";
    addInstruction(instruction);
}

void measureQubit(std::string Qreg, std::string Creg){
    std::string instruction = Creg + " = measure " + Qreg;
    addInstruction(instruction);
}

void measureRegister(std::vector<std::string> registerQuantum, std::vector<std::string> registerClassical){
    std::vector<std::string> regq = parseQregisterVector(registerQuantum);
    std::vector<std::string> regc = parseCregisterVector(registerClassical);
    int q = regq.size();
    int c = regc.size();
    if(q != c){
        std::cout << "beep boop - measureRegister error! register sizes don't match\n";
        return;
    }
    for(int i = 0; i < q; i++){
        std::string instruction = regc[i] + " = measure " + regq[i];
        addInstruction(instruction);
    }
    
}

void addInstruction(std::string instruction){
    algorithm.push_back(instruction + ";");
}

void addComment(std::string comment){
    algorithm.push_back("// " + comment);
}

std::vector<std::string> parseQregisterVector(std::vector<std::string> registers){
    std::vector<std::string> qubits;
    for(int i = 0; i < registers.size(); i++){
        if(QregisterList[registers[i]] > 1){
            for(int j = 0; j < QregisterList[registers[i]]; j++){
                qubits.push_back(registers[i] + "[" + std::to_string(j) + "]");
            }
        } else {
            qubits.push_back(registers[i]);
        }
    }
    return qubits;
}

std::vector<std::string> parseCregisterVector(std::vector<std::string> registers){
    std::vector<std::string> bits;
    for(int i = 0; i < registers.size(); i++){
        if(CregisterList[registers[i]] > 1){
            for(int j = 0; j < CregisterList[registers[i]]; j++){
                bits.push_back(registers[i] + "[" + std::to_string(j) + "]");
            }
        } else {
            bits.push_back(registers[i]);
        }
    }
    return bits;
}

void qasmIf(std::string condition){
    std::string instruction = "if (" + condition + ") {";
    addInstruction(instruction);
}

void qasmElse(){
    std::string instruction = "} else {";
    addInstruction(instruction);
}

void qasmFor(std::string condition){
    std::string instruction = "for (" + condition + ") {";
    addInstruction(instruction);
}

void qasmEndIf(){
    std::string instruction = "}";
    addInstruction(instruction);
}

void qasmEndFor(){
    std::string instruction = "}";
    addInstruction(instruction);
}

void printAlg(){
    for(int i = 0; i < algorithm.size(); i++){
        std::cout << algorithm[i];
        std::cout << "\n";
    }
}