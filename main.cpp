#include "cc8r.hpp"
#include <iostream>

int main() {
    CC8R cpu;

    // (5+3)x2 のプログラム
    uint8_t program[] = {
        0x14, 0x01, 0x05, // LDI R1, 5
        0x14, 0x02, 0x03, // LDI R2, 3
        0x14, 0x03, 0x02, // LDI R3, 2
        0x20, 0x01, 0x02, // ADD R1, R2
        0x18, 0x04, 0x00, // MV R4, R0
        0x40, 0x03, 0x04, // MUL R3, R4
        0x10              // HALT
    };

    for (int i = 0; i < sizeof(program); i++) {
        cpu.memory[i] = program[i];
    }

    while (cpu.execute(cpu.fetch()));

    cpu.display();

    return 0;
}