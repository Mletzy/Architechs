#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>

// This struct represents a single decoded MIPS instruction
struct Instruction
{
    std::string raw;  // 32-bit binary string
    std::string type; // Type of instruction
    int index;        // Instruction number

    // Fields extracted from the binary instruction  -Jonathan Arc
    int opcode = 0;
    int rs;
    int rd;
    int rt;
    int shamt;
    int func;
    int immediate;

    // Constructor: takes binary string + instruction ndex and immediately decodes it
    Instruction(const std::string &binary, int idx);

    // Function that descodes the binary string into fields above (in Instruction.cpp)
    void decode();
};

#endif
