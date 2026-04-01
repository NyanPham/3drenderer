#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include "display.h"

uint8_t is_running = 0;


void setup(void) {
    // allocate required mem in bytes to hold the color buffer
    g_color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
    
    g_color_buffer_texture = SDL_CreateTexture(
        g_renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height
    );
}

void process_input(void) {
    SDL_Event event;

    SDL_PollEvent(&event);
    
    switch(event.type) {
        case SDL_QUIT:
            is_running = 0;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                is_running = 0;
            }
            break;
        default:
            break;
    }
}

void update(void) {}

void render(void) {
    SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255);
    SDL_RenderClear(g_renderer);

    // draw_grid();
    draw_rect(60, 130, 60, 130, 0xFFFFFFFF);

    render_color_buffer();
    clear_color_buffer(0xFF000000);
    SDL_RenderPresent(g_renderer);
}

int main(void) {
    is_running = initialize_window() == 0;
    
    setup();

    while (is_running) {
        process_input();
        update();
        render();
    }
    
    destroy_window();

    return 0;
}
