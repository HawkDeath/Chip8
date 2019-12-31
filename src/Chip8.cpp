#include "Chip8.h"


namespace chip8 {

    Chip8::Chip8() : I(0), delayTimer(0), soundTimer(0), pc(0) {
        initializeFonts();
    }

    void Chip8::initializeFonts() {
        fonts[0] = 0xF0; fonts[1] = 0x90; fonts[2] = 0x90; fonts[3] = 0x90; fonts[4] = 0xF0; // 0
        fonts[5] = 0x20; fonts[6] = 0x60; fonts[7] = 0x20; fonts[8] = 0x20; fonts[9] = 0x70; // 1
        fonts[10] = 0xF0; fonts[11] = 0x10; fonts[12] = 0xF0; fonts[13] = 0x80; fonts[14] = 0xF0; // 2
        fonts[15] = 0xF0; fonts[16] = 0x10; fonts[17] = 0xF0; fonts[18] = 0x10; fonts[19] = 0xF0; // 3
        fonts[20] = 0x90; fonts[21] = 0x90; fonts[22] = 0xF0; fonts[23] = 0x10; fonts[24] = 0x10; // 4
        fonts[25] = 0xF0; fonts[26] = 0x80; fonts[27] = 0xF0; fonts[28] = 0x10; fonts[29] = 0xF0; // 5
        fonts[30] = 0xF0; fonts[31] = 0x80; fonts[32] = 0xF0; fonts[33] = 0x90; fonts[34] = 0xF0; // 6
        fonts[35] = 0xF0; fonts[36] = 0x10; fonts[37] = 0x20; fonts[38] = 0x40; fonts[39] = 0x40; // 7
        fonts[40] = 0xF0; fonts[41] = 0x90; fonts[42] = 0xF0; fonts[43] = 0x90; fonts[44] = 0xF0; // 8
        fonts[45] = 0xF0; fonts[46] = 0x90; fonts[47] = 0xF0; fonts[48] = 0x10; fonts[49] = 0xF0; // 9
        fonts[50] = 0xF0; fonts[51] = 0x90; fonts[52] = 0xF0; fonts[53] = 0x90; fonts[54] = 0x90; // A
        fonts[55] = 0xE0; fonts[56] = 0x90; fonts[57] = 0xE0; fonts[58] = 0x90; fonts[59] = 0xE0; // B
        fonts[60] = 0xF0; fonts[61] = 0x80; fonts[62] = 0x80; fonts[63] = 0x80; fonts[64] = 0xF0; // C
        fonts[65] = 0xE0; fonts[66] = 0x90; fonts[67] = 0x90; fonts[68] = 0x90; fonts[69] = 0xE0; // D
        fonts[70] = 0xF0; fonts[71] = 0x80; fonts[72] = 0xF0; fonts[73] = 0x80; fonts[74] = 0xF0; // E
        fonts[75] = 0xF0; fonts[76] = 0x80; fonts[77] = 0xF0; fonts[78] = 0x80; fonts[79] = 0x80; // F
    }

    void Chip8::loadRomToMemory(const std::string& loadFile, std::uint32_t memoryPosition) {
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

       for (auto& data : romData) {
            memory[memoryPosition++ & 0xFFF] = data;
       }

        romData.clear();
    }

    void Chip8::tick() {
        Instruction opcode = memory[pc & 0xFFF] * 0x100 + memory[(pc + 1) & 0xFFF];
        pc += 2;

        Instruction numberOfInstruction = (opcode >> 12) & 0xF; // sample 1nnn -> numberOfInstruction=0x1
        Instruction variantOfInstruction = (opcode >> 0) & 0xF; // sample 8xy3 -> variantOfInstruction=0x3
        Instruction y = (opcode >> 4) & 0xF;
        Instruction x = (opcode >> 8) & 0xF;
        Instruction kk = (opcode >> 0) & 0xFF;
        Instruction nnn = (opcode >> 0) & 0xFFF;

        auto& Vx = V[x];
        auto& Vy = V[y];
        auto& VF = V[0xF];
        std::printf("pc=%04x\n", pc);
        std::printf("Instruction %04x x=%04x y=%04x kk=%04x nnn=%04x\n", numberOfInstruction,x, y, kk, nnn);
        std::printf("Vx=%04x Vy=%04x VF=%04x I=%04x varInst=%04x\n", Vx, Vy, VF, I, variantOfInstruction);
        switch (numberOfInstruction) {
        case 0x0:
            if (nnn == 0xE0) { // CLS
                for (auto& c : displayMemory) c = 0;
            }
            else if (nnn == 0xEE) { // ret
                pc = stack[stackPointer-- % STACK_SIZE]; 
            }
            break;
        case 0x1: // JP addr
            pc = nnn;
            break;
        case 0x2: // CALL addr
            stack[++stackPointer % 12] = pc;
            pc = nnn;
            break;
        case 0x3: // SE Vx, byte (kk)
            if (Vx == kk) {
                pc += 2;
            }
            break;
        case 0x4: //  SNE Vx, byte (kk)
            if (Vx != kk) {
                pc += 2;
            }
            break;
        case 0x5:
            if (variantOfInstruction == 0x0) { // SE Vx, Vy
                if (Vx == Vy) {
                    pc += 2;
                }
            }
            else {
                std::string errorMessage = "[opcode: 0x5] " + std::to_string(variantOfInstruction) + " is unknow variant";
                throw std::runtime_error(errorMessage);
            }
            break;
        case 0x6: // LD Vx, byte (kk)
            Vx = kk;
            break;
        case 0x7: // ADD Vx, byte (kk)
            Vx += kk;
            break;
        case 0x8:
            switch (variantOfInstruction)
            {
            case 0x0: // LD Vx, Vy
                Vx = Vy;
                break;
            case 0x1: // OR Vx, Vy
                Vx |= Vy;
                break;
            case 0x2: // AND Vx, Vy
                Vx &= Vy;
                break;
            case 0x3: // XOR Vx, Vy
                Vx ^= Vy;
                break;
            case 0x4:// ADD Vx, Vy
                variantOfInstruction = Vx + Vy; // I don't want to declare a new value
                VF = (variantOfInstruction >> 8); // carry
                Vx = variantOfInstruction;
                break;
            case 0x5: // SUB Vx, Vy
                variantOfInstruction = Vx - Vy; // I don't want to declare a new value
                VF = !(variantOfInstruction >> 8); // not carry
                Vx = variantOfInstruction;
                break;
            case 0x6: // SHR Vx {, Vy}
                VF = Vy & 1;
                Vx = Vy >> 1;
                break;
            case 0x7: // SUBN Vx, Vy
                variantOfInstruction = Vy - Vx; // I don't want to declare a new value
                VF = !(variantOfInstruction >> 8);
                Vx = variantOfInstruction;
                break;
            case 0xE: // SHL Vx {, Vy}
                VF = Vy >> 7;
                Vx = Vy << 1;
                break;
            default: {
                std::string errorMessage = "[opcode: 0x8] " + std::to_string(variantOfInstruction) + " is unknow variant";
                throw std::runtime_error(errorMessage);
            }
            }
            break;
        case 0x9:
            if (variantOfInstruction == 0x0) { // SNE Vx, Vy
                if (Vx != Vy) {
                    pc += 2;
                }
            }
            else {
                std::string errorMessage = "[opcode: 0x9] " + std::to_string(variantOfInstruction) + " is unknow variant";
                throw std::runtime_error(errorMessage);
            }
            break;
        case 0xA: // LD I, addr
            I = nnn;
            break;
        case 0xB: // JP V0, addr
            pc = (nnn + V[0]);
            break;
        case 0xC: // RND Vx, byte
            Vx = (std::uniform_int_distribution<>(0, 255)(randomMachine) & kk);
            break;
        case 0xD: {// DRW Vx, Vy, nibble
            auto putPixel = [this](std::int32_t idx, std::uint8_t value) {
                return ((displayMemory[idx] ^= value) ^ value) & value;
            };
            for (kk = 0, x = Vx, y = Vy; variantOfInstruction--; )
                kk |= putPixel( ((x + 0) % Width + (y + variantOfInstruction) % Height * Width) / 8, memory[(I + variantOfInstruction) & 0xFFF] >> (x % 8))
                    | putPixel( ((x + 7) % Width + (y + variantOfInstruction) % Height * Width) / 8, memory[(I + variantOfInstruction) & 0xFFF] << (8 - x % 8));
            VF = (kk != 0);
        } break;
        case 0xE:
            switch (kk)
            {
            case 0x9E: // SKP Vx
                if (keys[Vx & 15]) {
                    pc += 2;
                }
                break;
            case 0xA1: // SKNP Vx
                if (!keys[Vx & 15]) {
                    pc += 2;
                }
                break;
            default: {
                std::string errorMessage = "[opcode: 0xE] " + std::to_string(kk) + " is unknow";
                throw std::runtime_error(errorMessage);
            }
            }
            break;
        case 0xF:
            switch (kk)
            {
            case 0x07: // LD Vx, DT
                Vx = delayTimer;
                break;
            case 0x0A: // LD Vx, K (wait for a key press)
                waitingKey = 0x80 | x;
                break;
            case 0x15: // LD DT, Vx
                delayTimer = Vx;
                break;
            case 0x18: // LD ST, Vx
                soundTimer = Vx;
                break;
            case 0x1E: // ADD I, Vx
                variantOfInstruction = (I & 0xFFF) + Vx;
                VF = variantOfInstruction >> 12;
                I = variantOfInstruction;
                break;
            case 0x29: // LD F, Vx
                I = &fonts[ (Vx & 15) * 5] - memory; // get char form font ( '- memory' means offset = 0)
                break;
            case 0x33: // LD B, Vx
                memory[(I + 0) & 0xFFF] = (Vx / 100) % 10;
                memory[(I + 1) & 0xFFF] = (Vx / 10) % 10;
                memory[(I + 2) & 0xFFF] = (Vx / 1) % 10;
                break;
            case 0x55: // LD [I], Vx
                for (variantOfInstruction = 0; variantOfInstruction <= x; ++variantOfInstruction) {
                    memory[I++ & 0XFFF] = V[variantOfInstruction];
                }
                break;
            case 0x65: // LD Vx, [I]
                for (variantOfInstruction = 0; variantOfInstruction <= x; ++variantOfInstruction) {
                    V[variantOfInstruction] = memory[I++ & 0XFFF];
                }
                break;
            default:{
                std::string errorMessage = "[opcode: 0xF] " + std::to_string(kk) + " is unknow";
                throw std::runtime_error(errorMessage);
            }
            }
            break;
        default: {
            std::string errorMessage = "Unknow instruction: " + std::to_string(numberOfInstruction);
            throw std::runtime_error(errorMessage);
        }
        }
    }

    void Chip8::display(Uint32* screen) {
        for (std::uint32_t pixelPosition = 0; pixelPosition < (Width * Height); ++pixelPosition)
            screen[pixelPosition] = 0xFFFFFFF * ((displayMemory[pixelPosition / 8] >> (7 - pixelPosition % 8)) & 1); // monochromatic
    }
}