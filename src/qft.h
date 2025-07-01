#include <string>
#include <vector>

// Quantum Fourier transform
void qft(std::vector<std::string> registers);

void cqft(std::vector<std::string> registers, std::string control, std::string ancilla);

// Approximate Quantum Fourier transform to k-qubit precision
void qftApprox(std::vector<std::string> registers, int k);