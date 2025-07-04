#include <string>
#include <vector>

void mulMAC(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerAncillas, std::vector<std::string> registerResult);

void cmulMAC(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerAncillas, std::vector<std::string> registerResult, std::string control);

void mulMACval(std::vector<std::string> registerq, int N, std::vector<std::string> registerAncillas, std::vector<std::string> registerResult);

void mulIPMAC(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerAncillas);

void mulIPMACval(std::vector<std::string> registerq, int N, std::vector<std::string> registerAncillas);

int modInvPowval2(int N, int n);

void mulMAC_s(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerAncillas, std::vector<std::string> registerResult);

void cmulMAC_s(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerAncilla, std::vector<std::string> registerResult, std::string control);

void mulMACval_s(std::vector<std::string> registerq, int N, std::vector<std::string> registerAncillas, std::vector<std::string> registerResult);



