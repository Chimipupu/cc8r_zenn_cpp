#ifndef CC8R_HPP
#define CC8R_HPP

#include <array>
#include <cstdint>

// フラグのビット位置
const uint8_t FLAG_ZERO = 0b10000000;
const uint8_t FLAG_CARRY = 0b01000000;
const uint8_t FLAG_OVERFLOW = 0b00100000;
const uint8_t FLAG_NEGATIVE = 0b00010000;

// オペコード
const uint8_t OP_NOP = 0x00;
const uint8_t OP_HALT = 0x10;
const uint8_t OP_LDI = 0x14;
const uint8_t OP_MV = 0x18;
const uint8_t OP_ADD = 0x20;
const uint8_t OP_SUB = 0x30;
const uint8_t OP_MUL = 0x40;
const uint8_t OP_DIV = 0x50;
const uint8_t OP_AND = 0x60;
const uint8_t OP_OR = 0x70;
const uint8_t OP_XOR = 0x80;
const uint8_t OP_SHL = 0x90;
const uint8_t OP_SHR = 0xA0;
const uint8_t OP_PUSH = 0xB0;
const uint8_t OP_POP = 0xC0;
const uint8_t OP_JMP = 0xD0;
const uint8_t OP_JZ = 0xE0;
const uint8_t OP_JNZ = 0xF0;

class CC8R {
    public:
        std::array<uint8_t, 8> registers;
        uint8_t pc;
        uint8_t sp;
        uint8_t flags;
        std::array<uint8_t, 256> memory;

        CC8R();
        uint8_t fetch();
        void update_flags(uint8_t result);
        bool execute(uint8_t instruction);
        void display() const;
};

#endif