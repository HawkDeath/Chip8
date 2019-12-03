#include <iostream>
#include "Chip8.h"

int main(int argc, char * argv[])
{
    if (argc < 2) {
        std::cerr << "missing rom file\n";
        return 1;
    }

    chip8::Chip8 chipApp;
    
    try {
        chipApp.loadRom(argv[1]);
        chipApp.tick();
    }
    catch (std::exception & e) {
        std::cerr << e.what() << '\n';
    }
    system("pause");
    return 0;
}