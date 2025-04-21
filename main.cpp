#include <iostream>
#include <fstream>
#include "parser.h"
#include "decoder.h"
#include "instruction.h"

//G.G changed the const for registers and memory so I could directly access them in SW and LW
void runSimulation( std::unordered_map<int, int> &registers, 
                    std::unordered_map<int, int> &memory, 
                   const std::vector<Instruction> &instructions)
{
    std::cout << "\nSIMULATION START\n";
    std::ofstream output("output.txt");

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
        int offset;
        int effAdd;


          case 0: //(opcode = 000000) For R-type instructions
            if( inst.func == 32 ) {//ADD (funct= 100000)
             //rd register will contain rs value + rt value
               registers[inst.rd] = registers[inst.rs] + registers[inst.rt];
                //Log remaining stages - Gregory Maddox 
                timeline.push_back("C#" + std::to_string(cycle++) + " I" + std::to_string(inst.index) + "-EX");
                timeline.push_back("C#" + std::to_string(cycle++) + " I" + std::to_string(inst.index) + "-WB");
               }
               else if( inst.func == 34 ) {//SUB (funct= 100010) -Jonathan Arc
               registers[inst.rd] = registers[inst.rs] - registers[inst.rt];
                //Log remaining stages - Gregory Maddox 
                timeline.push_back("C#" + std::to_string(cycle++) + " I" + std::to_string(inst.index) + "-EX");
                timeline.push_back("C#" + std::to_string(cycle++) + " I" + std::to_string(inst.index) + "-WB"); 
               } else if (inst.func == 42) { // SLT (Set Less Than) - Gregory Maddox 
                    if (registers[inst.rs] < registers[inst.rt]) {
                    registers[inst.rd] = 1;
                    } else {
                    registers[inst.rd] = 0;
                    }
                timeline.push_back("C#" + std::to_string(cycle++) + " I" + std::to_string(inst.index) + "-EX");
                timeline.push_back("C#" + std::to_string(cycle++) + " I" + std::to_string(inst.index) + "-WB");
                } else{
                std::cerr << "[ERROR] Unsupported R-type funct code: " << inst.func 
                          << " in instruction I" << inst.index << std::endl;
               } // Warning added - Gregory Maddox 
               
              

          break;
            case 4: // BEQ (opcode = 000100)
                timeline.push_back("C#" + std::to_string(cycle) + " I" + std::to_string(inst.index) + "-EX");
                cycle++;
                if (registers[inst.rs] == registers[inst.rt]) {
                pc += inst.immediate; // Branch if equal
                continue; // skip automatic pc++ - Gregory Maddox
                }
                break;

            case 5: // BNE (opcode = 000101)
                timeline.push_back("C#" + std::to_string(cycle) + " I" + std::to_string(inst.index) + "-EX");
                cycle++;
                if (registers[inst.rs] != registers[inst.rt]) {
                    pc += inst.immediate; // Branch
                    continue; // skip automatic pc++ - Gregory Maddox
                }
                break;
            case 8: //ADDI (opcode = 001000)
                timeline.push_back("C#" + std::to_string(cycle) + " I" + std::to_string(inst.index) + "-EX");
                timeline.push_back("C#" + std::to_string(cycle) + " I" + std::to_string(inst.index) + "-WB"); // Edited to include - Gregory M. 
                cycle++;
                 //rd = rs + imm;
                 registers[inst.rt] = registers[inst.rs] + inst.immediate; // edited to rt - Gregory M. 
                break;

          case 35: //LW (opcode = 100011) 
              // Added cycle++ for separate cycle logging - Gregory Maddox  
                timeline.push_back("C#" + std::to_string(cycle++) + " I" + std::to_string(inst.index) + "-EX");
                timeline.push_back("C#" + std::to_string(cycle++) + " I" + std::to_string(inst.index) + "-MEM");
                timeline.push_back("C#" + std::to_string(cycle++) + " I" + std::to_string(inst.index) + "-WB");
                
                // Calculate effective address - Gregory M. 
                effAdd = registers[inst.rs] + inst.immediate;

                /*
                cycle++;
                //rd = rs + imm;
                registers[inst.rd] = registers[inst.rs] + inst.immediate;
                offset = inst.immediate;
                effAdd = offset + registers[inst.rs];
                */

                if (effAdd % 4 != 0) {
                    std::cerr << "Unaligned memory access at address: " << effAdd << std::endl;
                    return;
                }

                registers[inst.rt] = memory[effAdd];
                break;

            	case 43: //SW (opcode = 101011)
                //Added cycle++ for separate cycle logging - Gregory Maddox 
                timeline.push_back("C#" + std::to_string(cycle++) + " I" + std::to_string(inst.index) + "-EX");
                timeline.push_back("C#" + std::to_string(cycle++) + " I" + std::to_string(inst.index) + "-MEM");
                //No Write-back in SW - Gregory Maddox 
                //timeline.push_back("C#" + std::to_string(cycle++) + " I" + std::to_string(inst.index) + "-WB");
                
                // Calculate effective memory address -  Gregory Maddox
                effAdd = registers[inst.rs] + inst.immediate;
                /*
                 registers[inst.rd] = registers[inst.rs] + inst.immediate;

                offset = inst.immediate;
                effAdd = offset + registers[inst.rs];
                */

                if (effAdd % 4 != 0) {
                    std::cerr << "Unaligned memory access at address: " << effAdd << std::endl;
                    return;
                }

                memory[effAdd] = registers[inst.rt];

                break;
          
        }
        cycle++;
        pc++; // Move to the next instruction
    }

    // Print timeline
    for (const auto &entry : timeline) {
      std::cout << entry << std::endl;
      output << entry << std::endl;
    }

    // Print final register contents
    std::cout << "\nREGISTERS\n";
    output << "REGISTERS\n";
    for (const auto &reg : registers) {
        if (reg.second != 0) {
            std::cout << "R" << reg.first << " " << reg.second << "\n";
            output << "R" << reg.first << " " << reg.second << "\n";
        }
    }

    // Print final memory contents
    std::cout << "\nMEMORY\n";
    output << "MEMORY\n";
    for (const auto &mem : memory) {
        if (mem.second != 0) {
            std::cout << mem.first << " " << mem.second << "\n";
            output << mem.first << " " << mem.second << "\n";
        }
    }

    // Just print out the decoded instructions for now
    for (const auto &inst : instructions)
    {
        std::cout << "I" << inst.index << " - Type: " << inst.type << ", Opcode: " << inst.opcode << "\n";
    }

    std::cout << "\nSIMULATION END\n";
    output.close();
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
