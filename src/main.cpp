#include "globals.h"
#include "simpleGates.h"
#include "toffoli.h"

int main() {
    initAlgorithm();

    addQRegister("q", 4);
    addCRegister("c", 4);
    initAllRegisters();
    //addComment("");

    x("q[3]");
    inverse(x, "q[3]");
    //addComment("");

    cx("q[2]", "q[3]");
    inverse(cx, "q[2]", "q[3]");
    //addComment("");

    h("q[1]");
    inverse(h, "q[1]");
    //addComment("");

    u(1,2,3, "q[0]");
    inverse(u, 1,2,3, "q[0]");

    measureRegister("q", "c");

    printAlg();
}
