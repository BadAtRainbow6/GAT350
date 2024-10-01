#include "Renderer.h"
#include <SDL.h>

bool Renderer::Initialize()
{
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        return false;
    }
    return true;
}

bool Renderer::CreateWindow(int width, int height, const char *name)
{
    // create window
    // returns pointer to window if successful or nullptr if failed
    SDL_Window* window = SDL_CreateWindow(name,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        SDL_Quit();
        return false;
    }

    // create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    while (true)
    {
        // clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        // show screen
        SDL_RenderPresent(renderer);
    }

    return true;
}
