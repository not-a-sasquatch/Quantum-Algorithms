#include <string>
#include <vector>

void addRCn(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> carry, std::vector<std::string> sum);

void addRC(std::string input0, std::string input1, std::string cin, std::string cout, std::string sum);

void addRC_noCin(std::string input0, std::string input1, std::string cout, std::string sum);

void carryRC(std::string input0, std::string input1, std::string cin, std::string cout);

void carryRC_noCin(std::string input0, std::string input1, std::string cout);

void caddRCn(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> ancillaRegisters, std::vector<std::string> sum, std::string ctrl);

void caddRC(std::string input0, std::string input1, std::string cin, std::string cout, std::string sum, std::string ctrl, std::string ancilla);

void caddRC_noCin(std::string input0, std::string input1, std::string cout, std::string sum, std::string ctrl, std::string ancilla);

void ccarryRC(std::string input0, std::string input1, std::string cin, std::string cout, std::string ctrl, std::string ancilla);

void ccarryRC_noCin(std::string input0, std::string input1, std::string cout, std::string ctrl, std::string ancilla);

void NMaddQFT_s(std::vector<std::string> register0, std::string ancilla, std::vector<std::string> register1);

void cNMaddQFT_s(std::vector<std::string> register0, std::string ancilla0, std::string ancilla1, std::string control, std::vector<std::string> register1);

void NMaddQFTval_s(std::vector<std::string> registerq, std::string ancilla, int N);

void cNMaddQFTval_s(std::vector<std::string> registerq, std::string registerAncilla, std::string control, int N);

void NMsubQFT_s(std::vector<std::string> register0, std::string registerAncilla, std::vector<std::string> register1);

void cNMsubQFT_s(std::vector<std::string> register0, std::vector<std::string> registerAncilla, std::string control, std::vector<std::string> register1);

void NMsubQFTval_s(std::vector<std::string> registerq, std::string ancilla, int N);

void cNMsubQFTval_s(std::vector<std::string> registerq, std::string ancilla, std::string control, int N);