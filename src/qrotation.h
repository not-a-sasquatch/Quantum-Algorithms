#include <string>
#include <vector>

void qrx(std::vector<std::string> registerPhase, std::string target, std::string ancilla, double scale);

void cqrx(std::vector<std::string> registerPhase, std::string target, std::vector<std::string> registerAncillas, std::string control, double scale);

void qry(std::vector<std::string> registerPhase, std::string target, std::string ancilla, double scale);

void cqry(std::vector<std::string> registerPhase, std::string target, std::vector<std::string> registerAncillas, std::string control, double scale);

void qrz(std::vector<std::string> registerPhase, std::string target, std::string ancilla, double scale);

void cqrz(std::vector<std::string> registerPhase, std::string target, std::vector<std::string> registerAncillas, std::string control, double scale);