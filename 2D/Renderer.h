#pragma once

class Renderer 
{
public:
	bool Initialize();
	bool CreateWindow(int width, int height, const char *name);
};