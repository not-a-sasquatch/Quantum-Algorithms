#include <string>
#include <vector>
#include "arithmetic.h"


void modSARval(std::vector<std::string> registerq, std::vector<std::string> ancillaRegister, int N);

void modMulval_s(std::vector<std::string> register0, std::vector<std::string> register1, int N, std::vector<std::string> registerResult, std::vector<std::string> ancillaRegister);

void modMulval2_s(std::vector<std::string> registerq, int val, std::vector<std::string> registerN, std::vector<std::string> resultRegister, std::vector<std::string> ancillaRegister);

void modMulval3_s(std::vector<std::string> registerq, int val, int N, std::vector<std::string> resultRegister, std::vector<std::string> ancillaRegister);
