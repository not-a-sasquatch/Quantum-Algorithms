#include <string>
#include <vector>
#include "addition.h"
#include "multiplication.h"
#include "division.h"
#include "exponentiation.h"
#include "modularAddition.h"
#include "modularMultiplication.h"
#include "modularExponentiation.h"

void QTC(std::vector<std::string> registerq);

void cQTC(std::vector<std::string> registerq, std::string control, std::string ancilla);

void CTC(std::vector<int> binArray);

std::vector<int> intToBinArray(int value, int depth);

std::vector<int> intToBinArray_s(int value, int depth);