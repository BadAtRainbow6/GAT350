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

        for (int i = 0; i < 20; i++)
        {
            int x1 = rand() % 200;
            int x2 = rand() % 200;
            int x3 = rand() % 200;
            int y1 = rand() % 150;
            int y2 = rand() % 150;
            int y3 = rand() % 150;

            framebuffer.DrawTriangle(x1, x2, x3, y1, y2, y3, color_t{ 255, 255, 255, 255 });
        }

        framebuffer.Update();

        renderer = framebuffer;

        // show screen
        SDL_RenderPresent(renderer.m_renderer);
    }

    return 0;
}