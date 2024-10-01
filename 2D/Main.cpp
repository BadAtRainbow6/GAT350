#include "Renderer.h"
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

    return 0;
}