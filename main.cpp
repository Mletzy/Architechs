#include <iostream>
#include "parser.h"
#include "decoder.h"
#include "instruction.h"

// Placeholder: to be edited later
void runSimulation(const std::unordered_map<int, int> &registers, const std::unordered_map<int, int> &memory, const std::vector<Instruction> &instructions)
{
    std::cout << "\nSIMULATION START\n";

    // Just print out the decoded instructions for now
    for (const auto &inst : instructions)
    {
        std::cout << "I" << inst.index << " - Type: " << inst.type << ", Opcode: " << inst.opcode << "\n";
    }

    std::cout << "\nSIMULATION END\n";
}

int main()
{
    while (true)
    {
        std::string inputFile, outputFile;
        std::cout << "Enter input and output filenames: ";
        std::cin >> inputFile >> outputFile;

        // Step 1: Parse file and extract registers, memory, binary instructions
        ParsedInput parsed = Parser::parseInputFile(inputFile);

        // Step 2: Decode binary instructions into Instruction structs
        std::vector<Instruction> decoded = Decoder::decodeAll(parsed.binaryInstructions);

        // Step 3: Run simulation logic (place holder)
        runSimulation(parsed.registers, parsed.memory, decoded);

        // Ask user if they want to simulate another file
        std::string cont;
        std::cout << "Run another simulation? (y/n): ";
        std::cin >> cont;
        if (cont != "y")
            break;
    }

    return 0;
}
