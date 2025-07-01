#include <string>
#include <vector>

void addQFT(std::vector<std::string> register0, std::vector<std::string> register1);

void caddQFT(std::vector<std::string> register0, std::vector<std::string> register1, std::string control, std:string ancilla);

void subQFT(std::vector<std::string> register0, std::vector<std::string> register1);

void csubQFT(std::vector<std::string> register0, std::vector<std::string> register1, std::string control, std:string ancilla);

void addQFTc(std::vector<std::string> registerq, std::vector<std::string> registerc);

void addQFTval(std::vector<std::string> registerq, int N);

void caddQFTval(std::vector<std::string> registerq, std::string control, int N);

void addQFT_s(std::vector<std::string> register0, std::vector<std::string> register1);

void addMod_s(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerM, std::vector<std::string> ancillaRegisters);

void addModval(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> ancilla, int N);

void addModval2(std::vector<std::string> registerq, std::vector<std::string> ancilla, int val, int N);

void caddModval(std::vector<std::string> registerq, std::vector<std::string> ancilla, std::string control, int val, int N);

void subModval(std::vector<std::string> registerq, std::vector<std::string> ancilla, int val, int N);

void csubModval(std::vector<std::string> registerq, std::vector<std::string> ancilla, std::string control, int val, int N);


