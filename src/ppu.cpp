#include "ppu.hpp"
#include <SDL2/SDL.h>

PPU::PPU(){
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(
            "ZChip",                  // window title
            SDL_WINDOWPOS_UNDEFINED,           // initial x position
            SDL_WINDOWPOS_UNDEFINED,           // initial y position
            640,                               // width, in pixels
            320,                               // height, in pixels
            SDL_WINDOW_OPENGL                  // flags - see below
    );
    renderer = SDL_CreateRenderer(window, -1, 0);
    alive = true;
    verify();
}

PPU::~PPU(){
    delete[] screen, window, renderer;
}

void PPU::processEvent(){
    while(SDL_PollEvent(&event)){
//        printf("%u\n", event.key.keysym.scancode);
        if(event.type == SDL_QUIT){
            alive = false;
            destroy();
        }
        else if(event.type == SDL_KEYDOWN){
            printf("down");
        }
        else if(event.type == SDL_KEYUP){
            printf("up");
        }
    }
}

void PPU::cls(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void PPU::destroy(){
    SDL_Delay(3000);
    SDL_DestroyWindow(window);
    SDL_Quit();
};

void PPU::verify(){
    if (window == nullptr) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
    }
}