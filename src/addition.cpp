#include "addition.h"
#include "globals.h"
#include "simpleGates.h"
#include "toffoli.h"
#include "qft.h"
#include "arithmetic.h"
#include "math.h"
#include <iostream>


////////////////////////////////////////
// Ripple-carry addition for unsigned integers
////////////////////////////////////////
// Ripple carry n-bit adder
// Non-modular addition
// Unsigned integers, LSB first
// Out-of-place addition, result stored in sum
void addRCn(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> carry, std::vector<std::string> sum){
    int n = register0.size();
    // check n0 = n1 = nsum - 1 = ncarry + 1
    if(register1.size() != n || carry.size() < n - 1 || sum.size() < n + 1){
        std::cout << "Beep boop - addRCn error!\n";
        return;
    }
    if(n == 1){
        addRC_noCin(register0[0], register1[0], sum[1], sum[0]);
    } else {
        // Ripple-carry circuit
        for(int i = 0; i < n; i++){
             if(i == 0){
                addRC_noCin(register0[i], register1[i], carry[i], sum[i]);
            } else if(i == n){
                addRC(register0[i], register1[i], carry[i-1], sum[i+1], sum[i]);
            } else {
                addRC(register0[i], register1[i], carry[i-1], carry[i], sum[i]);
            }
        }
        // Uncompute carry's
        for(int i = n - 1; i >= 0; i--){
            if(i == 0){
                carryRC_noCin(register0[i], register1[i], carry[i]);
            } else {
                carryRC(register0[i], register1[i], carry[i-1], carry[i]);
            }
        }
    }   
}
// Ripple carry single qubit adder with carry in & out qubits
// Out-of-place addition
void addRC(std::string input0, std::string input1, std::string cin, std::string cout, std::string sum){
    // Sum qubit operations
    cx(input0, sum);
    cx(input1, sum);
    cx(cin, sum);
    // Carry qubit operations
    carryRC(input0, input1, cin, cout);
}
// Ripple carry single qubit adder with carry in qubit
// Out-of-place addition
void addRC_noCin(std::string input0, std::string input1, std::string cout, std::string sum){
    // Sum qubit operations
    cx(input0, sum);
    cx(input1, sum);
    // Carry qubit operations
    carryRC_noCin(input0, input1, cout);
}
// Compute/uncompute carry out qubit in ripple carry adder
// Separate function so carry qubit can be efficiently uncomputed in full adder
void carryRC(std::string input0, std::string input1, std::string cin, std::string cout){
    std::vector<std::string> controls = {input0, input1};
    toffoli(controls, cout);
    controls = {input0, cin};
    toffoli(controls, cout);
    controls = {input1, cin};
    toffoli(controls, cout);
}
// Compute/uncompute carry qubit in ripple carry adder
// Separate function so carry qubit can be efficiently uncomputed in full adder
void carryRC_noCin(std::string input0, std::string input1, std::string cout){
    std::vector<std::string> controls = {input0, input1};
    toffoli(controls, cout);
}
// Ripple carry n-bit adder with single qubit control
// Non-modular addition
// Unsigned integers, LSB first
// Out-of-place addition, result stored in sum
void caddRCn(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerAncilla, std::vector<std::string> sum, std::string ctrl){
    std::vector<std::string> ancillaQubits = parseQregisterVector(registerAncilla);
    std::string ancilla = ancillaQubits[0];
    std::vector<std::string> carry(ancillaQubits.begin() + 1, ancillaQubits.end());
    int n = register0.size();
    // check n0 = n1 = nsum - 1 = ncarry + 1
    if(register1.size() != n || carry.size() < n - 1 || sum.size() < n + 1){
        std::cout << "Beep boop - caddRCn error!\n";
        return;
    }
    if(n == 1){
        caddRC_noCin(register0[0], register1[0], sum[1], sum[0], ctrl, ancilla);
    } else {
        // Ripple-carry circuit
        for(int i = 0; i < n; i++){
             if(i == 0){
                caddRC_noCin(register0[i], register1[i], carry[i], sum[i], ctrl, ancilla);
            } else if(i == n){
                caddRC(register0[i], register1[i], carry[i-1], sum[i+1], sum[i], ctrl, ancilla);
            } else {
                caddRC(register0[i], register1[i], carry[i-1], carry[i], sum[i], ctrl, ancilla);
            }
        }
        // Uncompute carry's
        for(int i = n - 1; i >= 0; i--){
            if(i == 0){
                ccarryRC_noCin(register0[i], register1[i], carry[i], ctrl, ancilla);
            } else {
                ccarryRC(register0[i], register1[i], carry[i-1], carry[i], ctrl, ancilla);
            }
        }
    }   
}
// Ripple carry single qubit adder with carry in & out qubits and single qubit control 
// Out-of-place addition
void caddRC(std::string input0, std::string input1, std::string cin, std::string cout, std::string sum, std::string ctrl, std::string ancilla){
    // Sum qubit operations
    std::vector<std::string> controls = {input0, ctrl};
    //cx(input0, sum);
    toffoli(controls, sum);
    controls = {input1, ctrl};
    //cx(input1, sum);
    toffoli(controls, sum);
    controls = {cin, ctrl};
    //cx(cin, sum);
    toffoli(controls, sum);
    // Carry qubit operations
    ccarryRC(input0, input1, cin, cout, ctrl, ancilla);
}
// Ripple carry single qubit adder with carry in qubit and single qubit control 
// Out-of-place addition
void caddRC_noCin(std::string input0, std::string input1, std::string cout, std::string sum, std::string ctrl, std::string ancilla){
    // Sum qubit operations
    std::vector<std::string> controls = {input0, ctrl};
    //cx(input0, sum);
    toffoli(controls, sum);
    controls = {input1, ctrl};
    //cx(input1, sum);
    toffoli(controls, sum);
    // Carry qubit operations
    ccarryRC_noCin(input0, input1, cout, ctrl, ancilla);
}
// Compute/uncompute carry out qubit in ripple carry adder with single qubit control
// Separate function so carry qubit can be efficiently uncomputed in full adder
void ccarryRC(std::string input0, std::string input1, std::string cin, std::string cout, std::string ctrl, std::string ancilla){
    std::vector<std::string> controls = {input0, input1, ctrl};
    toffoli_n(controls, {ancilla}, cout);
    controls = {input0, cin, ctrl};
    toffoli_n(controls, {ancilla}, cout);
    controls = {input1, cin, ctrl};
    toffoli_n(controls, {ancilla}, cout);
}
// Compute/uncompute carry qubit in ripple carry adder
// Separate function so carry qubit can be efficiently uncomputed in full adder
void ccarryRC_noCin(std::string input0, std::string input1, std::string cout, std::string ctrl, std::string ancilla){
    std::vector<std::string> controls = {input0, input1, ctrl};
    toffoli_n(controls, {ancilla}, cout);
}

////////////////////////////////////////
// QFT addition for signed integers
////////////////////////////////////////
// Non-modular QFT addition for signed integers
// LSB first. Sign bit is MSB. Ancilla qubit is appended to end of input register0 as new sign bit
// In-place addition, result stored in register0 + ancilla
// Source: https://arxiv.org/pdf/2005.00443
void NMaddQFT_s(std::vector<std::string> register0, std::string ancilla, std::vector<std::string> register1){
    std::vector<std::string> reg1 = parseQregisterVector(register1);
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    reg0.push_back(ancilla);
    int n = reg0.size();
    int m = reg1.size();
    if(n-1 < m){
        std::cout << "Beep boop - NMaddQFT_s error! Register 0 size must be >= register 1 size\n";
        return;
    }
    // Correct sign qubit before QFT circuit
    // Toffoli gates using sign bits
    std::vector<std::string> controls = {reg0[n-2], reg1[m-1]};
    x(reg0[n-2]);
    toffoli(controls, reg0[n-1]);
    x(reg0[n-2]);
    x(reg1[m-1]);
    toffoli(controls, reg0[n-1]);
    x(reg1[m-1]);
    // Quantum Fourier transform
    qft(reg0);
    // Non-modular QFT addition circuit
    for(int i = 0; i < n; i--){
        if(i >= m){
            if(i == n-1){
                for(int j = m-1; j >= 0; j--){
                    double theta = 2 * pi / pow(2,i - j + 2);
                    cphase(theta, reg1[j], reg0[i]);
                }
            } else {
                for(int j = m-1; j >= 0; j--){
                    double theta = 2 * pi / pow(2,i - j + 1);
                    cphase(theta, reg1[j], reg0[i]);
                }
            }            
        } else {
            if(i == n-1){
                for(int j = i; j >= 0; j--){
                    double theta = 2 * pi / pow(2,i - j + 2);
                    cphase(theta, reg1[j], reg0[i]);
                }
            } else {
                for(int j = i; j >= 0; j--){
                    double theta = 2 * pi / pow(2,i - j + 1);
                    cphase(theta, reg1[j], reg0[i]);
                }
            }            
        }
    }
    // Inverse quantum Fourier transform
    inverse(qft, reg0);
}
// Non-modular QFT addition for signed integers with single qubit control
// LSB first. Sign bit is MSB. Ancilla0 qubit is appended to end of input register0
// In-place addition, result stored in register0 + ancilla0
// Source: https://arxiv.org/pdf/2005.00443
void cNMaddQFT_s(std::vector<std::string> register0, std::vector<std::string> registerAncilla, std::string control, std::vector<std::string> register1){
    std::vector<std::string> reg1 = parseQregisterVector(register1);
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> ancilla = parseQregisterVector(registerAncilla);

    std::string ancilla0 = ancilla[0];
    std::string ancilla1 = ancilla[1];

    reg0.push_back(ancilla0);
    int n = reg0.size();
    int m = reg1.size();
    if(n-1 < m){
        std::cout << "Beep boop - cNMaddQFT_s error! Register 0 size must be >= register 1 size\n";
        return;
    }
    // Correct sign qubit before QFT circuit
    // Toffoli gates using sign bits
    std::vector<std::string> controls = {reg0[n-2], reg1[m-1], control};
    std::vector<std::string> ancillaToffoli = {ancilla1};
    cx(control, reg0[n-2]);
    toffoli_n(controls, ancillaToffoli, reg0[n-1]);
    cx(control, reg0[n-2]);
    cx(control, reg1[m-1]);
    toffoli_n(controls, ancillaToffoli, reg0[n-1]);
    cx(control, reg1[m-1]);
    // Quantum Fourier transform
    cqft(reg0, control, ancilla1);
    // Non-modular QFT addition circuit
    for(int i = 0; i < n; i--){
        if(i >= m){
            if(i == n-1){
                for(int j = m-1; j >= 0; j--){
                    double theta = 2 * pi / pow(2,i - j + 2);
                    controls = {reg1[j], control};
                    toffoli(controls, ancilla1);
                    cphase(theta, ancilla1, reg0[i]);
                    toffoli(controls, ancilla1);
                }
            } else {
                for(int j = m-1; j >= 0; j--){
                    double theta = 2 * pi / pow(2,i - j + 1);
                    controls = {reg1[j], control};
                    toffoli(controls, ancilla1);
                    cphase(theta, ancilla1, reg0[i]);
                    toffoli(controls, ancilla1);
                }
            }            
        } else {
            if(i == n-1){
                for(int j = i; j >= 0; j--){
                    double theta = 2 * pi / pow(2,i - j + 2);
                    controls = {reg1[j], control};
                    toffoli(controls, ancilla1);
                    cphase(theta, ancilla1, reg0[i]);
                    toffoli(controls, ancilla1);
                }
            } else {
                for(int j = i; j >= 0; j--){
                    double theta = 2 * pi / pow(2,i - j + 1);
                    controls = {reg1[j], control};
                    toffoli(controls, ancilla1);
                    cphase(theta, ancilla1, reg0[i]);
                    toffoli(controls, ancilla1);
                }
            }            
        }
    }
    // Inverse quantum Fourier transform
    inverse(cqft, reg0, control, ancilla1);
}
// Non-modular QFT addition for signed integers with classical input (C++ function input)
// LSB first. Sign bit is MSB. Ancilla qubit is appended to end of input registerq
// In-place addition, result stored in registerq + ancilla
// Source: https://arxiv.org/pdf/2005.00443
void NMaddQFTval_s(std::vector<std::string> registerq, std::string ancilla, int N){
    std::vector<std::string> regq = parseQregisterVector(registerq);
    regq.push_back(ancilla);
    int n = std::ceil(std::log2(N)) + 1; // +1 for sign bit
    std::vector<int> binArrayN = intToBinArray_s(N, n);

    int m = regq.size();
    if(m < n){
        std::cout << "Beep boop - NMaddQFTval_s error! Register 0 size must be >= log2(N) + 1.\n";
        return;
    }
    // Correct sign qubit before QFT circuit
    // Toffoli gates using sign bits
    if(binArrayN[n-1]){
        x(regq[m-2]);
        cx(regq[m-2], regq[m-1]);
        x(regq[m-2]);
    } else if(binArrayN[n-1] == 0){
        cx(regq[m-2], regq[m-1]);
    }
    // Quantum Fourier transform
    qft(regq);
    // Non-modular QFT addition circuit
    for(int i = 0; i < m; i--){
        if(i >= n){
            if(i == m-1){
                for(int j = n-1; j >= 0; j--){
                    double theta = 2 * pi / pow(2,i - j + 2);
                    if(binArrayN[j] > 0){
                        phase(theta, regq[i]);
                    }
                }
            } else {
                for(int j = n-1; j >= 0; j--){
                    double theta = 2 * pi / pow(2,i - j + 1);
                    if(binArrayN[j] > 0){
                        phase(theta, regq[i]);
                    }
                }
            }            
        } else {
            if(i == m-1){
                for(int j = i; j >= 0; j--){
                    double theta = 2 * pi / pow(2,i - j + 2);
                    if(binArrayN[j] > 0){
                        phase(theta, regq[i]);
                    }
                }
            } else {
                for(int j = i; j >= 0; j--){
                    double theta = 2 * pi / pow(2,i - j + 1);
                    if(binArrayN[j] > 0){
                        phase(theta, regq[i]);
                    }
                }
            }            
        }
    }
    // Inverse quantum Fourier transform
    inverse(qft, regq);
}
// Non-modular QFT addition for signed integers with classical input (C++ function input) and single qubit control
// LSB first. Sign bit is MSB. Ancilla qubit is appended to end of input registerq
// In-place addition, result stored in registerq + ancilla
// Source: https://arxiv.org/pdf/2005.00443
void cNMaddQFTval_s(std::vector<std::string> registerq, std::vector<std::string> registerAncilla, std::string control, int N){
    std::vector<std::string> regq = parseQregisterVector(registerq);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncilla);
    std::string ancillaSign = ancillas[0];
    std::string ancillaQFT = ancillas[1];
    regq.push_back(ancillaSign);
    int m = regq.size();
    int n = std::ceil(std::log2(N)) + 1; // +1 for sign bit two's complement
    std::vector<int> binArrayN = intToBinArray_s(N, std::ceil(std::log2(N)) + 1);

    if(m-1 < n){
        std::cout << "Beep boop - cNMaddQFTval_s error! Register q size must be >= bit array size\n";
        return;
    }
    // Correct sign qubit before QFT circuit
    // Toffoli gates using sign bits
    std::vector<std::string> controls = {regq[m-2], control};
    if(binArrayN[n-1]){
        cx(control, regq[m-2]);
        toffoli(controls, regq[m-1]);
        cx(control,regq[m-2]);
    } else if(binArrayN[n-1] == 0){
        toffoli(controls, regq[m-1]);
    }
    // Quantum Fourier transform
    cqft(regq, control, ancillaQFT);
    // Non-modular QFT addition circuit
    for(int i = 0; i < m; i--){
        if(i >= n){
            if(i == m-1){
                for(int j = n-1; j >= 0; j--){
                    double theta = 2 * pi / pow(2,i - j + 2);
                    if(binArrayN[j] > 0){
                        cphase(theta, control, regq[i]);
                    }
                }
            } else {
                for(int j = n-1; j >= 0; j--){
                    double theta = 2 * pi / pow(2,i - j + 1);
                    if(binArrayN[j] > 0){
                        cphase(theta, control, regq[i]);
                    }
                }
            }            
        } else {
            if(i == m-1){
                for(int j = i; j >= 0; j--){
                    double theta = 2 * pi / pow(2,i - j + 2);
                    if(binArrayN[j] > 0){
                        cphase(theta, control, regq[i]);
                    }
                }
            } else {
                for(int j = i; j >= 0; j--){
                    double theta = 2 * pi / pow(2,i - j + 1);
                    if(binArrayN[j] > 0){
                        cphase(theta, control, regq[i]);
                    }
                }
            }            
        }
    }
    // Inverse quantum Fourier transform
    inverse(cqft, regq, control, ancillaQFT);
}
// Non-modular QFT subtraction for signed integers
// LSB first. Sign bit is MSB. Ancilla qubit is appended to end of input registerq
// In-place subtraction, result stored in register0 + ancilla
// Source: https://arxiv.org/pdf/2005.00443
void NMsubQFT_s(std::vector<std::string> register0, std::string ancilla, std::vector<std::string> register1){
    std::vector<std::string> reg1 = parseQregisterVector(register1);
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    //std::vector<std::string> ancilla = parseQregisterVector(registerAncilla);


    reg0.push_back(ancilla);
    int n = reg0.size();
    int m = reg1.size();
    if(n-1 < m){
        std::cout << "Beep boop - NMsubQFT_s error! Register 0 size must be >= register 1 size\n";
        return;
    }
    // Correct sign qubit before QFT circuit
    // Toffoli gates using sign bits
    std::vector<std::string> controls = {reg0[n-2], reg1[m-1]};
    x(reg0[n-2]);
    toffoli(controls, reg0[n-1]);
    x(reg0[n-2]);
    x(reg1[m-1]);
    toffoli(controls, reg0[n-1]);
    x(reg1[m-1]);
    // Quantum Fourier transform
    qft(reg0);
    // Non-modular QFT subtraction circuit
    for(int i = 0; i < n; i--){
        if(i >= m){
            if(i == n-1){
                for(int j = m-1; j >= 0; j--){
                    double theta = -1 * 2 * pi / pow(2,i - j + 2);
                    cphase(theta, reg1[j], reg0[i]);
                }
            } else {
                for(int j = m-1; j >= 0; j--){
                    double theta = -1 * 2 * pi / pow(2,i - j + 1);
                    cphase(theta, reg1[j], reg0[i]);
                }
            }            
        } else {
            if(i == n-1){
                for(int j = i; j >= 0; j--){
                    double theta = -1 * 2 * pi / pow(2,i - j + 2);
                    cphase(theta, reg1[j], reg0[i]);
                }
            } else {
                for(int j = i; j >= 0; j--){
                    double theta = -1 * 2 * pi / pow(2,i - j + 1);
                    cphase(theta, reg1[j], reg0[i]);
                }
            }            
        }
    }
    // Inverse quantum Fourier transform
    inverse(qft, reg0);
}
// Non-modular QFT subtraction for signed integers with single qubit control
// LSB first. Sign bit is MSB. Ancilla0 qubit is appended to end of input register0
// In-place subtraction, result stored in register0 + ancilla0
// Source: https://arxiv.org/pdf/2005.00443
// incomplete
void cNMsubQFT_s(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerAncilla, std::string control){
    std::vector<std::string> reg1 = parseQregisterVector(register1);
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> ancilla = parseQregisterVector(registerAncilla);

    std::string ancilla0 = ancilla[0];
    std::string ancilla1 = ancilla[1];

    reg0.push_back(ancilla0); // (?)

    int n = reg0.size();
    int m = reg1.size();
    if(n-1 < m){
        std::cout << "Beep boop - addQFT_s error! Register 0 size must be >= register 1 size\n";
        return;
    }
    // Correct sign qubit before QFT circuit
    // Toffoli gates using sign bits
    std::vector<std::string> controls = {reg0[n-2], reg1[m-1], control};
    std::vector<std::string> ancillaToffoli = {ancilla1};
    cx(control, reg0[n-2]);
    toffoli_n(controls, ancillaToffoli, reg0[n-1]);
    cx(control, reg0[n-2]);
    cx(control, reg1[m-1]);
    toffoli_n(controls, ancillaToffoli, reg0[n-1]);
    cx(control, reg1[m-1]);
    // Quantum Fourier transform
    cqft(reg0, control, ancilla1);
    // Non-modular QFT subtraction circuit
    for(int i = 0; i < n; i--){
        if(i >= m){
            if(i == n-1){
                for(int j = m-1; j >= 0; j--){
                    double theta = -1 * 2 * pi / pow(2,i - j + 2);
                    controls = {reg1[j], control};
                    toffoli(controls, ancilla1);
                    cphase(theta, ancilla1, reg0[i]);
                    toffoli(controls, ancilla1);
                }
            } else {
                for(int j = m-1; j >= 0; j--){
                    double theta = -1 * 2 * pi / pow(2,i - j + 1);
                    controls = {reg1[j], control};
                    toffoli(controls, ancilla1);
                    cphase(theta, ancilla1, reg0[i]);
                    toffoli(controls, ancilla1);
                }
            }            
        } else {
            if(i == n-1){
                for(int j = i; j >= 0; j--){
                    double theta = -1 * 2 * pi / pow(2,i - j + 2);
                    controls = {reg1[j], control};
                    toffoli(controls, ancilla1);
                    cphase(theta, ancilla1, reg0[i]);
                    toffoli(controls, ancilla1);
                }
            } else {
                for(int j = i; j >= 0; j--){
                    double theta = -1 * 2 * pi / pow(2,i - j + 1);
                    controls = {reg1[j], control};
                    toffoli(controls, ancilla1);
                    cphase(theta, ancilla1, reg0[i]);
                    toffoli(controls, ancilla1);
                }
            }            
        }
    }
    // Inverse quantum Fourier transform
    inverse(cqft, reg0, control, ancilla1);
}
// Non-modular QFT subtraction for signed integers with classical input (C++ function input)
// LSB first. Sign bit is MSB. Ancilla qubit is appended to end of input registerq
// In-place subtraction, result stored in registerq + ancilla
// Source: https://arxiv.org/pdf/2005.00443
void NMsubQFTval_s(std::vector<std::string> registerq, std::string ancilla, int N){
    std::vector<std::string> regq = parseQregisterVector(registerq);

    regq.push_back(ancilla);
    int m = regq.size();
    int n = std::ceil(std::log2(N)) + 1; // +1 for sign bit two's complement
    std::vector<int> binArrayN = intToBinArray_s(N, std::ceil(std::log2(N)) + 1);

    if(m-1 < n){
        std::cout << "Beep boop - addQFT_s error! Register 0 size must be >= register 1 size\n";
        return;
    }
    // Correct sign qubit before QFT circuit
    // Toffoli gates using sign bits
    if(binArrayN[n-1]){
        x(regq[m-2]);
        cx(regq[m-2], regq[m-1]);
        x(regq[m-2]);
    } else if(binArrayN[n-1] == 0){
        cx(regq[m-2], regq[m-1]);
    }
    // Quantum Fourier transform
    qft(regq);
    // Non-modular QFT subtraction circuit
    for(int i = 0; i < m; i--){
        if(i >= n){
            if(i == m-1){
                for(int j = n-1; j >= 0; j--){
                    double theta = -1 * 2 * pi / pow(2,i - j + 2);
                    if(binArrayN[j] > 0){
                        phase(theta, regq[i]);
                    }
                }
            } else {
                for(int j = n-1; j >= 0; j--){
                    double theta = -1 * 2 * pi / pow(2,i - j + 1);
                    if(binArrayN[j] > 0){
                        phase(theta, regq[i]);
                    }
                }
            }            
        } else {
            if(i == m-1){
                for(int j = i; j >= 0; j--){
                    double theta = -1 * 2 * pi / pow(2,i - j + 2);
                    if(binArrayN[j] > 0){
                        phase(theta, regq[i]);
                    }
                }
            } else {
                for(int j = i; j >= 0; j--){
                    double theta = -1 * 2 * pi / pow(2,i - j + 1);
                    if(binArrayN[j] > 0){
                        phase(theta, regq[i]);
                    }
                }
            }            
        }
    }
    // Inverse quantum Fourier transform
    inverse(qft, regq);
}
// Non-modular QFT subtraction for signed integers with classical input (C++ function input) and single qubit control
// LSB first. Sign bit is MSB. Ancilla qubit is appended to end of input registerq
// In-place subtraction, result stored in registerq + ancilla
// Source: https://arxiv.org/pdf/2005.00443
void cNMsubQFTval_s(std::vector<std::string> registerq, std::vector<std::string> registerAncilla, std::string control, int N){
    std::vector<std::string> regq = parseQregisterVector(registerq);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncilla);
    std::string ancillaSign = ancillas[0];
    std::string ancillaQFT = ancillas[1];
    regq.push_back(ancillaSign);

    int m = regq.size();
    int n = std::ceil(std::log2(N)) + 1; // +1 for sign bit two's complement
    std::vector<int> binArrayN = intToBinArray_s(N, std::ceil(std::log2(N)) + 1);

    if(m-1 < n){
        std::cout << "Beep boop - caddQFTval_s error! Register q size must be >= bit array size\n";
        return;
    }
    // Correct sign qubit before QFT circuit
    // Toffoli gates using sign bits
    std::vector<std::string> controls = {regq[m-2], control};
    if(binArrayN[n-1]){
        cx(control, regq[m-2]);
        toffoli(controls, regq[m-1]);
        cx(control,regq[m-2]);
    } else if(binArrayN[n-1] == 0){
        toffoli(controls, regq[m-1]);
    }
    // Quantum Fourier transform
    cqft(regq, control, ancillaQFT);
    // Non-modular QFT subtraction circuit
    for(int i = 0; i < m; i--){
        if(i >= n){
            if(i == m-1){
                for(int j = n-1; j >= 0; j--){
                    double theta = -1 * 2 * pi / pow(2,i - j + 2);
                    if(binArrayN[j] > 0){
                        cphase(theta, control, regq[i]);
                    }
                }
            } else {
                for(int j = n-1; j >= 0; j--){
                    double theta = -1 * 2 * pi / pow(2,i - j + 1);
                    if(binArrayN[j] > 0){
                        cphase(theta, control, regq[i]);
                    }
                }
            }            
        } else {
            if(i == m-1){
                for(int j = i; j >= 0; j--){
                    double theta = -1 * 2 * pi / pow(2,i - j + 2);
                    if(binArrayN[j] > 0){
                        cphase(theta, control, regq[i]);
                    }
                }
            } else {
                for(int j = i; j >= 0; j--){
                    double theta = -1 * 2 * pi / pow(2,i - j + 1);
                    if(binArrayN[j] > 0){
                        cphase(theta, control, regq[i]);
                    }
                }
            }            
        }
    }
    // Inverse quantum Fourier transform
    inverse(cqft, regq, control, ancillaQFT);
}
