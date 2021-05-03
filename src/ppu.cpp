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
    key_map = init_keymap();
    fontset = init_fontset();
    renderer = SDL_CreateRenderer(window, -1, 0);
    alive = true;
    verify();
}

PPU::~PPU() {
    delete[] screen, window, renderer;
}

void PPU::processEvent(){
    const uint8_t * keystroke = SDL_GetKeyboardState(NULL);

    for(int i = 0; i < key_map.size(); i++){
        if(keystroke[key_map[i]]){
            if(!key[i]){
                key[i] = true;
            }
        }
    }
    poll();
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

void PPU::poll(){
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
            alive = false;
            destroy();
        }
    }
}

void PPU::draw_sprite(uint8_t x, uint8_t y, uint8_t h, uint8_t w){
}

auto PPU::input() -> uint8_t {
    std::cout << "Enter a key." << std::endl;

    while(true){
        const uint8_t * keystroke = SDL_GetKeyboardState(NULL);
        for(int i = 0; i < key_map.size(); i++){
            if(keystroke[key_map[i]]){
                std::cout << "Pressed : " << i << std::endl;
                return i;
            }
        }
        poll();
    }
}

auto PPU::init_keymap() ->  std::vector<uint8_t>{
    return
    {
        SDL_SCANCODE_1,
        SDL_SCANCODE_2,
        SDL_SCANCODE_3,
        SDL_SCANCODE_4,
        SDL_SCANCODE_Q,
        SDL_SCANCODE_W,
        SDL_SCANCODE_E,
        SDL_SCANCODE_R,
        SDL_SCANCODE_A,
        SDL_SCANCODE_S,
        SDL_SCANCODE_D,
        SDL_SCANCODE_F,
        SDL_SCANCODE_Z,
        SDL_SCANCODE_X,
        SDL_SCANCODE_C,
        SDL_SCANCODE_V
    };
}

auto PPU::init_fontset() -> std::vector<uint8_t>{
    return
        {
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };
}