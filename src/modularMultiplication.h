#include <string>
#include <vector>


void modSARval(std::vector<std::string> registerq, std::vector<std::string> registerAncillas, int N);

void modMul_s(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerN, std::vector<std::string> registerResult, std::vector<std::string> registerAncillas);

void modMulval_s(std::vector<std::string> register0, std::vector<std::string> register1, int N, std::vector<std::string> registerResult, std::vector<std::string> registerAncillas);

void modMulval2_s(std::vector<std::string> registerq, int val, std::vector<std::string> registerN, std::vector<std::string> registerResult, std::vector<std::string> registerAncillas);

void modMulval3_s(std::vector<std::string> registerq, int val, int N, std::vector<std::string> registerResult, std::vector<std::string> registerAncillas);
