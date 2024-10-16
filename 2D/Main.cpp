#include "Renderer.h"
#include "Framebuffer.h"
#include "MathUtils.h"
#include "Image.h"
#include "PostProcess.h"
#include "Model.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

    Image img1 = Image();
    img1.Load("LebronFailedBeefJump.png");

    Image img2 = Image();
    img2.Load("GangsterMario.png");

    Image imgAlpha = Image();
    imgAlpha.Load("colors.png");
    PostProcess::Alpha(imgAlpha.m_buffer, 128);

    vertices_t vertices = { { -5, 5, 0 }, { 5, 5, 0 }, { -5, -5, 0 } };
    Model model(vertices, { 0, 255, 0, 255 });

    SetBlendMode(BlendMode::NORMAL);

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

        int ticks = SDL_GetTicks();
        float time = ticks * 0.001f;
        float t = std::abs(std::sin(time));

#pragma region alpha_blending

        /*SetBlendMode(BlendMode::NORMAL);
        framebuffer.DrawImage(100, 100, img1);
        framebuffer.DrawImage(500, 100, img2);

        SetBlendMode(BlendMode::ALPHA);
        framebuffer.DrawImage(mx, my, imgAlpha);*/

#pragma endregion

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        glm::mat4 translate = glm::translate(modelMatrix, glm::vec3(240.0f, 240.0f, 0.0f));
        glm::mat4 scale = glm::scale(modelMatrix, glm::vec3(5));
        glm::mat4 rotate = glm::rotate(modelMatrix, glm::radians(time * 90), glm::vec3{ 0, 0, 1 });

        modelMatrix = translate * scale * rotate;

        model.Draw(framebuffer, modelMatrix);

        framebuffer.Update();

        renderer = framebuffer;

        // show screen
        SDL_RenderPresent(renderer.m_renderer);
    }

    return 0;
}