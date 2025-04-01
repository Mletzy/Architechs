#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>

// This struct represents a single decoded MIPS instruction
struct Instruction
{
    std::string raw;  // 32-bit binary string
    std::string type; // Type of instruction
    int index;        // Instruction number

    /* TODO */
    // Fields extracted from the binary instruction
    int opcode = 0;
    // Constructor: takes binary string + instruction ndex and immediately decodes it
    Instruction(const std::string &binary, int idx);

    // Function that descodes the binary string into fields above (in Instruction.cpp)
    void decode();
};

#endif
