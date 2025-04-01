#include "instruction.h"
#include <bitset>
#include <stdexcept>

// Initializes fields and immediately calls decode()
Instruction::Instruction(const std::string &binary, int idx) : raw(binary), index(idx)
{
    // Validate the instruction length
    if (binary.length() != 32)
    {
        throw std::invalid_argument("Instruction must be 32 bits");
    }

    // Decode fields from the binary
    decode();
}

// Decodes a 32-bit binary string into R-type or I-type fields
/* TODO */
void Instruction::decode()
{
    opcode = std::stoi(raw.substr(0, 6), nullptr, 2); // First 6 bits = opcode

    // R-type instruction (opcode = 0)
    // I-type instruction
    // sign-extend 16-bit immediate if it's negative (starts with '1')
    if (raw[16] == '1')
    {
        // Extend to 32-bit signed value
    }
}