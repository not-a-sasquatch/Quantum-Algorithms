#include <vector>
#include <string>

void div_s(std::vector<std::string> registerq, std::vector<std::string> registerN, std::string registerAncilla, std::vector<std::string> registerQuotient, std::vector<std::string> registerModulus);

void divval_s(std::vector<std::string> registerq, int N, std::string ancilla, std::vector<std::string> registerDivisor, std::vector<std::string> registerModulus);

void divvalIP_s(std::vector<std::string> registerq, int N, std::string ancilla);

void uncomputeQuotientval_s(std::vector<std::string> registerq, std::vector<std::string> registerQuotient, std::vector<std::string> registerModulus, int N, std::vector<std::string> ancilla);

void uncomputeQuotient_s(std::vector<std::string> registerq, std::vector<std::string> registerN, std::vector<std::string> registerQuotient, std::vector<std::string> registerModulus, std::vector<std::string> ancillaRegister);

void uncomputeModulusval_s(std::vector<std::string> registerq, std::vector<std::string> registerQuotient, std::vector<std::string> registerModulus, int N, std::vector<std::string> ancillaRegister);

void uncomputeModulus_s(std::vector<std::string> registerq, std::vector<std::string> registerN, std::vector<std::string> registerQuotient, std::vector<std::string> registerModulus, std::vector<std::string> ancillaRegister);

void quotient_s(std::vector<std::string> registerq, std::vector<std::string> registerN, std::vector<std::string> registerResult, std::vector<std::string> ancillaRegister);

void modulus_s(std::vector<std::string> registerq, std::vector<std::string> registerN, std::vector<std::string> registerResult, std::vector<std::string> ancillaRegister);

void quotientval_s(std::vector<std::string> registerq, std::vector<std::string> registerN, std::vector<std::string> registerResult, std::vector<std::string> ancillaRegister);

void modulusval_s(std::vector<std::string> registerq, int N, std::vector<std::string> registerResult, std::vector<std::string> ancillaRegister);

