// Header file with function declaration and documentation

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <unordered_map>
#include <vector>

// This struct holds the results of parsing the input file
struct ParsedInput
{
    std::unordered_map<int, int> registers;
    std::unordered_map<int, int> memory;
    std::vector<std::string> binaryInstructions;
};

class Parser
{
public:
    // Main function that reads and parses the input file
    static ParsedInput parseInputFile(const std::string &filename);
};

#endif
