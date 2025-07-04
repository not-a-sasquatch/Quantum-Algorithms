#include <vector>
#include <string>

void div_s(std::vector<std::string> registerq, std::vector<std::string> registerN, std::string registerAncilla, std::vector<std::string> registerQuotient, std::vector<std::string> registerModulus);

void cdiv_s(std::vector<std::string> registerq, std::vector<std::string> registerN, std::vector<std::string> registerAncillas, std::vector<std::string> registerQuotient, std::vector<std::string> registerModulus, std::string control);

void divval_s(std::vector<std::string> registerq, int N, std::vector<std::string> registerAncillas, std::vector<std::string> registerQuotient, std::vector<std::string> registerModulus);

void divvalIP_s(std::vector<std::string> registerq, int N, std::vector<std::string> registerAncillas);

void divcIP_s(std::vector<std::string> registerq, std::vector<std::string> registerc, std::vector<std::string> registerAncillas);

void uncomputeQuotientval_s(std::vector<std::string> registerq, std::vector<std::string> registerQuotient, std::vector<std::string> registerModulus, int N, std::vector<std::string> registerAncillas);

void uncomputeQuotient_s(std::vector<std::string> registerq, std::vector<std::string> registerN, std::vector<std::string> registerQuotient, std::vector<std::string> registerModulus, std::vector<std::string> registerAncillas);

void uncomputeModulusval_s(std::vector<std::string> registerq, std::vector<std::string> registerQuotient, std::vector<std::string> registerModulus, int N, std::vector<std::string> registerAncillas);

void uncomputeModulus_s(std::vector<std::string> registerq, std::vector<std::string> registerN, std::vector<std::string> registerQuotient, std::vector<std::string> registerModulus, std::vector<std::string> registerAncillas);

void cuncomputeModulus_s(std::vector<std::string> registerq, std::vector<std::string> registerN, std::vector<std::string> registerQuotient, std::vector<std::string> registerModulus, std::vector<std::string> registerAncillas, std::string control);

void quotient_s(std::vector<std::string> registerq, std::vector<std::string> registerN, std::vector<std::string> registerResult, std::vector<std::string> registerAncillas);

void cquotient_s(std::vector<std::string> registerq, std::vector<std::string> registerN, std::vector<std::string> registerResult, std::vector<std::string> registerAncillas, std::string control);

void modulus_s(std::vector<std::string> registerq, std::vector<std::string> registerN, std::vector<std::string> registerResult, std::vector<std::string> registerAncillas);

void quotientval_s(std::vector<std::string> registerq, int N, std::vector<std::string> registerResult, std::vector<std::string> registerAncillas);

void modulusval_s(std::vector<std::string> registerq, int N, std::vector<std::string> registerResult, std::vector<std::string> registerAncillas);

