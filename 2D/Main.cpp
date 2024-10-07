#include "Renderer.h"
#include "Framebuffer.h"
#include "MathUtils.h"
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

    Framebuffer framebuffer(renderer, 800, 600);

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

        /*framebuffer.DrawLine(20, 30, 20, 30, color_t{ 255, 255, 255, 255 });

        framebuffer.DrawTriangle(40, 50, 60, 40, 50, 40, color_t{ 255, 255, 255, 255 });

        framebuffer.DrawCircle(100, 50, 20, color_t{ 255, 255, 255, 255 });*/

        int mx;
        int my;
        SDL_GetMouseState(&mx, &my);

        framebuffer.DrawLinearCurve(200, 300, 100, 200, color_t{ 255, 255, 255, 255 });
        //framebuffer.DrawQuadraticCurve(100, mx, 400, 100, my, 100, color_t{ 255, 255, 255, 255 });
        framebuffer.DrawCubicCurve(200, 200, mx, 600, 200, 100, my, 400, color_t{ 255, 255, 255, 255 });

        int ticks = SDL_GetTicks();
        float time = ticks * 0.001f;
        float t = std::abs(std::sin(time));
        int x;
        int y;
        CubicPoint(200, 200, mx, 600, 200, 100, my, 400, t, x, y);
        framebuffer.DrawRect(x - 20, y - 20, 40, 40, color_t{ 255, 255, 255, 255 });

        framebuffer.Update();

        renderer = framebuffer;

        // show screen
        SDL_RenderPresent(renderer.m_renderer);
    }

    return 0;
}