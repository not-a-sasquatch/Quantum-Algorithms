#include <string>
#include <vector>

void toffoli(std::vector<std::string> controls, std::string target);

void toffoli_n(std::vector<std::string> controls, std::vector<std::string> ancilla, std::string target);

void toffoli_2n_3(std::vector<std::string> controls, std::vector<std::string> ancilla, std::string target);