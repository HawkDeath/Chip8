#include <iostream>
#include <thread>
#include <SDL.h>
#undef main
#include "Chip8.h"

int main(int argc, char * argv[])
{
    if (argc < 2) {
        std::cerr << "missing rom file\n";
        return 1;
    }
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, chip8::Width * 4, chip8::Height * 6, SDL_WINDOW_RESIZABLE);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, chip8::Width, chip8::Height);
   
    bool quit = false;
    try {
    chip8::Chip8 chipApp;
    chipApp.loadRomToMemory(argv[1]);


    while (!quit)
    {
        chipApp.tick();
        Uint32 screenTexture[chip8::Width * chip8::Height];// = { 0 };

        chipApp.display(screenTexture);

        SDL_UpdateTexture(texture, nullptr, screenTexture, 4 * chip8::Width);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);

        SDL_Delay(1000 / 60);
       // std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    }
    catch (std::exception & e) {
        std::cerr << e.what() << '\n';
    }


    SDL_DestroyWindow(window);
    SDL_Quit();
    system("pause");
    return 0;
}