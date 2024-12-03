#pragma once
#include "Framebuffer.h"
#include <string>
#include <iostream>

class Renderer
{
public:
	virtual int Init_SDL();
	virtual int Create_Window(const char* name, int width, int height);
	
	void CopyFrameBuffer(const Framebuffer& framebuffer);
	void operator = (const Framebuffer& framebuffer);

	SDL_Window* m_window{ nullptr };
	SDL_Renderer* m_renderer { nullptr };

	int m_width{ 0 };
	int m_height{ 0 };
};

