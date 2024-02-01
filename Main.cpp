#include "xml_reader.h"
#include "structures.h"
#include "global_var.cpp"

#include "Bai1.cpp"
#include "Bai2.cpp"
#include "Bai3.cpp"
#include "Bai4.cpp"
#include "Bai5.cpp"
#include "Bai6.cpp"
#include "Bai7.cpp"

void init() {
    Constant::v = 1;
    H = 20;
    deltaT = 1000;
}

int main() {
    init();
    // Bai_1();
    // Bai_2();
    // Bai_3();
    // Bai_4();
    // Bai_5();
    // Bai_6();
    Bai_7();

    return 0;
}