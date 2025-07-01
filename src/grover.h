#include <string>
#include <vector>
#include <functional>

void grover(std::vector<std::string> workRegisters, std::vector<std::string> readoutRegisters,  std::vector<std::string> ancillaRegisters, std::function<void(std::vector<std::string>, std::vector<std::string>)> oracle);

void diffusion(std::vector<std::string> workQubits, std::vector<std::string> ancillaQubits);

//void hiddenBitStringOracle(std::vector<std::string> ancilla, std::vector<int> bitString);