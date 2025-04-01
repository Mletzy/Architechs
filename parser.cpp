// Actual Implementation
#include "parser.h"
#include <fstream>     
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

/* 
    Helper function to trim whitespace from a string (start and end) 
    return "" if all spaces OR empty 
    return string
*/
static std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    size_t last = str.find_last_not_of(" \t\r\n");

    if (first == std::string::npos || last == std::string::npos)
    {
        return ""; 
    }

    return str.substr(first, last - first + 1);
}

/* 
    Main parser function
    Open the input file
    Remove surrounding whitespace
    Skip blank lines
    Switch section based on keyword
    Break line into words
    Remove 'R' and convert to integer
    Read the whole line as a binary string
*/
ParsedInput Parser::parseInputFile(const std::string& filename) { ParsedInput data;
    std::ifstream infile(filename); 
    if(!infile) 
    {
        std::cerr << "Error: Cannot open file " << filename << "\n";
        exit(1);
    }

    std::string line; 
    enum Section { NONE, REG, MEM, CODE };
    Section section = NONE;

    while (std::getline(infile, line))
    {
        line = trim(line);
        if (line.empty()) continue;

        if(line == "REGISTERS")
        {
            section = REG;
            continue;
        } else if (line == "MEMORY")
        {
            section = MEM; 
            continue;
        }   else if(line == "CODE")
        {
            section = CODE;
            continue;
        }

    }
}