#include "cc8r.hpp"
#include <iostream>

CC8R::CC8R()
{
    registers.fill(0);
    pc = 0;
    sp = 0xF0;
    flags = 0;
    memory.fill(0);
}

uint8_t CC8R::fetch()
{
    uint8_t instruction = memory[pc];
    pc = (pc + 1) % 256;
    return instruction;
}

void CC8R::update_flags(uint8_t result)
{
    if (result == 0)
    {
        flags |= FLAG_ZERO;
    }
    else
    {
        flags &= ~FLAG_ZERO;
    }

    if (result & 0x80)
    {
        flags |= FLAG_NEGATIVE;
    }
    else
    {
        flags &= ~FLAG_NEGATIVE;
    }
}

bool CC8R::execute(uint8_t instruction)
{
    switch (instruction)
    {
    case OP_NOP:
        std::cout << "NOP\n";
        break;
    case OP_HALT:
        std::cout << "HALT\n";
        return false;
    case OP_LDI:
    {
        uint8_t ra = fetch();
        uint8_t value = fetch();
        registers[ra] = value;
        std::cout << "LDI R" << (int)ra << ", " << (int)value << "\n";
        update_flags(value);
        break;
    }
    case OP_MV:
    {
        uint8_t ra = fetch();
        uint8_t rb = fetch();
        registers[ra] = registers[rb];
        std::cout << "MV R" << (int)ra << ", R" << (int)rb << "\n";
        update_flags(registers[rb]);
        break;
    }
    case OP_ADD:
    {
        uint8_t ra = fetch();
        uint8_t rb = fetch();
        uint16_t result = registers[ra] + registers[rb];
        registers[0] = result & 0xFF;
        std::cout << "ADD R" << (int)ra << ", R" << (int)rb << "\n";
        update_flags(result & 0xFF);
        if (result > 0xFF)
        {
            flags |= FLAG_CARRY;
        }
        else
        {
            flags &= ~FLAG_CARRY;
        }
        break;
    }
    case OP_SUB:
    {
        uint8_t ra = fetch();
        uint8_t rb = fetch();
        uint16_t result = registers[ra] - registers[rb];
        registers[0] = result & 0xFF;
        std::cout << "SUB R" << (int)ra << ", R" << (int)rb << "\n";
        update_flags(result & 0xFF);
        if (result > 0xFF)
        {
            flags |= FLAG_CARRY;
        }
        else
        {
            flags &= ~FLAG_CARRY;
        }
        break;
    }
    case OP_MUL:
    {
        uint8_t ra = fetch();
        uint8_t rb = fetch();
        uint16_t result = registers[ra] * registers[rb];
        registers[0] = result & 0xFF;
        std::cout << "MUL R" << (int)ra << ", R" << (int)rb << "\n";
        update_flags(result & 0xFF);
        if (result > 0xFF)
        {
            flags |= FLAG_OVERFLOW;
        }
        else
        {
            flags &= ~FLAG_OVERFLOW;
        }
        break;
    }
    case OP_DIV:
    {
        uint8_t ra = fetch();
        uint8_t rb = fetch();
        if (registers[rb] != 0)
        {
            uint8_t result = registers[ra] / registers[rb];
            registers[0] = result;
            std::cout << "DIV R" << (int)ra << ", R" << (int)rb << "\n";
            update_flags(result);
        }
        else
        {
            flags |= FLAG_OVERFLOW;
            std::cout << "Division by zero!\n";
        }
        break;
    }
    case OP_AND:
    {
        uint8_t ra = fetch();
        uint8_t rb = fetch();
        uint8_t result = registers[ra] & registers[rb];
        registers[0] = result;
        std::cout << "AND R" << (int)ra << ", R" << (int)rb << "\n";
        update_flags(result);
        break;
    }
    case OP_OR:
    {
        uint8_t ra = fetch();
        uint8_t rb = fetch();
        uint8_t result = registers[ra] | registers[rb];
        registers[0] = result;
        std::cout << "OR R" << (int)ra << ", R" << (int)rb << "\n";
        update_flags(result);
        break;
    }
    case OP_XOR:
    {
        uint8_t ra = fetch();
        uint8_t rb = fetch();
        uint8_t result = registers[ra] ^ registers[rb];
        registers[0] = result;
        std::cout << "XOR R" << (int)ra << ", R" << (int)rb << "\n";
        update_flags(result);
        break;
    }
    case OP_SHL:
    {
        uint8_t ra = fetch();
        uint8_t result = registers[ra] << 1;
        registers[ra] = result;
        std::cout << "SHL R" << (int)ra << "\n";
        update_flags(result);
        break;
    }
    case OP_SHR:
    {
        uint8_t ra = fetch();
        uint8_t result = registers[ra] >> 1;
        registers[ra] = result;
        std::cout << "SHR R" << (int)ra << "\n";
        update_flags(result);
        break;
    }
    case OP_PUSH:
    {
        uint8_t ra = fetch();
        sp = (sp - 1) % 256;
        memory[sp] = registers[ra];
        std::cout << "PUSH R" << (int)ra << "\n";
        break;
    }
    case OP_POP:
    {
        uint8_t ra = fetch();
        registers[ra] = memory[sp];
        sp = (sp + 1) % 256;
        std::cout << "POP R" << (int)ra << "\n";
        update_flags(registers[ra]);
        break;
    }
    case OP_JMP:
    {
        uint8_t addr = fetch();
        pc = addr;
        std::cout << "JMP to 0x" << std::hex << (int)addr << "\n";
        break;
    }
    case OP_JZ:
    {
        uint8_t addr = fetch();
        if (flags & FLAG_ZERO)
        {
            pc = addr;
            std::cout << "JZ to 0x" << std::hex << (int)addr << "\n";
        }
        else
        {
            std::cout << "Skipping JZ\n";
        }
        break;
    }
    case OP_JNZ:
    {
        uint8_t addr = fetch();
        if (!(flags & FLAG_ZERO))
        {
            pc = addr;
            std::cout << "JNZ to 0x" << std::hex << (int)addr << "\n";
        }
        else
        {
            std::cout << "Skipping JNZ\n";
        }
        break;
    }
    default:
        std::cout << "Unknown opcode: 0x" << std::hex << (int)instruction << "\n";
        return false;
    }
    return true;
}

void CC8R::display() const
{
    std::cout << "Registers: ";
    for (uint8_t reg : registers)
    {
        std::cout << (int)reg << " ";
    }
    std::cout << "\nFlags: 0x" << std::hex << (int)flags << std::dec
              << "\nSP: 0x" << std::hex << (int)sp << std::dec
              << "\nPC: 0x" << std::hex << (int)pc << std::dec << "\n";
}