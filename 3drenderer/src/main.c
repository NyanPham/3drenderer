#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>

uint8_t is_running = 0;
SDL_Window* g_window = NULL;
SDL_Renderer* g_renderer = NULL;

uint32_t* g_color_buffer = NULL;
SDL_Texture * g_color_buffer_texture = NULL;

int window_width = 800;
int window_height = 600;

int initialize_window(void) {
    
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return -1;
    }

    // Use SDL to query what the fullscreen max. width and height are
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    window_width = display_mode.w;
    window_height = display_mode.h;

    // Create an SDL Window
    g_window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED, // posX
        SDL_WINDOWPOS_CENTERED, // poxY
        window_width,           // width
        window_height,          // height
        SDL_WINDOW_BORDERLESS
    );

    if (!g_window) {
        fprintf(stderr, "Error creating the SDL window.\n");
        return -1;
    }
    
    // Create an SDL renderer
    g_renderer = SDL_CreateRenderer(g_window, -1, 0);

    if (!g_renderer) {
        fprintf(stderr, "Error creating the SDL Renderer.\n");
        return -1;
    }

    SDL_SetWindowFullscreen(g_window, SDL_WINDOW_FULLSCREEN);

    return 0;
    
}

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

void draw_grid(void) {
    for (int y = 0; y < window_height; y++) {
        for (int x = 0; x < window_width; x++) {
            if ((x % 10) == 0 || (y % 10) == 0) {
                g_color_buffer[(window_width * y) + x] = 0xFF333333;
            }
        }
    }
}

void draw_rect(int x, int y, int w, int h, uint32_t color) {
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            g_color_buffer[(j + y) * window_width + (i + x)] = color;
        }
    }
}

void clear_color_buffer(uint32_t color) {
    for (int y = 0; y < window_height; y++) {
        for (int x = 0; x < window_width; x++) {
            g_color_buffer[(window_width * y) + x] = color;
        }
    }
}

void render_color_buffer(void) {
    SDL_UpdateTexture(
        g_color_buffer_texture,
        NULL,
        g_color_buffer,
        (int)(window_width * sizeof(uint32_t))
    );
    SDL_RenderCopy(g_renderer, g_color_buffer_texture, NULL, NULL); 
}

void render(void) {
    SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255);
    SDL_RenderClear(g_renderer);

    // draw_grid();
    draw_rect(60, 130, 60, 130, 0xFFFFFFFF);

    render_color_buffer();
    clear_color_buffer(0xFF000000);
    SDL_RenderPresent(g_renderer);
}

void destroy_window(void) {
    free(g_color_buffer);
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    SDL_Quit();
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
