#ifndef CHIP8_H
#define CHIP8_H

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

#include <SDL.h>

// DOCS:
// http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

namespace chip8 {
    const std::int32_t MAX_MEMORY_SIZE = 4096; // 0x000 - 0xFFF (4 KB)
    const std::int32_t START_OF_CHIP_PROGRAM = 0x200; // 512
    const std::int16_t V_REG_SIZE = 16;
    const std::int16_t STACK_SIZE = 12;
    const std::int16_t KEYS_COUNT = 16;
    const std::int32_t FONT_SIZE = 16 * 5;
    const std::int32_t Width = 64;
    const std::int32_t Height = 32;
    const std::int32_t DISPLAY_SIZE = (Width * Height) / 8;

    using Instruction = std::uint16_t;

    class Chip8
    {
    public:
        Chip8();
        ~Chip8() = default;

        void loadRomToMemory(const std::string& loadFile, std::uint32_t memoryPosition = 0x200);
        void tick();
        void display(Uint32* screen);

    private:
        void initializeFonts();

    public:
        std::uint8_t fonts[FONT_SIZE];
        std::uint8_t memory[MAX_MEMORY_SIZE] = { 0x12, 0x00 };
        std::uint8_t displayMemory[DISPLAY_SIZE] = { 0 };
        std::uint8_t V[V_REG_SIZE];
        std::uint8_t keys[KEYS_COUNT];
        
        std::uint8_t delayTimer;
        std::uint8_t soundTimer;
        std::uint8_t stackPointer;
        std::uint8_t waitingKey;

        std::uint16_t stack[STACK_SIZE];
        std::uint16_t pc;
        std::uint16_t I;

        std::mt19937 randomMachine{};
    };
}

#endif // !CHIP8_H
