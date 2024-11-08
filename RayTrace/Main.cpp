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
#include "Plane.h"
#include "Triangle.h"

#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include <iostream>

void InitScene01(Scene& scene, Camera& camera);

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

    //InitScene01(scene, camera);

    auto white = std::make_shared<Lambertian>(color3_t{ 1, 1, 1 });
    auto red = std::make_shared<Lambertian>(color3_t{ 1, 0, 0 });
    auto green = std::make_shared<Lambertian>(color3_t{ 0, 1, 0 });
    auto metal = std::make_shared<Metal>(color3_t{ 0, 1, 0 }, 0);

    std::shared_ptr<Material> gray = std::make_shared<Lambertian>(color3_t{ 0.5f });
    auto plane = std::make_unique<Plane>(Transform{ glm::vec3{ 0, -2, 0 }, glm::vec3{ 0, 0, 0 } }, gray);
    scene.AddObject(std::move(plane));

    scene.Update();
    scene.Render(framebuffer, camera, 20, 5);
    framebuffer.Update();

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
        /*framebuffer.Clear(ColorConvert(color4_t{ 0, 0, 0, 1 }));*/

        renderer = framebuffer;

        // show screen
        SDL_RenderPresent(renderer.m_renderer);
    }

    return 0;
}

void InitScene01(Scene& scene, Camera& camera)
{
    camera.SetFOV(20.0f);
    camera.SetView({ 13, 2, 3 }, { 0, 0, 0 });

    auto ground_material = std::make_shared<Lambertian>(color3_t(0.5f));
    scene.AddObject(std::make_unique<Plane>(Transform{ glm::vec3{ 0 } }, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = randomf();
            glm::vec3 center(a + 0.9 * randomf(), 0.2, b + 0.9 * randomf());

            if ((center - glm::vec3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = HSVtoRGB(randomf(0, 360), 1.0f, 1.0f);
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    scene.AddObject(std::make_unique<Sphere>(Transform{ center }, 0.2f, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = HSVtoRGB(randomf(0, 360), 1.0f, 1.0f);
                    auto fuzz = randomf(0.0f, 0.5f);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    scene.AddObject(std::make_unique<Sphere>(Transform{ center }, 0.2f, sphere_material));
                }
                else {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(color3_t{ 1 }, 1.5f);
                    scene.AddObject(std::make_unique<Sphere>(Transform{ center }, 0.2f, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(color3_t{ 1 }, 1.5f);
    scene.AddObject(std::make_unique<Sphere>(Transform{ glm::vec3{ 0, 1, 0 } }, 1.0f, material1));

    auto material2 = std::make_shared<Lambertian>(color3_t(0.4f, 0.2f, 0.1f));
    scene.AddObject(std::make_unique<Sphere>(Transform{ glm::vec3{ -4, 1, 0 } }, 1.0f, material2));

    auto material3 = std::make_shared<Metal>(color3_t(0.7f, 0.6f, 0.5f), 0.0f);
    scene.AddObject(std::make_unique<Sphere>(Transform{ glm::vec3{ 4, 1, 0 } }, 1.0f, material3));
}