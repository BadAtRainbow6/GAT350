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
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[])
{
    Renderer renderer = Renderer();

    std::shared_ptr<Model> tableModel = std::make_shared<Model>();
    tableModel->Load("table.obj");

    std::shared_ptr<Model> appleModel = std::make_shared<Model>();
    appleModel->Load("apple.obj");

    std::shared_ptr<Model> plateModel = std::make_shared<Model>();
    plateModel->Load("plate.obj");

    Image background;
    background.Load("room.png");

    if (!renderer.Initialize()) {
        printf("Error when initializing.");
        return 1;
    }

    if (!renderer.CreateWindow(800, 600, "Home")) {
        printf("Error when creating window.");
        return 1;
    }

    Camera camera(renderer.m_width, renderer.m_height);
    camera.SetProjection(60.0f, 800.0f / 600.0f, 0.1f, 200.0f);
    Transform cameraTransform{ { 0, 0, -20} };

    Time time;
    Input input;

    input.Initialize();

    Framebuffer framebuffer(renderer, 800, 600);

    std::vector<std::unique_ptr<Actor>> actors;

	Transform tableTransform{ glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0.2f } };
	std::unique_ptr<Actor> tableActor = std::make_unique<Actor>(tableTransform, tableModel);
	tableActor->SetColor(color_t{ 150, 75, 0, 255 });
	actors.push_back(std::move(tableActor));

    Transform plateTransform{ glm::vec3{ 0, 11.0f, 0 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 2.0f } };
    std::unique_ptr<Actor> plateActor = std::make_unique<Actor>(plateTransform, plateModel);
    plateActor->SetColor(color_t{ 200, 200, 200, 255 });
    actors.push_back(std::move(plateActor));

    Transform appleTransform{ glm::vec3{ 0.0f, 14.0f, 0.0f }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 2.0f } };
    std::unique_ptr<Actor> appleActor = std::make_unique<Actor>(appleTransform, appleModel);
    appleActor->SetColor(color_t{ 255, 0, 0, 255 });
    actors.push_back(std::move(appleActor));

    SetBlendMode(BlendMode::NORMAL);

    bool quit = false;
    while (!quit) {
        time.Tick();
        input.Update();

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


        //framebuffer.DrawLine(20, 30, 20, 30, color_t{ 255, 255, 255, 255 });
        /*
        framebuffer.DrawTriangle(40, 50, 60, 40, 50, 40, color_t{255, 255, 255, 255});

        framebuffer.DrawCircle(100, 50, 20, color_t{ 255, 255, 255, 255 });*/

        int mx;
        int my;
        SDL_GetMouseState(&mx, &my);

        /*
        framebuffer.DrawLinearCurve(200, 300, 100, 200, color_t{ 255, 255, 255, 255 });
        framebuffer.DrawQuadraticCurve(100, mx, 400, 100, my, 100, color_t{ 255, 255, 255, 255 });
        framebuffer.DrawCubicCurve(200, 200, mx, 600, 200, 100, my, 400, color_t{ 255, 255, 255, 255 });

        int x;
        int y;
        CubicPoint(200, 200, mx, 600, 200, 100, my, 400, t, x, y);
        framebuffer.DrawRect(x - 20, y - 20, 40, 40, color_t{ 255, 255, 255, 255 });*/

#pragma region alpha_blending

        /*SetBlendMode(BlendMode::NORMAL);
        framebuffer.DrawImage(100, 100, img1);
        framebuffer.DrawImage(500, 100, img2);

        SetBlendMode(BlendMode::ALPHA);
        framebuffer.DrawImage(mx, my, imgAlpha);*/

#pragma endregion

        /*glm::mat4 modelMatrix = glm::mat4(1.0f);
        glm::mat4 translate = glm::translate(modelMatrix, glm::vec3(240.0f, 240.0f, 0.0f));
        glm::mat4 scale = glm::scale(modelMatrix, glm::vec3(5));
        glm::mat4 rotate = glm::rotate(modelMatrix, glm::radians(time * 90), glm::vec3{ 0, 0, 1 });

        modelMatrix = translate * scale * rotate;

        model.Draw(framebuffer, modelMatrix);*/

        if (input.GetMouseButtonDown(2)) 
        {
            input.SetMouseRelative(true);

            glm::vec3 direction{ 0 };
            if (input.GetKeyDown(SDL_SCANCODE_D))   direction.x = 1;
            if (input.GetKeyDown(SDL_SCANCODE_A))   direction.x = -1;

            if (input.GetKeyDown(SDL_SCANCODE_W))   direction.y = 1;
            if (input.GetKeyDown(SDL_SCANCODE_S))   direction.y = -1;

            if (input.GetKeyDown(SDL_SCANCODE_E))   direction.z = 1;
            if (input.GetKeyDown(SDL_SCANCODE_Q))   direction.z = -1;

            cameraTransform.rotation.y += input.GetMouseRelative().x * 0.25f;
            cameraTransform.rotation.x = Clamp((cameraTransform.rotation.x + input.GetMouseRelative().y * 0.25f), -89.0f, 89.0f);

            glm::vec3 offset = cameraTransform.GetMatrix() * glm::vec4{ direction, 0 };

            cameraTransform.position += offset * 70.0f * time.GetDeltaTime();
        }
        else
        {
            input.SetMouseRelative(false);
        }
        
        camera.SetView(cameraTransform.position, cameraTransform.position + cameraTransform.GetForward());

        framebuffer.DrawImage(0, 0, background);

        for (auto& actor : actors) 
        {
            actor->Draw(framebuffer, camera);
        }

        framebuffer.Update();

        renderer = framebuffer;

        // show screen
        SDL_RenderPresent(renderer.m_renderer);
    }

    return 0;
}