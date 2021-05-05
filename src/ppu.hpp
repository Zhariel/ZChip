#include <iostream>
#include <functional>
#include <map>
#include <SDL2/SDL.h>

class PPU{
    public:
    std::vector<uint8_t> key_map;
    unsigned char screen[64 * 32]{};
    std::vector<uint8_t> fontset;
    bool key[16]{false};
    SDL_Window *window;
    SDL_Surface *surface;
    SDL_Renderer *renderer;
    bool alive;
    SDL_Event event;

    PPU();
    ~PPU();
    std::vector<uint8_t> init_keymap();
    std::vector<uint8_t> init_fontset();
    void processEvent();
    void poll();
    uint8_t input();
    void draw_sprite(uint8_t memory[4096], uint16_t x, uint16_t y, uint16_t location, uint16_t N);
    void drawSquare(uint16_t x, uint16_t y);
    void cls(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void verify();
    void destroy();
};