#include "Renderer.h"
#include "Framebuffer.h"
#include "MathUtils.h"
#include "Image.h"
#include "PostProcess.h"
#include "Model.h"
#include "Transform.h"
#include "ETime.h"
#include "Input.h"
#include "Camera.h"
#include "Actor.h"
#include "Random.h"
#include "Scene.h"
#include "Sphere.h"

#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[])
{
    Renderer renderer = Renderer();

    Time time;

    SetBlendMode(BlendMode::NORMAL);

    if (!renderer.Initialize()) {
        printf("Error when initializing.");
        return 1;
    }

    if (!renderer.CreateWindow(800, 600, "Home")) {
        printf("Error when creating window.");
        return 1;
    }

    Framebuffer framebuffer(renderer, 800, 600);

    Camera camera{ 70.0f, framebuffer.m_width / (float)framebuffer.m_height };
    camera.SetView({ 0, 0, -20 }, { 0, 0, 0 });

    Scene scene;

    std::shared_ptr<Material> material = std::make_shared<Material>(color3_t{ 1, 0, 0 });
    std::unique_ptr<Sphere> object = std::make_unique<Sphere>(glm::vec3{ 0 }, 2.0f, material);
    scene.AddObject(std::move(object));

    bool quit = false;
    while (!quit) {
        time.Tick();
        
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
        framebuffer.Clear(ColorConvert(color4_t{ 0, 0, 0, 1 }));

        scene.Render(framebuffer, camera);

        framebuffer.Update();

        renderer = framebuffer;

        // show screen
        SDL_RenderPresent(renderer.m_renderer);
    }

    return 0;
}