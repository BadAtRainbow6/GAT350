#include "Renderer.h"
#include "Framebuffer.h"

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
    m_window = SDL_CreateWindow(name,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_SHOWN);
    if (m_window == nullptr)
    {
        SDL_Quit();
        return false;
    }

    // create renderer
    m_renderer = SDL_CreateRenderer(m_window, -1, 0);

    return true;
}

void Renderer::CopyFramebuffer(const Framebuffer& framebuffer)
{
    SDL_RenderCopy(m_renderer, framebuffer.m_texture, NULL, NULL);
}

void Renderer::operator=(const Framebuffer& framebuffer)
{
    SDL_RenderCopy(m_renderer, framebuffer.m_texture, NULL, NULL);
}
