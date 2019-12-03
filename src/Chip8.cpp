#include "Chip8.h"

namespace chip8 {

    Chip8::Chip8()
    {
        for (auto& i : memory) {
            i = 0;
        }
        pc = nullptr;
    }

    void Chip8::loadRom(const std::string& loadFile)
    {
        std::FILE* romFile = std::fopen(loadFile.c_str(), "rb");
        std::vector<std::uint8_t> romData(MAX_MEMORY_SIZE - 512);

        if (romFile != nullptr) {
            std::size_t romSize = std::fread(romData.data(), 1, romData.size(), romFile);
            romData.resize(romSize);

            std::fclose(romFile);
        }
        else {
            throw std::invalid_argument(loadFile + " rom cannot be loaded");
        }
        
        auto decodedRom = decodeRomInstructions(romData);
        loadRomToMemory(decodedRom);
        romData.clear();
    }

    void Chip8::tick()
    {
        std::printf("PC: %04x\n", *pc);
    }

    std::vector<Instruction> Chip8::decodeRomInstructions(std::vector<std::uint8_t>& romData)
    {
        std::vector<Instruction> data;
        for (std::size_t i = 0; i < romData.size(); i += 2)
        {
            Instruction v1 = (romData[i] << 8);
            Instruction v2 = romData[i + 1];
            Instruction opcode = v1 | v2;
            data.push_back(opcode);
        }
        return data;
    }
    void Chip8::loadRomToMemory(std::vector<Instruction> & instructions)
    {
        if (MAX_MEMORY_SIZE - START_OF_CHIP_PROGRAM > instructions.size()) {
            for (int i = 0; i < instructions.size(); ++i) {
                memory[START_OF_CHIP_PROGRAM + i] = instructions[i];
            }
            pc = &memory[START_OF_CHIP_PROGRAM]; // set program counter
        }
        else {
            throw std::invalid_argument("chip8 program is too big\n");
        }
    }
}