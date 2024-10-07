#include "Renderer.h"
#include "Framebuffer.h"
#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[])
{
    Renderer renderer = Renderer();

    if (!renderer.Initialize()) {
        printf("Error when initializing.");
        return 1;
    }

    if (!renderer.CreateWindow(800, 600, "Home")) {
        printf("Error when creating window.");
        return 1;
    }

    Framebuffer framebuffer(renderer, 200, 150);

    bool quit = false;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }
        }

        // clear screen
        framebuffer.Clear(color_t{ 0, 0, 0, 255 });

        framebuffer.DrawLine(20, 30, 20, 30, color_t{ 255, 255, 255, 255 });

        framebuffer.DrawTriangle(40, 50, 60, 40, 50, 40, color_t{ 255, 255, 255, 255 });

        framebuffer.DrawCircle(100, 50, 20, color_t{ 255, 255, 255, 255 });

        framebuffer.Update();

        renderer = framebuffer;

        // show screen
        SDL_RenderPresent(renderer.m_renderer);
    }

    return 0;
}