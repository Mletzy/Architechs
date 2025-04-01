//Header file with function declaration and documentation

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <unordered_map>    
#include <vector>           

/* 
    Maps register numbers and memory addresses to values
    Holds the list of binary instruction strings
    Load and parse the input file
*/

struct ParsedInput 
{ 
    std::unordered_map<int, int> registers;
    std::unordered_map<int, int> memory; 
    std::vector<std::string> binaryInstructions;
};

class Parser 
{
public:
    static ParsedInput parseInputFile(const std::string& filename);
};

#endif
