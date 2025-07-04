#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

#define QASMVERSION "OPENQASM 3"

#define pi           3.14159265358979323846  /* pi */

extern std::vector<std::string> algorithm;

void addQRegister(std::string name, int size);

void addCRegister(std::string name, int size);

void addInstruction(std::string instruction);

void addComment(std::string comment);

void initAlgorithm();

void initRegister(std::string reg); 

void initAllRegisters();

void resetRegister(std::string reg);

void resetAllRegisters();

void setCregister(std::string reg, int value);

void measureQubit(std::string Qreg, std::string Creg);

void measureRegister(std::vector<std::string> registerQuantum, std::vector<std::string> registerClassical);

std::vector<std::string> parseQregisterVector(std::vector<std::string> registers);

std::vector<std::string> parseCregisterVector(std::vector<std::string> registers);

void qasmIf(std::string condition);

void qasmElse();

void qasmFor(std::string condition);

void qasmEndIf();

void qasmEndFor();

void printAlg();

template <class Function, class ... Args>
void inverse(Function func, Args...args){
    // Run the function, keep track of added instructions
    int index0 = algorithm.size();
    func(args...);
    int index1 = algorithm.size();
    std::vector<std::string> instructions;
    // Iterate through added instructions, convert to their inverses
    for(int i = 0; i < index1 - index0; i++){
        std::string instr = algorithm.back();
        algorithm.erase(algorithm.end() - 1);
        if(instr.find("U(") != std::string::npos){
            // U or CU
            int pos1 = instr.find("(");
            if(instr[pos1+1] == '-'){
                instr.erase(pos1+1, 1);
            } else {
                instr.insert(pos1+1, 1, '-');
            }
            int pos2 = instr.find(",", pos1 + 1);
            int pos3 = instr.find(",", pos2 + 1);
            int pos4 = instr.find(")", pos3 + 1);
            double phi = std::stod(instr.substr(pos2+2, pos3 - pos2 - 2));
            double lambda = std::stod(instr.substr(pos3+2, pos4 - pos3 - 2));
            instr.erase(pos3+2, pos4 - pos3 - 2);
            instr.erase(pos2+2, pos3 - pos2 - 2);
            instr.insert(pos2 + 2, std::to_string(-1*lambda));
            pos3 = instr.find(",", pos2 + 1);
            instr.insert(pos3 + 2, std::to_string(-1*phi));
        } else {
            // gphase
            if(instr[7] == '-'){
                instr.erase(7, 1);
            } else {
                instr.insert(7, 1, '-');
            }
        }
        instructions.push_back(instr);
    }
    // Add inverted instructions back in reverse order
    for(int i = 0; i < instructions.size(); i++){
        algorithm.push_back(instructions[i]);
    }
}


