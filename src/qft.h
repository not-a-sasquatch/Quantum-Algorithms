#include <string>
#include <vector>

void qft(std::vector<std::string> registers);

void cqft(std::vector<std::string> registers, std::string control, std::string ancilla);

void qftApprox(std::vector<std::string> registers, int k);

void qftn(std::vector<std::string> registers, int N);