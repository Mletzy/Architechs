#include "decoder.h"

// Decode a list of binary instruction strings into Instruction structs
std::vector<Instruction> Decoder::decodeAll(const std::vector<std::string> &binaryList)
{
    std::vector<Instruction> result; // Vector to store decoded instructions
    int idx = 1;                     // Instruction index

    // Loop through each binary string and create a decoded Instrucction object
    for (const auto &binary : binaryList)
    {
        result.emplace_back(binary, idx++); // Emplace = contruct in-place
    }

    // Return all decoded instructions
    return result;
}