#include "parser.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

// Utility function to remove leading and trailing whitespace from a string
static std::string trim(const std::string &str)
{
    size_t first = str.find_first_not_of(" \t\r\n");
    size_t last = str.find_last_not_of(" \t\r\n");

    if (first == std::string::npos || last == std::string::npos)
    {
        return "";
    }

    return str.substr(first, last - first + 1);
}

// Parses the input file and returns a ParsedInput struct with registers, memory, and instructions
ParsedInput Parser::parseInputFile(const std::string &filename)
{
    ParsedInput data;
    std::ifstream infile(filename);

    if (!infile)
    {
        std::cerr << "Error: Cannot open file " << filename << "\n";
        exit(1);
    }

    std::string line;
    enum Section
    {
        NONE,
        REG,
        MEM,
        CODE
    };
    Section section = NONE; // Tracks which part of the input we're in

    // Switch section based on keyword headers
    while (std::getline(infile, line))
    {
        line = trim(line);
        if (line.empty())
            continue;

        if (line == "REGISTERS")
        {
            section = REG;
            continue;
        }
        else if (line == "MEMORY")
        {
            section = MEM;
            continue;
        }
        else if (line == "CODE")
        {
            section = CODE;
            continue;
        }

        // Break line into tokens
        std::istringstream iss(line);
        
        /* TODO */
        // Parse a register line 
        if (section == REG)
        {
            std::string reg; 
            int value; 
            
            // Extract register name and its value from the line
            iss >> reg >> value; 
            
            // Make sure the register name starts with 'R'
            if (reg[0] == 'R')
            {
                int regNum;
            	if(reg.length()>2){ regNum = std::stoi(reg.substr(1,2)); }
            	else{  regNum = std::stoi(reg.substr(1)); } 
                if(regNum != 0) // R0 is always 0 in MIPS
                    data.registers[regNum] = value; // Convert to register (store int)
            }

        } else if (section == MEM) // Parse a memory line
        {
            int addr, value; 
            // Read memory address and its contents
            iss >> addr >> value; 
            // Store in the memory map
            data.memory[addr] = value; 
        }
        // Parse a code line
        else if (section == CODE)
        {
            // Entire line is assumed to be a 32-bit instruction.
            data.binaryInstructions.push_back(line);
        }
    }
    // When done parsing all lines, return the filled ParsInput struct
    return data;
}
