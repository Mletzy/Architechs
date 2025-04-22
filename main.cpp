#include <iostream>
#include <fstream>
#include "parser.h"
#include "decoder.h"
#include "instruction.h"


//G.G changed the const for registers and memory so I could directly access them in SW and LW
void runSimulation( std::unordered_map<int, int> &registerMap,
                    std::unordered_map<int, int> &memoryMap,
                   const std::vector<Instruction> &instructions)
{
    std::cout << "\nSIMULATION START\n";
    std::ofstream output("output.txt");

    int cycle = 1; // Tracks the clock cycle
    int pc = 0;    // Program counter (index of instruction)
    int instruction=0; //Number of instructions printed.

    std::vector<std::string> timeline; // Store cycle-by-cycle execution

    int registers[2][32]; // Array for holding Register data.
    for(int i=0; i<32; i++){ // Registers array initialization
    	registers[0][i]=0; // 0 in first index means unused. 1 means used.
    	registers[1][i]=0; // Value stored in register.
    }


    for(const auto &reg : registerMap) {
   	 registers[0][(reg.first)]=1; // Mark that register as having been used.
	 registers[1][reg.first]=reg.second; // Store value from input file in register.
    }

    int memory[2][250]; // Array for holding Register data.
    for(int i=0; i<250; i++){ // Registers array initialization
    	memory[0][i]=0; // 0 in first index means unused. 1 means used.
    	memory[1][i]=0; // Value stored in register.
    }

    for(const auto &mem : memoryMap) {
   	 memory[0][(mem.first/4)]=1; // Mark that register as having been used.
	 memory[1][mem.first/4]=mem.second; // Store value from input file in register.
    }

    /*
     * These terms refer to the stages of the MIPS pipeline:
     * Instruction Fetch (IF), Instruction Decode (ID), Execute (EX), Memory Access (MEM), and Write Back (WB).
     */

    while (pc < instructions.size()) 
    {
        Instruction inst = instructions[pc];
        instruction++;
        // Log cycles
        timeline.push_back("C#" + std::to_string(cycle) + " I" + std::to_string(instruction) + "-IF");
        cycle++;
        timeline.push_back("C#" + std::to_string(cycle) + " I" + std::to_string(instruction) + "-ID");
        cycle++;

        // Execute instruction
        switch (inst.opcode) 
        {
        int offset;
        int effAdd;


          case 0: //(opcode = 000000) For R-type instructions
            timeline.push_back("C#" + std::to_string(cycle) + " I" + std::to_string(instruction) + "-EX");
            cycle++;
            timeline.push_back("C#" + std::to_string(cycle) + " I" + std::to_string(instruction) + "-WB");
          	registers[0][inst.rd] = 1; // Mark as used.
            if( inst.func == 32 ) {//ADD (funct= 100000)
             //rd register will contain rs value + rt value
               registers[1][inst.rd] = registers[1][inst.rs] + registers[1][inst.rt];
               }
               else if( inst.func == 34 ) {//SUB (funct= 100010) -Jonathan Arc
               registers[1][inst.rd] = registers[1][inst.rs] - registers[1][inst.rt];
               }          
               else if (inst.func == 42) { // SLT (Set Less Than)
                timeline.push_back("C#" + std::to_string(cycle) + " I" + std::to_string(instruction) + "-EX");
                cycle++;
                timeline.push_back("C#" + std::to_string(cycle) + " I" + std::to_string(instruction) + "-WB");
                registers[0][inst.rd] = 1; // Mark destination register as used
                if (registers[1][inst.rs] < registers[1][inst.rt]) {
                    registers[1][inst.rd] = 1;
                } else {
                    registers[1][inst.rd] = 0;
                }
            }  
          break;

            case 4: // BEQ (opcode = 000100)
                timeline.push_back("C#" + std::to_string(cycle) + " I" + std::to_string(instruction) + "-EX");
                //cycle++;
                if (registers[1][inst.rs] == registers[1][inst.rt]) {
                pc += inst.immediate; // Branch if equal
                }
                break;

            case 5: // BNE (opcode = 000101)
                timeline.push_back("C#" + std::to_string(cycle) + " I" + std::to_string(instruction) + "-EX");
                //cycle++;
                if (registers[1][inst.rs] != registers[1][inst.rt]) {
                    pc += inst.immediate; // Branch
                }
                break;
            case 8: //ADDI (opcode = 001000)
                timeline.push_back("C#" + std::to_string(cycle) + " I" + std::to_string(instruction) + "-EX");
                cycle++;
		timeline.push_back("C#" + std::to_string(cycle) + " I" + std::to_string(instruction) + "-WB");
                 //rd = rs + imm;
                 registers[0][inst.rt] = 1; //Mark register as in use.
                 registers[1][inst.rt] = registers[1][inst.rs] + inst.immediate;
                break;

          case 35: //LW (opcode = 100011) 
                timeline.push_back("C#" + std::to_string(cycle) + " I" + std::to_string(instruction) + "-EX");
                cycle++;
                timeline.push_back("C#" + std::to_string(cycle) + " I" + std::to_string(instruction) + "-MEM");
                cycle++;
                timeline.push_back("C#" + std::to_string(cycle) + " I" + std::to_string(instruction) + "-WB");
                
                //rd = rs + imm;
                registers[0][inst.rt] = 1; //Mark register as in use.
                registers[1][inst.rt] = registers[1][inst.rs] + inst.immediate;
                offset = inst.immediate;
                effAdd = offset + registers[1][inst.rs];
                if (effAdd % 4 != 0) {
                    std::cerr << "Unaligned memory access at address: " << effAdd << std::endl;
                    return;
                }
                registers[1][inst.rt] = memory[1][(effAdd/4)];
                break;

            	case 43: //SW (opcode = 101011)
                timeline.push_back("C#" + std::to_string(cycle) + " I" + std::to_string(instruction) + "-EX");
                cycle++;
                timeline.push_back("C#" + std::to_string(cycle) + " I" + std::to_string(instruction) + "-MEM");

                //rd = rs + imm;
                registers[0][inst.rt] = 1; //Mark register as in use.

                offset = inst.immediate;
                effAdd = offset + registers[1][inst.rs];


                if (effAdd % 4 != 0) {
                    std::cerr << "Unaligned memory access at address: " << effAdd << std::endl;
                    return;
                }
                memory[0][(effAdd/4)] = 1; // Mark memory as in use.
                memory[1][(effAdd/4)] = registers[1][inst.rt]; //Assign memory to location
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
    for (int i=1; i<32; i++){
        if (registers[0][i] == 1) {
            std::cout << "R" << i << " " << registers[1][i] << "\n";
            output << "R" << i << " " << registers[1][i] << "\n";
        }
    }


    // Print final memory contents
    std::cout << "\nMEMORY\n";
    output << "MEMORY\n";
    for (int i=1; i<250; i++){
        if (memory[0][i] == 1) {
            std::cout << i*4 << " " << memory[1][i] << "\n";
            output << i*4 << " " << memory[1][i] << "\n";
        }
    }

    // Just print out the decoded instructions for now
    /*
    for (const auto &inst : instructions){
    	std::cout << "I" << inst.index << " - Type: " << inst.type << ", Opcode: " << inst.opcode << "\n";
    }
    */

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

        // Step 3: Run simulation logic
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
