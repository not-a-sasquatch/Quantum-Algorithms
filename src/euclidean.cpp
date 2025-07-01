
// Greatest common denominator between two quantum registers based on the extended Euclidean algorithm
// Signed integers, LSB first
// https://arxiv.org/abs/quant-ph/0301141
void gcd(std::vector<std::string> register0, std::vector<std::string> register1, std::vector<std::string> registerResult, std::vector<std::string> registerAncillas){
    std::vector<std::string> reg0 = parseQregisterVector(register0);
    std::vector<std::string> reg1 = parseQregisterVector(register1);
    std::vector<std::string> result = parseQregisterVector(registerResult);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas);
    
    std::string aLessB = ancillas[0];
    std::string aGreatB = ancillas[1];
    std::string aEqualB = ancillas[2];
    std::string control = ancillas[3];

    std::vector<std::string> compAncillas;
    std::vector<std::string> toffAncillas;
    std::vector<std::string> euclideanAncillas;
    std::vector<std::string> addAncillas;
    
    std::vector<std::string> regA0;
    std::vector<std::string> regA1;
    std::vector<std::string> regB0;
    std::vector<std::string> regB1;
    // check input size

    // Max number of iterations
    // Verify max cutoff = 4.5*n
    int cutoff = std::ceil(4.5*registerN.size()); 

    // Initialize registers (A0,A1),(B0,B2) = (0,reg0),(1,reg1)
    for(int i = 0; i < regN.size(); i++){
        cx(reg0[i], regA1[i]);
    }
    for(int i = 0; i < regq.size(); i++){
        cx(reg1[i], regB1[i]);
    }
    x(regB0[0]);

    // Compare A1, B1 and swap if necessary so that A1 > B1
    // Dont uncompute yet since will do this swap again at end of computation
    QComp(regA, regB, aLessB, aGreatB, aEqualB, compAncillas);
    cswapn(aLessB, regA1, regB1);


    // Extended Euclidean algorithm
    for(int i = 0; i < cutoff; i++){
        // If either A1 == 0 or B1 == 0, flip control   
        for(int j = 0; j < regA1.size(); j++){\
            x(regA1[j]);
        }
        for(int j = 0; j < regB1.size(); j++){
            x(regB1[j]);
        }
        std::vector<std::string> temp(regA1.begin(), regA1.end());
        temp.insert( temp.end(), regB1.begin(), regB1.end() );
        toffoli_n(regA1, toffAncillas, control);
        toffoli_n(regB1, toffAncillas, control);
        toffoli_n(temp, toffAncillas, control);
        // If control high, increment counter
        caddQFTval(counter, control, 1);
        // Uncompute control flip 
        toffoli_n(temp, toffAncillas, control);
        toffoli_n(regB1, toffAncillas, control);
        toffoli_n(regA1, toffAncillas, control);
        for(int j = 0; j < regA1.size(); j++){\
            x(regA1[j]);
        }
        for(int j = 0; j < regB1.size(); j++){
            x(regB1[j]);
        }
        // If counter == 0, flip control
        for(int j = 0; j < counter.size(); j++){\
            x(counter[j]);
        }
        toffoli_n(counter, toffAncillas, control);
        // If control high, run iteration of extended euclidean algorithm
        extEuclideanCycle(regA0, regA1, regB0, regB1, control, euclideanAncillas);
        // Uncompute control flip
        toffoli_n(counter, toffAncillas, control);
        // If counter == 0, flip control
        for(int j = 0; j < counter.size(); j++){\
            x(counter[j]);
        }   
    }
    /////////////////////////////

    // Copy final value gcd(reg0, reg1) into result
    // At end of algorithm, we either have (A0,A1),(B0,B1) = (#,gcd(reg0,reg1)),(#,0) or (A0,A1),(B0,B1) = (#,0),(#,gcd(reg0,reg1))
    
    // If A0 == 0, flip control
    for(int i = 0; i < regA0.size(); i++){
        x(regA0[i]);
    }
    toffoli_n(regA0, toffAncillas, control);
    // If control high, copy B0 into result
    for(int i = 0; i < regB0.size(); i++){
        toffoli({control, regB0[i]}, result[i]);
    }
    // Uncompute control
    toffoli_n(regA0, toffAncillas, control);
    for(int i = 0; i < regA0.size(); i++){
        x(regA0[i]);
    }
    // If B0 == 0, flip control
    for(int i = 0; i < regB0.size(); i++){
        x(regB0[i]);
    }
    toffoli_n(regB0, toffAncillas, control);
    // If control high, copy B0 into result
    for(int i = 0; i < regA0.size(); i++){
        toffoli({control, regA0[i]}, result[i]);
    }
    // Uncompute control
    toffoli_n(regB0, toffAncillas, control);
    for(int i = 0; i < regB0.size(); i++){
        x(regB0[i]);
    }
    
    ////////////////////////////////////
    // Uncompute extended Euclidean algorithm
    for(int i = 0; i < cutoff; i++){
        // If counter == 0, flip control
        for(int j = 0; j < counter.size(); j++){\
            x(counter[j]);
        }
        toffoli_n(counter, toffAncillas, control);
        // If control low, run inverse iteration of extended euclidean algorithm
        x(control);
        inverse(extEuclideanCycle, regA0, regA1, regB0, regB1, control, euclideanAncillas);
        x(control);
        // Uncompute control flip
        toffoli_n(counter, toffAncillas, control);
        for(int j = 0; j < counter.size(); j++){\
            x(counter[j]);
        }
        // If either A1 == 0 or B1 == 0, flip control
        for(int j = 0; j < regA1.size(); j++){\
            x(regA1[j]);
        }
        for(int j = 0; j < regB1.size(); j++){
            x(regB1[j]);
        }
        std::vector<std::string> temp(regA1.begin(), regA1.end());
        temp.insert( temp.end(), regB1.begin(), regB1.end() );
        toffoli_n(regA1, toffAncillas, control);
        toffoli_n(regB1, toffAncillas, control);
        toffoli_n(temp, toffAncillas, control);
        // If control high, decrement counter
        csubQFTval(counter, control, 1);
        // Uncompute control flip 
        toffoli_n(temp, toffAncillas, control);
        toffoli_n(regB1, toffAncillas, control);
        toffoli_n(regA1, toffAncillas, control);
        for(int j = 0; j < regA1.size(); j++){\
            x(regA1[j]);
        }
        for(int j = 0; j < regB1.size(); j++){
            x(regB1[j]);
        }
    }

    // Uncompute initial controlled-swap
    cswapn(aLessB, regA1, regB1);
    inverse(QComp, regA, regB, aLessB, aGreatB, aEqualB, compAncillas);
    
    // Uncompute register initalization
    for(int i = 0; i < reg0.size(); i++){
        cx(reg0[i], regA1[i]);
    }
    for(int i = 0; i < reg1.size(); i++){
        cx(reg1[i], regB1[i]);
    }
    x(regB0[0]);

}


// Quantum modular inverse algorithm based on the extended Euclidean algorithm
// Signed integers, LSB first
// https://arxiv.org/abs/quant-ph/0301141
void modularInverse(std::vector<std::string> registerq, std::vector<std::string> registerN, std::vector<std::string> registerResult, std::vector<std::string> registerAncillas){
    std::vector<std::string> regq = parseQregisterVector(registerq);
    std::vector<std::string> regN = parseQregisterVector(registerN);
    std::vector<std::string> result = parseQregisterVector(registerResult);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas);

    std::string control;
    
    std::vector<std::string> toffAncillas;
    std::vector<std::string> euclideanAncillas;
    std::vector<std::string> addAncillas;

    std::vector<std::string> regA0;
    std::vector<std::string> regA1;
    std::vector<std::string> regB0;
    std::vector<std::string> regB1;
    // check input size

    // Max number of iterations
    // Verify max cutoff = 4.5*n
    int cutoff = std::ceil(4.5*registerN.size()); 

    // Initialize registers (A0,A1),(B0,B1) = (0,N),(1,q)
    for(int i = 0; i < regN.size(); i++){
        cx(regN[i], regA1[i]);
    }
    for(int i = 0; i < regq.size(); i++){
        cx(regq[i], regB1[i]);
    }
    x(regB0[0]);

    // Extended Euclidean algorithm
    for(int i = 0; i < cutoff; i++){
        // If either A1 == 0 or B1 == 0, flip control   
        for(int j = 0; j < regA1.size(); j++){\
            x(regA1[j]);
        }
        for(int j = 0; j < regB1.size(); j++){
            x(regB1[j]);
        }
        std::vector<std::string> temp(regA1.begin(), regA1.end());
        temp.insert( temp.end(), regB1.begin(), regB1.end() );
        toffoli_n(regA1, toffAncillas, control);
        toffoli_n(regB1, toffAncillas, control);
        toffoli_n(temp, toffAncillas, control);
        // If control high, increment counter
        caddQFTval(counter, control, 1);
        // Uncompute control flip 
        toffoli_n(temp, toffAncillas, control);
        toffoli_n(regB1, toffAncillas, control);
        toffoli_n(regA1, toffAncillas, control);
        for(int j = 0; j < regA1.size(); j++){\
            x(regA1[j]);
        }
        for(int j = 0; j < regB1.size(); j++){
            x(regB1[j]);
        }
        // If counter == 0, flip control
        for(int j = 0; j < counter.size(); j++){\
            x(counter[j]);
        }
        toffoli_n(counter, toffAncillas, control);
        // If control high, run iteration of extended euclidean algorithm
        extEuclideanCycle(regA0, regA1, regB0, regB1, control, euclideanAncillas);
        // Uncompute control flip
        toffoli_n(counter, toffAncillas, control);
        // If counter == 0, flip control
        for(int j = 0; j < counter.size(); j++){\
            x(counter[j]);
        }   
    }

    /////////////////////////////
    // Correct final value and copy to result register
    // At end of algorithm, we either have (A0,A1),(B0,B1) = (q^-1,1),(-N,0) or (A0,A1),(B0,B1) = (q^-1 - N,1),(N,0)
    // Copy A0 into result
    for(int i = 0; i < regA0.size(); i++){
        cx(regA0[i], result[i]);
    }
    // If B0 sign qubit is low, add B0 (N) to result
    // Store sign qubit in control
    cx(regB0[regB0.size() - 1], control);
    x(control);
    caddQFT(result, regB0, control);
    // Uncompute temp
    x(control);
    cx(regB0[regB0.size() - 1], control);


    ////////////////////////////////////
    // Uncompute extended Euclidean algorithm
    for(int i = 0; i < cutoff; i++){
        // If counter == 0, flip control
        for(int j = 0; j < counter.size(); j++){\
            x(counter[j]);
        }
        toffoli_n(counter, toffAncillas, control);
        // If control low, run inverse iteration of extended euclidean algorithm
        x(control);
        inverse(extEuclideanCycle, regA0, regA1, regB0, regB1, control, euclideanAncillas);
        x(control);
        // Uncompute control flip
        toffoli_n(counter, toffAncillas, control);
        for(int j = 0; j < counter.size(); j++){\
            x(counter[j]);
        }
        // If either A1 == 0 or B1 == 0, flip control
        for(int j = 0; j < regA1.size(); j++){\
            x(regA1[j]);
        }
        for(int j = 0; j < regB1.size(); j++){
            x(regB1[j]);
        }
        std::vector<std::string> temp(regA1.begin(), regA1.end());
        temp.insert( temp.end(), regB1.begin(), regB1.end() );
        toffoli_n(regA1, toffAncillas, control);
        toffoli_n(regB1, toffAncillas, control);
        toffoli_n(temp, toffAncillas, control);
        // If control high, decrement counter
        csubQFTval(counter, control, 1);
        // Uncompute control flip 
        toffoli_n(temp, toffAncillas, control);
        toffoli_n(regB1, toffAncillas, control);
        toffoli_n(regA1, toffAncillas, control);
        for(int j = 0; j < regA1.size(); j++){\
            x(regA1[j]);
        }
        for(int j = 0; j < regB1.size(); j++){
            x(regB1[j]);
        }
    }

    // Uncompute register initalization
    for(int i = 0; i < regN.size(); i++){
        cx(regN[i], regA1[i]);
    }
    for(int i = 0; i < regq.size(); i++){
        cx(regq[i], regB1[i]);
    }
    x(regB0[0]);


}

// Assumes A1 >= B1
void extEuclideanCycle(std::vector<std::string> registerA0, std::vector<std::string> registerA1, std::vector<std::string> registerB0, std::vector<std::string> registerB1, std::string control, std:vector<std::string> registerAncillas){
    std::vector<std::string> regA0 = parseQregisterVector(registerA0);
    std::vector<std::string> regA1 = parseQregisterVector(registerA1);
    std::vector<std::string> regB0 = parseQregisterVector(registerB0);
    std::vector<std::string> regB1 = parseQregisterVector(registerB1);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncillas); 

    std::string aLessB = ancillas[0];
    std::string aGreatB = ancillas[1];
    std::string aEqualB = ancillas[2];
    std::string aGreatOrEqualB = ancillas[3];

    std::vector<std::string> regQuotient;
    std::vector<std::string> regMul;
    std::vector<std::string> compAncillas;
    std::vector<std::string> quotientAncillas;
    std::vector<std::string> multAncillas;
    std::vector<std::string> subAncillas;
    // check input size

    // could implement check that A >= B?
    // run comparator to check if >=, and if not then swap, and run qcomp in reverse with inputs swapped to clear outputs
    // could also just calc modulus in upper level algorithm as initial check

    // A >= B
    // q = floor(A1/B1)
    cquotient_s(regA1, regB1, regQuotient, quotientAncillas, control);
    // q -> q*B0
    cmulMAC_s(regQuotient, regB0, multAncillas, regMul, control);
    // A0 -> A0 - q*B0
    csubQFT_s(regA0, regMul, control, subAncilla);
    // q*B0 -> q
    inverse(cmulMAC_s, regQuotient, regB0, multAncillas, regMul, control);
    // q -> q*B1
    cmulMAC_s(regQuotient, regB1, multAncillas, regMul, control);
    // A1 -> A1 - q*B1
    csubQFT_s(regA1, regMul, control, subAncilla);
    // q*B1 -> q
    inverse(cmulMAC_s, regQuotient, regB1, multAncillas, regMul, control);
    // Uncompute q
    // q = floor(-1*(A0 - qB0)/B0), original A0
    inverse(cquotient_s, regA0, regB0, regQuotient, quotientAncillas, aGreatOrEqualB, control);

    // Swap A0 <-> B0, A1 <-> B1 since now B1 >= A1
    cswapn(control, regA0, regB0);
    cswapn(control, regA1, regB1);
}   

void extEuclideanCycleFull(std::vector<std::string> registerA0, std::vector<std::string> registerA1, std::vector<std::string> registerB0, std::vector<std::string> registerB1, std::string control, std:vector<std::string> registerAncillas){
    std::vector<std::string> regA0 = parseQregisterVector(registerA0);
    std::vector<std::string> regA1 = parseQregisterVector(registerA1);
    std::vector<std::string> regB0 = parseQregisterVector(registerB0);
    std::vector<std::string> regB1 = parseQregisterVector(registerB1);
    std::vector<std::string> ancillas = parseQregisterVector(registerAncilla); 

    std::string aLessB = ancillas[0];
    std::string aGreatB = ancillas[1];
    std::string aEqualB = ancillas[2];
    std::string aGreatOrEqualB = ancillas[3];

    std::vector<std::string> regQuotient;
    std::vector<std::string> regMul;
    std::vector<std::string> compAncillas;
    std::vector<std::string> quotientAncillas;
    std::vector<std::string> multAncillas;
    std::string subAncilla;
    std::string toffAncilla;

    // need to implement control using toffoli
    // Compute comparison qubits
    QComp(regA, regB, aLessB, aGreatB, aEqualB, compAncillas);

    // Computer A >= B qubit
    toffoli({control, aGreatB}, aGreatOrEqualB);
    toffoli({control, aEqualB}, aGreatOrEqualB);
    toffoli_n({control, aGreatB, aEqualB}, {toffAncilla}, aGreatOrEqualB);
    //cx(aGreatB, aGreatOrEqualB);
    //cx(aEqualB, aGreatOrEqualB);
    //toffoli({aGreatB, aEqualB}, aGreatOrEqualB);

    // need to implement control using toffoli
    // If A >= B
    // q = floor(A1/B1)
    cquotient_s(regA1, regB1, regQuotient, quotientAncillas, aGreatOrEqualB);
    // q -> q*B0
    cmulMAC_s(regQuotient, regB0, multAncillas, regMul, aGreatOrEqualB);
    // A0 -> A0 - q*B0
    csubQFT_s(regA0, regMul, aGreatOrEqualB, subAncilla);
    // q*B0 -> q
    inverse(cmulMAC_s, regQuotient, regB0, multAncillas, regMul, aGreatOrEqualB);
    // q -> q*B1
    cmulMAC_s(regQuotient, regB1, multAncillas, regMul, aGreatOrEqualB);
    // A1 -> A1 - q*B1
    csubQFT_s(regA1, regMul, aGreatOrEqualB, subAncilla);
    // q*B1 -> q
    inverse(cmulMAC_s, regQuotient, regB1, multAncillas, regMul, aGreatOrEqualB);
    // Uncompute q
    // q = floor(-1*(A0 - qB0)/B0), original A0
    inverse(cquotient_s, regA0, regB0, regQuotient, quotientAncillas, aGreatOrEqualB);

    // need to implement control using toffoli
    // If A < B
    // q = floor(B1/A1)
    cquotient_s(regB1, regA1, regQuotient, quotientAncillas, aLessB);
    // q -> q*A0
    cmulMAC_s(regQuotient, regA0, multAncillas, regMul, aLessB);
    // B0 -> B0 - q*A0
    csubQFT_s(regB0, regMul, aLessB, subAncilla);
    // q*A0 -> q
    inverse(cmulMAC_s, regQuotient, regA0, multAncillas, regMul, aLessB);
    // q -> q*A1
    cmulMAC_s(regQuotient, regA1, multAncillas, regMul, aLessB);
    // B1 -> B1 - q*A1
    csubQFT_s(regB1, regMul, aLessB, subAncilla);
    // q*A1 -> q
    inverse(cmulMAC_s, regQuotient, regA1, multAncillas, regMul, aLessB);
    // Uncompute q
    // q = floor(-1*(B0 - qA0)/A0), original B0
    inverse(cquotient_s, regB0, regA0, regQuotient, quotientAncillas, aLessB);

    // need to implement control using toffoli
    // Ucompute comparison qubits
    
    
    toffoli_n({control, aGreatB, aEqualB}, {toffAncilla}, aGreatOrEqualB);
    toffoli({control, aEqualB}, aGreatOrEqualB);
    toffoli({control, aGreatB}, aGreatOrEqualB);

    //toffoli({aGreatB, aEqualB}, aGreatOrEqualB);
    //cx(aEqualB, aGreatOrEqualB);
    //cx(aGreatB, aGreatOrEqualB);

    QComp(regA, regB, aLessB, aGreatB, aEqualB, compAncillas);
}   