#include <string>
#include <vector>

void expMod(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerN, std::vector<std::string> registerResult, std::vector<std::string> registerAncilla, int cutoff);

void expModval2(std::vector<std::string> register0, std::vector<std::string> register1, int N, std::vector<std::string> registerResult, std::vector<std::string> registerAncilla);

void expModval(std::vector<std::string> registerq, int power, std::vector<std::string> registerN, std::vector<std::string> registerResult, std::vector<std::string> registerAncilla);

void expModval(std::vector<std::string> registerq, int power, int N, std::vector<std::string> registerResult, std::vector<std::string> registerAncilla);