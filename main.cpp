#include <iostream>
#include "parser.h"
#include "decoder.h"
#include "instruction.h"

// Placeholder: to be edited later
void runSimulation(const std::unordered_map<int, int> &registers, 
                   const std::unordered_map<int, int> &memory, 
                   const std::vector<Instruction> &instructions)
{
    std::cout << "\nSIMULATION START\n";

    int cycle = 1; // Tracks the clock cycle
    int pc = 0;    // Program counter (index of instruction)

    std::vector<std::string> timeline; // Store cycle-by-cycle execution

    while (pc < instructions.size()) 
    {
        Instruction inst = instructions[pc];

        // Log cycles
        timeline.push_back("C#" + std::to_string(cycle) + " I" + std::to_string(inst.index) + "-IF");
        cycle++;
        timeline.push_back("C#" + std::to_string(cycle) + " I" + std::to_string(inst.index) + "-ID");
        cycle++;

        // Execute instruction
        switch (inst.opcode) 
        {
          case 0: //(opcode = 000000) For R-type instructions
            /*if( inst.funct == 32 ) {//ADD (funct= 100000)
             //rd register will contain rs value + rt value
               registers[inst.rd] = registers[inst.rs] + registers[inst.rt];
               }
               else if( inst.funct == 34 ) {//SUB (funct= 100010) -Jonathan Arc
               registers[inst.rd] = registers[inst.rs] - registers[inst.rt];
               }               
            */

          break;
            case 4: // BEQ (opcode = 000100)
              /*
                timeline.push_back("C#" + std::to_string(cycle) + " I" + std::to_string(inst.index) + "-EX");
                cycle++;
                if (registers[inst.rs] == registers[inst.rt]) {
                pc += inst.immediate; // Branch if equal
                }
                break;
              */
            case 5: // BNE (opcode = 000101)
                timeline.push_back("C#" + std::to_string(cycle) + " I" + std::to_string(inst.index) + "-EX");
                cycle++;
                /*
                if (registers[inst.rs] != registers[inst.rt]) {
                    pc += inst.immediate; // Branch
                }
                */
                break;
            case 8: //ADDI (opcode = 001000)
                timeline.push_back("C#" + std::to_string(cycle) + " I" + std::to_string(inst.index) + "-EX");
                cycle++;
                /*
                //rd = rs + imm;
                 registers[inst.rd] = registers[inst.rs] + inst.immediate;

                */
              
                break;
          
        }

        cycle++;
        pc++; // Move to the next instruction
    }

    // Print timeline
    for (const auto &entry : timeline) {
      std::cout << entry << std::endl;
    }

    // Print final register contents
    std::cout << "\nREGISTERS\n";
    for (const auto &reg : registers) {
        if (reg.second != 0) {
            std::cout << "R" << reg.first << " " << reg.second << "\n";
        }
    }

    // Print final memory contents
    std::cout << "\nMEMORY\n";
    for (const auto &mem : memory) {
        if (mem.second != 0) {
            std::cout << mem.first << " " << mem.second << "\n";
        }
    }

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
