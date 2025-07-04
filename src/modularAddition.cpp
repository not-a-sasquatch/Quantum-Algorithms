#include "modularAddition.h"
#include "globals.h"
#include "simpleGates.h"
#include "toffoli.h"
#include "qft.h"
#include "arithmetic.h"
#include "math.h"
#include <iostream>

////////////////////////////////////////
// QFT addition for unsigned integers
////////////////////////////////////////
// Quantum Fourier transform n-bit adder, modular 2^n
// Draper QFT adder
// Unsigned integers, LSB first
// In-place addition, result stored in register0
void addQFT(std::vector<std::string> register0, std::vector<std::string> register1){
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> reg1 = parseQregisterVector(register1);   
    int n = reg0.size();
    int m = reg1.size();
    // check input sizes
    if(n < m){
        std::cout << "Beep boop - addQFT error! Register 0 size must be >= register 1 size.\n";
        return;
    }
    // Quantum Fourier transform
    qft(reg0);
    // Evolve phases of Fourier transformed state
    for(int i = 0; i < n; i--){
        if(i >= m){
            for(int j = m-1; j >= 0; j--){
                double theta = pi / pow(2,i - j + 1);
                cphase(theta, reg1[j], reg0[i]);
            }
        } else {
            for(int j = i; j >= 0; j--){
                double theta = pi / pow(2,i - j + 1);
                cphase(theta, reg1[j], reg0[i]);
            }
        }
    }
    // Inverse quantum Fourier transform
    inverse(qft, reg0);
}
// Quantum Fourier transform n-bit adder, modular 2^n, with single qubit control
// Draper QFT adder
// Unsigned integers, LSB first
// In-place addition, result stored in register0
void caddQFT(std::vector<std::string> register0, std::vector<std::string> register1, std::string control, std::string ancilla){
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> reg1 = parseQregisterVector(register1);   
    int n = reg0.size();
    int m = reg1.size();
    // check input sizes
    if(n < m){
        std::cout << "Beep boop - caddQFT error! Register 0 size must be >= register 1 size.\n";
        return;
    }
    // Quantum Fourier transform
    cqft(reg0, control, ancilla);
    // Evolve phases of Fourier transformed state
    for(int i = 0; i < n; i--){
        if(i >= m){
            for(int j = m-1; j >= 0; j--){
                double theta = pi / pow(2,i - j + 1);
                std::vector<std::string> controls = {reg1[j], control};
                toffoli(controls, ancilla);
                cphase(theta, ancilla, reg0[i]);
                toffoli(controls, ancilla);
            }
        } else {
            for(int j = i; j >= 0; j--){
                double theta = pi / pow(2,i - j + 1);
                std::vector<std::string> controls = {reg1[j], control};
                toffoli(controls, ancilla);
                cphase(theta, ancilla, reg0[i]);
                toffoli(controls, ancilla);
            }
        }
    }
    // Inverse quantum Fourier transform
    inverse(cqft, reg0, control, ancilla);
}
// Quantum Fourier transform n-bit subtraction, modular 2^n
// Draper QFT adder with sign flip
// Unsigned integers, LSB first
// In-place subtraction, result stored in register0
void subQFT(std::vector<std::string> register0, std::vector<std::string> register1){
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> reg1 = parseQregisterVector(register1);   
    int n = reg0.size();
    int m = reg1.size();
    // check input sizes
    if(n < m){
        std::cout << "Beep boop - subQFT error! Register 0 size must be >= register 1 size.\n";
        return;
    }
    // Quantum Fourier transform
    qft(reg0);
    // Evolve phases of Fourier transformed state
    for(int i = 0; i < n; i--){
        if(i >= m){
            for(int j = m-1; j >= 0; j--){
                double theta = -1 * pi / pow(2,i - j + 1);
                cphase(theta, reg1[j], reg0[i]);
            }
        } else {
            for(int j = i; j >= 0; j--){
                double theta = -1 * pi / pow(2,i - j + 1);
                cphase(theta, reg1[j], reg0[i]);
            }
        }
    }
    // Inverse quantum Fourier transform
    inverse(qft, reg0);
}
// Quantum Fourier transform n-bit subtraction, modular 2^n, with single qubit control
// Draper QFT adder
// Unsigned integers, LSB first
// In-place subtraction, result stored in register0
void csubQFT(std::vector<std::string> register0, std::vector<std::string> register1, std::string control, std::string ancilla){
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> reg1 = parseQregisterVector(register1);   
    int n = reg0.size();
    int m = reg1.size();
    // check input sizes
    if(n < m){
        std::cout << "Beep boop - csubQFT error! Register 0 size must be >= register 1 size.\n";
        return;
    }
    // Quantum Fourier transform
    cqft(reg0, control, ancilla);
    // Evolve phases of Fourier transformed state
    for(int i = 0; i < n; i--){
        if(i >= m){
            for(int j = m-1; j >= 0; j--){
                double theta = -1 * pi / pow(2,i - j + 1);
                std::vector<std::string> controls = {reg1[j], control};
                toffoli(controls, ancilla);
                cphase(theta, ancilla, reg0[i]);
                toffoli(controls, ancilla);
            }
        } else {
            for(int j = i; j >= 0; j--){
                double theta = -1 * pi / pow(2,i - j + 1);
                std::vector<std::string> controls = {reg1[j], control};
                toffoli(controls, ancilla);
                cphase(theta, ancilla, reg0[i]);
                toffoli(controls, ancilla);
            }
        }
    }
    // Inverse quantum Fourier transform
    inverse(cqft, reg0, control, ancilla);
}
// Quantum Fourier transform n-bit adder with classical input (QASM bit), modular 2^n
// Draper QFT adder 
// Unsigned integers, LSB first
// In-place addition, result stored in registerq
void addQFTc(std::vector<std::string> registerq, std::vector<std::string> registerc){
    std::vector<std::string> regq = parseQregisterVector(registerq);
    std::vector<std::string> regc = parseCregisterVector(registerc);   
    int n = regq.size();
    int m = regc.size();
    if(n < m){
        std::cout << "Beep boop - addQFTc error! Register q size must be >= register c size.\n";
        return;
    }
    // Quantum Fourier transform
    qft(regq);
    // Evolve phases of Fourier transformed state
    for(int i = 0; i < n; i--){
        if(i >= m){
            for(int j = m-1; j >= 0; j--){
                double theta = pi / pow(2,i - j + 1);
                //if(regc[j] > 0){
                qasmIf(regc[j] + " == 1");
                    phase(theta, regq[i]);
                qasmEndIf();
                //}
            }
        } else {
            for(int j = i; j >= 0; j--){
                double theta = pi / pow(2,i - j + 1);
                //if(regc[j] > 0){
                qasmIf(regc[j] + " == 1");
                    phase(theta, regq[i]);
                qasmEndIf();
                //}
            }
        }
    }
    // Inverse quantum Fourier transform
    inverse(qft, regq);
}
// Quantum Fourier transform n-bit adder with classical input (C++ function input), modular 2^n
// Draper QFT adder
// Unsigned integers, LSB first
// In-place addition, result stored in registerq
void addQFTval(std::vector<std::string> registerq, int N){
    std::vector<std::string> regq = parseQregisterVector(registerq);  
    int m = regq.size();
    int n = std::ceil(std::log2(N));
    std::vector<int> binArrayN = intToBinArray_s(N, n);
    // check input sizes
    if(m < n){
        std::cout << "Beep boop - addQFTval error! register q size must be >= bitarray size.\n";
        return;
    }
    // Quantum Fourier transform
    qft(regq);
    // Evolve phases of Fourier transformed state
    for(int i = 0; i < m; i--){
        if(i >= n){
            for(int j = n-1; j >= 0; j--){
                double theta = pi / pow(2,i - j + 1);
                if(binArrayN[j] > 0){
                    phase(theta, regq[i]);
                }
            }
        } else {
            for(int j = i; j >= 0; j--){
                double theta = pi / pow(2,i - j + 1);
                if(binArrayN[j] > 0){
                    phase(theta, regq[i]);
                }
            }
        }
    }
    // Inverse quantum Fourier transform
    inverse(qft, regq);
}
// Quantum Fourier transform n-bit adder with classical input (C++ function input) and single qubit control
// Draper QFT adder
// Unsigned integers, LSB first
// In-place addition, result stored in registerq
void caddQFTval(std::vector<std::string> registerq, std::string control, std::string ancilla, int N){
    std::vector<std::string> regq = parseQregisterVector(registerq);  

    int m = regq.size();
    int n = std::ceil(std::log2(N));
    std::vector<int> binArrayN = intToBinArray_s(N, n);
    // check input sizes
    if(m < n){
        std::cout << "Beep boop - caddQFTval error! register q size must be >= bitarray size.\n";
        return;
    }
    // Quantum Fourier transform
    cqft(regq, control, ancilla);
    // Evolve phases of Fourier transformed state
    for(int i = 0; i < m; i--){
        if(i >= n){
            for(int j = n-1; j >= 0; j--){
                double theta = pi / pow(2,i - j + 1);
                if(binArrayN[j] > 0){
                    cphase(theta, control, regq[i]);
                }
            }
        } else {
            for(int j = i; j >= 0; j--){
                double theta = pi / pow(2,i - j + 1);
                if(binArrayN[j] > 0){
                    cphase(theta, control, regq[i]);
                }
            }
        }
    }
    // Inverse quantum Fourier transform
    inverse(cqft, regq, control, ancilla);
}

// Quantum Fourier transform n-bit subtraction with classical input (C++ function input), modular 2^n
// Draper QFT subtraction
// Unsigned integers, LSB first
// In-place addition, result stored in registerq
void subQFTval(std::vector<std::string> registerq, int N){
    std::vector<std::string> regq = parseQregisterVector(registerq);  
    int m = regq.size();
    int n = std::ceil(std::log2(N));
    std::vector<int> binArrayN = intToBinArray_s(N, n);
    // check input sizes
    if(m < n){
        std::cout << "Beep boop - addQFTval error! register q size must be >= bitarray size.\n";
        return;
    }
    // Quantum Fourier transform
    qft(regq);
    // Evolve phases of Fourier transformed state
    for(int i = 0; i < m; i--){
        if(i >= n){
            for(int j = n-1; j >= 0; j--){
                double theta = -1 * pi / pow(2,i - j + 1);
                if(binArrayN[j] > 0){
                    phase(theta, regq[i]);
                }
            }
        } else {
            for(int j = i; j >= 0; j--){
                double theta = -1 * pi / pow(2,i - j + 1);
                if(binArrayN[j] > 0){
                    phase(theta, regq[i]);
                }
            }
        }
    }
    // Inverse quantum Fourier transform
    inverse(qft, regq);
}

// Quantum Fourier transform n-bit subtraction with classical input (C++ function input) and single qubit control
// Draper QFT subtraction
// Unsigned integers, LSB first
// In-place addition, result stored in registerq
void csubQFTval(std::vector<std::string> registerq, std::string control, std::string ancilla, int N){
    std::vector<std::string> regq = parseQregisterVector(registerq);  

    int m = regq.size();
    int n = std::ceil(std::log2(N));
    std::vector<int> binArrayN = intToBinArray_s(N, n);
    // check input sizes
    if(m < n){
        std::cout << "Beep boop - caddQFTval error! register q size must be >= bitarray size.\n";
        return;
    }
    // Quantum Fourier transform
    cqft(regq, control, ancilla);
    // Evolve phases of Fourier transformed state
    for(int i = 0; i < m; i--){
        if(i >= n){
            for(int j = n-1; j >= 0; j--){
                double theta = -1 * pi / pow(2,i - j + 1);
                if(binArrayN[j] > 0){
                    cphase(theta, control, regq[i]);
                }
            }
        } else {
            for(int j = i; j >= 0; j--){
                double theta = -1 * pi / pow(2,i - j + 1);
                if(binArrayN[j] > 0){
                    cphase(theta, control, regq[i]);
                }
            }
        }
    }
    // Inverse quantum Fourier transform
    inverse(cqft, regq, control, ancilla);
}

////////////////////////////////////////
// QFT addition for signed integers
////////////////////////////////////////
// Quantum Fourier transform n-bit adder, modular 2^n
// Draper QFT adder
// Signed integers, LSB first
// In-place addition, result stored in register0
// Incomplete
void addQFT_s(std::vector<std::string> register0, std::vector<std::string> register1){
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> reg1 = parseQregisterVector(register1);   
    int n = reg0.size();
    int m = reg1.size();
    if(n < m){
        std::cout << "Beep boop - addQFT_s error! Register 0 size must be >= register 1 size.\n";
        return;
    }

    addQFT(reg0, reg1);
}

// Quantum Fourier transform n-bit addition, modular 2^n, with single qubit control
// Draper QFT addition
// Signed integers, LSB first
// In-place addittion, result stored in register0
// Incomplete
void caddQFT_s(std::vector<std::string> register0, std::vector<std::string> register1, std::string control, std::string ancilla){
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> reg1 = parseQregisterVector(register1);   
    int n = reg0.size();
    int m = reg1.size();
    if(n < m){
        std::cout << "Beep boop - addQFT_s error! Register 0 size must be >= register 1 size.\n";
        return;
    }

    caddQFT(reg0, reg1, control, ancilla);

}

// Quantum Fourier transform n-bit adder, modular 2^n, with classical input (C++ function input)
// Draper QFT adder
// Signed integers, LSB first
// In-place addition, result stored in register0
// Incomplete
void addQFTval_s(std::vector<std::string> registerq, int N){
    std::vector<std::string> regq = parseQregisterVector(registerq); 

    addQFTval(regq, N);
}

// Quantum Fourier transform n-bit adder, modular 2^n, with classical input (C++ function input) and single qubit control
// Draper QFT adder
// Signed integers, LSB first
// In-place addition, result stored in register0
// Incomplete
void caddQFTval_s(std::vector<std::string> registerq, std::string control, std::string ancilla, int N){
    std::vector<std::string> regq = parseQregisterVector(registerq); 

    caddQFTval(regq, control, ancilla, N);
}

// Quantum Fourier transform n-bit subtraction, modular 2^n
// Draper QFT subtraction
// Signed integers, LSB first
// In-place subtraction, result stored in register0
// Incomplete
void subQFT_s(std::vector<std::string> register0, std::vector<std::string> register1){
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> reg1 = parseQregisterVector(register1);   
    int n = reg0.size();
    int m = reg1.size();
    if(n < m){
        std::cout << "Beep boop - addQFT_s error! Register 0 size must be >= register 1 size.\n";
        return;
    }

    subQFT(reg0, reg1);

}

// Quantum Fourier transform n-bit subtraction, modular 2^n, with single qubit control
// Draper QFT subtraction
// Signed integers, LSB first
// In-place subtraction, result stored in register0
// Incomplete
void csubQFT_s(std::vector<std::string> register0, std::vector<std::string> register1, std::string control, std::string ancilla){
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> reg1 = parseQregisterVector(register1);   
    int n = reg0.size();
    int m = reg1.size();
    if(n < m){
        std::cout << "Beep boop - addQFT_s error! Register 0 size must be >= register 1 size.\n";
        return;
    }

    csubQFT(reg0, reg1, control, ancilla);

}

// Quantum Fourier transform n-bit subtraction, modular 2^n, with classical input (C++ function input)
// Draper QFT subtraction
// Signed integers, LSB first
// In-place subtraction, result stored in register0
// Incomplete
void subQFTval_s(std::vector<std::string> registerq, int N){
    std::vector<std::string> regq = parseQregisterVector(registerq);

    subQFTval(regq, N);

}

// Quantum Fourier transform n-bit subtraction with classical input (C++ function input) and single qubit control
// Draper QFT subtraction
// Unsigned integers, LSB first
// In-place subtraction, result stored in registerq
// incomplete
void csubQFTval_s(std::vector<std::string> registerq, std::string control, std::string ancilla, int N){
    std::vector<std::string> regq = parseQregisterVector(registerq);  

    csubQFTval(regq, control, ancilla, N);
}

////////////////////////////////////////
// Modular addition for signed integers
////////////////////////////////////////
// Modular addition of two quantum registers with quantum modulus 
// Signed integers, LSB first
// reg0 + reg1 mod regM, all inputs quantum
// Modified from https://arxiv.org/pdf/1801.01081
void addMod_s(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerM, std::vector<std::string> registerAncillas){
    std::vector<std::string> ancilla = parseQregisterVector(registerAncillas);
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> reg1 = parseQregisterVector(register1);
    std::vector<std::string> regM = parseQregisterVector(registerM);
    int n = reg0.size();
    // check input sizes

    std::string ancillaSign = ancilla[0];
    std::string ancillaAdd = ancilla[1];

    addQFT_s(reg0, reg1);
    // Trial subtraction of regM
    subQFT_s(reg0, regM);
    // Controlled re-addition of regM
    cx(reg0[n-1], ancillaSign);
    caddQFT_s(reg0, regM, ancillaSign, ancillaAdd);
    // Uncompute ancillaSign by subtracting then re-adding reg1 (this part assumes reg0, reg1 < regM so that (reg0 + reg1) - reg1 - regM < 0) 
    subQFT_s(reg0, reg1);
    cx(reg0[n-1], ancillaSign);
    addQFT_s(reg0, reg1);
}

// Modular addition of two quantum registers with classical modulus N (C++ input)
// Incomplete
void addModval_s(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerAncillas, int N){
    std::vector<std::string> ancilla = parseQregisterVector(registerAncillas);
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> reg1 = parseQregisterVector(register1);
    int n = reg0.size();
    // check input sizes

    std::string ancillaSign = ancilla[0];
    std::string ancillaAdd = ancilla[1];

    addQFT_s(reg0, reg1);
    // Trial subtraction of regM
    subQFTval_s(reg0, N);
    // Controlled re-addition of regM
    cx(reg0[n-1], ancillaSign);
    caddQFTval_s(reg0, ancillaSign, ancillaAdd, N);
    // Uncompute ancillaSign by subtracting then re-adding reg1 (this part assumes reg0, reg1 < N so that (reg0 + reg1) - reg1 - N < 0) 
    subQFT_s(reg0, reg1);
    cx(reg0[n-1], ancillaSign);
    addQFT_s(reg0, reg1);
}

// Modular addition of classical integer val (C++ input) into quantum register with classical modulus N (C++ inputs)
// Incomplete
void addModval2_s(std::vector<std::string> registerq, std::vector<std::string> registerAncillas, int val, int N){
    std::vector<std::string> ancilla = parseQregisterVector(registerAncillas);
    std::vector<std::string> regq = parseQregisterVector(registerq);
    int n = regq.size();
    // check input sizes

    std::string ancillaSign = ancilla[0];
    std::string ancillaAdd = ancilla[1];

    addQFTval_s(regq, val);
    // Trial subtraction of regM
    subQFTval_s(regq, N);
    // Controlled re-addition of regM
    cx(regq[n-1], ancillaSign);
    caddQFTval_s(regq, ancillaSign, ancillaAdd, N);
    // Uncompute ancillaSign by subtracting then re-adding reg1 (this part assumes regq, val < N so that (regq + val) - val - N < 0) 
    subQFTval_s(regq, val);
    cx(regq[n-1], ancillaSign);
    addQFTval_s(regq, val);
}

// Controlled modular addition into quantum register with quantum modulus 
// Incomplete
void caddMod_s(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerM, std::vector<std::string> registerAncillas, std::string control){
    std::vector<std::string> ancilla = parseQregisterVector(registerAncillas);
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> reg1 = parseQregisterVector(register1);
    std::vector<std::string> regM = parseQregisterVector(registerM);
    int n = reg0.size();
    // check input sizes

    std::string ancillaSign = ancilla[0];
    std::string ancillaAdd = ancilla[1];

    caddQFT_s(reg0, reg1, control, ancillaAdd);
    // Trial subtraction of regM
    csubQFT_s(reg0, regM, control, ancillaAdd);
    // Controlled re-addition of regM
    toffoli({control, reg0[n-1]}, ancillaSign);
    //cx(reg0[n-1], ancillaSign);
    caddQFT_s(reg0, regM, ancillaSign, ancillaAdd);
    // Uncompute ancillaSign by subtracting then re-adding reg1 (this part assumes reg0, reg1 < regM so that (reg0 + reg1) - reg1 - regM < 0) 
    csubQFT_s(reg0, reg1, control, ancillaAdd);
    toffoli({control, reg0[n-1]}, ancillaSign);
    //cx(reg0[n-1], ancillaSign);
    caddQFT_s(reg0, reg1, control, ancillaAdd);
}

// Controlled modular addition of classical integer val (C++ input) into quantum register with classical modulus N (C++ inputs)
// Incomplete
void caddModval_s(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerAncillas, int N, std::string control){
    std::vector<std::string> ancilla = parseQregisterVector(registerAncillas);
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> reg1 = parseQregisterVector(register1);
    int n = reg0.size();
    // check input sizes

    std::string ancillaSign = ancilla[0];
    std::string ancillaAdd = ancilla[1];

    caddQFT(reg0, reg1, control, ancillaAdd);
    // Trial subtraction of regM
    csubQFTval(reg0, control, ancillaAdd, N);
    // Controlled re-addition of regM
    toffoli({control, reg0[n-1]}, ancillaSign);
    //cx(reg0[n-1], ancillaSign);
    caddQFTval(reg0, ancillaSign, ancillaAdd, N);
    // Uncompute ancillaSign by subtracting then re-adding reg1 (this part assumes reg0, reg1 < N so that (reg0 + reg1) - reg1 - N < 0) 
    csubQFT(reg0, reg1, control, ancillaAdd);
    toffoli({control, reg0[n-1]}, ancillaSign);
    //cx(reg0[n-1], ancillaSign);
    caddQFT(reg0, reg1, control, ancillaAdd);
}

// Modular subtraction into quantum register with classical modulus N (C++ inputs)
// Incomplete
void subModval_s(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerAncillas, int N){
    std::vector<std::string> ancilla = parseQregisterVector(registerAncillas);
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> reg1 = parseQregisterVector(register1);
    int n = reg0.size();
    // check input sizes

    std::string ancillaSign = ancilla[0];
    std::string ancillaAdd = ancilla[1];

    subQFT_s(reg0, reg1);
    // Trial subtraction of regM
    subQFTval_s(reg0, N);
    // Controlled re-addition of regM
    cx(reg0[n-1], ancillaSign);
    caddQFTval_s(reg0, ancillaSign, ancillaAdd, N);
    // Uncompute ancillaSign by subtracting then re-adding reg1 (this part assumes reg0, reg1 < N so that (reg0 - reg1) + reg1 - N < 0) 
    addQFT_s(reg0, reg1);
    cx(reg0[n-1], ancillaSign);
    subQFT_s(reg0, reg1);
}

// Controlled modular subtraction of classical integer val (C++ input) into quantum register with classical modulus N (C++ inputs)
// Incomplete
void csubModval(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerAncillas, std::string control, int N){
    std::vector<std::string> ancilla = parseQregisterVector(registerAncillas);
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> reg1 = parseQregisterVector(register1);
    int n = reg0.size();
    // check input sizes

    std::string ancillaSign = ancilla[0];
    std::string ancillaAdd = ancilla[1];

    csubQFT(reg0, reg1, control, ancillaAdd);
    // Trial subtraction of regM
    csubQFTval(reg0, control, ancillaAdd, N);
    // Controlled re-addition of regM
    toffoli({control, reg0[n-1]}, ancillaSign);
    //cx(reg0[n-1], ancillaSign);
    caddQFTval(reg0, ancillaSign, ancillaAdd, N);
    // Uncompute ancillaSign by subtracting then re-adding reg1 (this part assumes reg0, reg1 < N so that (reg0 - reg1) + reg1 - N < 0) 
    caddQFT(reg0, reg1, control, ancillaAdd);
    toffoli({control, reg0[n-1]}, ancillaSign);
    //cx(reg0[n-1], ancillaSign);
    csubQFT(reg0, reg1, control, ancillaAdd);
}

