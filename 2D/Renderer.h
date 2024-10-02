#pragma once
#include <SDL.h>

class Renderer 
{
public:
	bool Initialize();
	bool CreateWindow(int width, int height, const char *name);
	void CopyFramebuffer(const class Framebuffer& framebuffer);

	void operator = (const class Framebuffer& framebuffer);

public:
	SDL_Renderer* m_renderer;
	SDL_Window* m_window;
};