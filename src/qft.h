#include <string>
#include <vector>

void qft(std::vector<std::string> registerq);

void cqft(std::vector<std::string> registerq, std::string control, std::string ancilla);

void qftApprox(std::vector<std::string> registerq, int k);

void cqftApprox(std::vector<std::string> registerq, std::string control, std::string ancilla, int k);

void qftn(std::vector<std::string> registerq, int N);

void cqftn(std::vector<std::string> registerq, int N, std::string control);