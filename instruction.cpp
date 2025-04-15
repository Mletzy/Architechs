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


// Helper function for converting binary strings to integers as part of decode(). -Jonathan Arc
unsigned int binToDec(const std::string& bin, int start, int end) { // Converts binary string into integer value
    unsigned int value = 0;
    for (int i = start; i <= end; i++) {
        value = (value << 1) | (bin[i] - '0'); // Combines shifted left binary with next binary digit. Note: '0'-'0'=0 and '1'-'0'=1
    }
    return value;
}


// Decodes a 32-bit binary string into R-type or I-type fields
void Instruction::decode()
{
    opcode = std::stoi(raw.substr(0, 6), nullptr, 2); // First 6 bits = opcode

    if(opcode == 0x00){        // R-type instruction (opcode = 0)
        rs = binToDec(raw, 6, 10);
        rt = binToDec(raw, 11, 15);
        rd = binToDec(raw, 16, 20);
        shamt = binToDec(raw, 21, 25);
        func = binToDec(raw, 26, 31);

    	}
    else{					// I-type instruction
        rs = binToDec(raw, 6, 10);
        rt = binToDec(raw, 11, 15);
        immediate = binToDec(raw, 16, 31);
    	}

    if (raw[16] == '1'){    // sign-extend 16-bit immediate if it's negative (starts with '1')        
        immediate = (short)immediate; //cast to signed value
    }
}
