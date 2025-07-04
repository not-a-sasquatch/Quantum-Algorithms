#include <string>
#include <vector>


void qphase(std::vector<std::string> registerPhase, std::string target, std::string ancilla, double scale);

void cqphase(std::vector<std::string> registerPhase, std::string target, std::vector<std::string> registerAncillas, std::string control, double scale);