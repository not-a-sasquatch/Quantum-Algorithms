#include <string>
#include <vector>

void gcd(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerResult, std::vector<std::string> registerAncillas);

void modularInverse(std::vector<std::string> registerq, std::vector<std::string> registerN, std::vector<std::string> registerResult, std::vector<std::string> registerAncillas);

void modularInverseVal(std::vector<std::string> registerq, int N, std::vector<std::string> registerResult, std::vector<std::string> registerAncillas);

void extEuclideanCycle(std::vector<std::string> registerA0, std::vector<std::string> registerA1, std::vector<std::string> registerB0, std::vector<std::string> registerB1, std::string control, std::vector<std::string> registerAncillas);

void extEuclideanCycleFull(std::vector<std::string> registerA0, std::vector<std::string> registerA1, std::vector<std::string> registerB0, std::vector<std::string> registerB1, std::string control, std::vector<std::string> registerAncillas);
