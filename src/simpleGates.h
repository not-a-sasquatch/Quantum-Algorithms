#include <string>
#include <vector>

//# define pi           3.14159265358979323846  /* pi */

// Built-in functions/primitive instructions
void u(double theta, double phi, double lambda, std::string qubit);

void cu(double theta, double phi, double lambda, std::string control, std::string target);

void gphase(double theta);

// Hadamard gate on a single qubit
void h(std::string qubit);

// Hadamard gate on a single qubit with single qubit control
void ch(std::string control, std::string target);

// X gate on a single qubit
void x(std::string qubit);

// Controlled X gate on a single qubit with single qubit control
void cx(std::string control, std::string target);

// Y gate on a single qubit
void y(std::string qubit);

// Controlled Y gate on a single qubit with single qubit control
void cy(std::string control, std::string target);

// Z gate on a single qubit
void z(std::string qubit);

// Controlled Z gate on a single qubit with single qubit control
void cz(std::string control, std::string target);

// Single-qubit rotation about x-axis
void rx(double theta, std::string qubit);

// Controlled-rx gate on a single qubit with single qubit control
void crx(double theta, std::string control, std::string target);

// Single-qubit rotation about y-axis
void ry(double theta, std::string qubit);

// Controlled-ry gate on a single qubit with single qubit control
void cry(double theta, std::string control, std::string target);

// Single-qubit rotation about z-axis
void rz(double theta, std::string qubit);

// Controlled-rz gate on a single qubit with single qubit control
void crz(double theta, std::string control, std::string target);

// Swap gate between two qubits
void swap(std::string qubit1, std::string qubit2);

// Swap gate between two registers
void swapn(std::vector<std::string> register0, std::vector<std::string> register1);

// Swap gate between two qubits with a single qubit control
void cswap(std::string control, std::string target1, std::string target2);

// Swap gate between two registers with a single qubit control
void cswapn(std::string control, std::vector<std::string> register0, std::vector<std::string> register1);

// Phase gate on a single qubit
void phase(double theta, std::string qubit);

// Phase gate with a single qubit control
void cphase(double theta, std::string control, std::string target);

// T gate on a single qubit
void t(std::string qubit);

// s gate on a single qubit
void s(std::string qubit);

// Copy qubit register into register of 0-initialized qubit's
void copy(std::vector<std::string> registerq, std::vector<std::string> ancillas);

