#ifndef CHIP8_H
#define CHIP8_H

#include <array>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>

// DOCS:
// http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

namespace chip8 {
    const std::int32_t MAX_MEMORY_SIZE = 4096; // 0x000 - 0xFFF (4 KB)
    const std::int32_t START_OF_CHIP_PROGRAM = 0x200; // 512
    const std::int16_t STACK_SIZE = 16;
    using Instruction = std::uint16_t;

    class Chip8
    {
    public:
        Chip8();
        ~Chip8() = default;

        void loadRom(const std::string& loadFile);
        void tick();
    private:
        std::vector<Instruction> decodeRomInstructions(std::vector<std::uint8_t>& romData);
        void loadRomToMemory(std::vector<Instruction> & insturtions);

    private:
        std::array<std::uint16_t, MAX_MEMORY_SIZE> memory;
        std::array<std::uint16_t, STACK_SIZE> stack;
        std::uint16_t I;
        std::uint16_t delayTimer;
        std::uint16_t soundTimer;
        std::uint16_t *pc;

    };
}

#endif // !CHIP8_H
