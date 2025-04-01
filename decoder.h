#ifndef DECODER_H
#define DECODER_H

#include "instruction.h"
#include <string>
#include <vector>

// This class converts raw binary strings to fully decoded Instruction objects
class Decoder
{
public:
    // Converts a vector of binary strings to a vector of decoded Instruction structs
    static std::vector<Instruction> decodeAll(const std::vector<std::string> &binaryList);
};

#endif
