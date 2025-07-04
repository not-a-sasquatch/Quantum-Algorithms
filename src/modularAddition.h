#include <string>
#include <vector>

void addQFT(std::vector<std::string> register0, std::vector<std::string> register1);

void caddQFT(std::vector<std::string> register0, std::vector<std::string> register1, std::string control, std::string ancilla);

void subQFT(std::vector<std::string> register0, std::vector<std::string> register1);

void csubQFT(std::vector<std::string> register0, std::vector<std::string> register1, std::string control, std::string ancilla);

void addQFTc(std::vector<std::string> registerq, std::vector<std::string> registerc);

void addQFTval(std::vector<std::string> registerq, int N);

void caddQFTval(std::vector<std::string> registerq, std::string control, std::string ancilla, int N);

void subQFTval(std::vector<std::string> registerq, int N);

void csubQFTval(std::vector<std::string> registerq, std::string control, std::string ancilla, int N);

void addQFT_s(std::vector<std::string> register0, std::vector<std::string> register1);

void caddQFT_s(std::vector<std::string> register0, std::vector<std::string> register1, std::string control, std::string ancilla);

void addQFTval_s(std::vector<std::string> registerq, int N);

void caddQFTval_s(std::vector<std::string> registerq, std::string control, std::string ancilla, int N);

void subQFT_s(std::vector<std::string> register0, std::vector<std::string> register1);

void csubQFT_s(std::vector<std::string> register0, std::vector<std::string> register1, std::string control, std::string ancilla);

void subQFTval_s(std::vector<std::string> registerq, int N);

void csubQFTval_s(std::vector<std::string> registerq, std::string control, std::string ancilla, int N);

void addMod_s(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerM, std::vector<std::string> registerAncillas);

void addModval_s(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerAncillas, int N);

void addModval2_s(std::vector<std::string> registerq, std::vector<std::string> registerAncillas, int val, int N);

void caddMod_s(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerM, std::vector<std::string> registerAncillas, std::string control);

void caddModval_s(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerAncillas, int N, std::string control);

void subModval_s(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerAncillas, int N);

void csubModval(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerAncillas, std::string control, int N);

