#include "SDL.h"
#include "iostream"
#include "Renderer.h"
#include "Framebuffer.h"
#include "Image.h"
#include "Input.h"
#include "Actor.h"
#include "Scene.h"
#include "Random.h"
#include "Tracer.h"
#include "Plane.h"
#include "PostProcess.h"
#include "MathUtils.h"
#include "Model.h"
#include "Transform.h"
#include "ETime.h"
#include "Camera.h"
#include <memory>
#include <glm/gtc/matrix_transform.hpp>


void InitScene(Scene& scene);
void InitScene01(Scene& scene, Camera& camera);

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));

	std::string img = "C:/Users/jrowe/source/repos/GAT350/Build/Photos/";
	class Renderer r;
	
	// initialize SDL
	Time time;
	Input input;

	input.Initialize();
	r.Init_SDL();

	// create window
	// returns pointer to window if successful or nullptr if failed
	r.Create_Window("RayTracer", 800, 600);

	SetBlendMode(BlendMode::Normal);

	Framebuffer framebuffer(r, r.m_width, r.m_height);

	Camera cam{ 70.0f, framebuffer.m_width / (float)framebuffer.m_height };

	//cam.SetView({ 5, 5, -70 }, { 0, 0, 0 });
	
	Scene scene;
	
	//InitScene(scene);
	InitScene01(scene, cam);
	
	scene.Render(framebuffer, cam, 200, 6);

	framebuffer.Update();

	bool quit = false;
	while (!quit) //main loop
	{
		time.Tick();
		input.Update();
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = true;
			}
		}

		r.CopyFrameBuffer(framebuffer);
		//show screen
		SDL_RenderPresent(r.m_renderer);
	
	}
	return 0;
}

void InitScene(Scene& scene)
{
	//scene.SetSky(HSVtoRGB(300.0f, 240, 0.2f), HSVtoRGB(300.0f, 240, 1));
	// materials
	std::array<std::shared_ptr<Material>, 3> palette;
	std::shared_ptr<Material> white = std::make_shared<Dielectric>(color3_t{ 1 }, 1.33f);
	std::shared_ptr<Material> red = std::make_shared<Emissive>(color3_t{ 1, 0, 0 }, 1.0f);
	std::shared_ptr<Material> green = std::make_shared<Emissive>(color3_t{ 0, 1, 0 }, 1.0f);
	std::shared_ptr<Material> blue = std::make_shared<Emissive>(color3_t{ 0, 0, 1 }, 1000);
	std::shared_ptr<Material> light = std::make_shared<Emissive>(color3_t{ 1, 1, 1 }, 1000);
	std::shared_ptr<Material> plane_material = std::make_shared<Metal>(color3_t{ 1,1,1 }, 0.75f);

	palette[0] = white;
	palette[1] = red;
	palette[2] = blue;

	// plane
	auto plane = std::make_unique<Plane>( Transform(glm::vec3{ 0, -10, 0 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 1 }), plane_material);
	scene.AddObject(std::move(plane));
	
	auto plane2 = std::make_unique<Plane>( Transform(glm::vec3{ 0, 45, 0 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 1 }), plane_material);
	scene.AddObject(std::move(plane2));

	auto wall1 = std::make_unique<Plane>(Transform(glm::vec3{ -10 , 0 , 0 }, glm::vec3{ 0, 0, 100 }, glm::vec3{ 1 }), red);
	scene.AddObject(std::move(wall1));
	
	auto wall2 = std::make_unique<Plane>(Transform(glm::vec3{ 40 , 0 , 0 }, glm::vec3{ 0, 0, -100 }, glm::vec3{ 1 }), green);
	scene.AddObject(std::move(wall2));

	// spheres
	for (int i = 0; i < 4; i++)
	{
		auto spheres = std::make_unique<Sphere>(Transform(random(glm::vec3{ -5 }, glm::vec3{ 20 })), randomf(2.0f, 5.0f), palette.at(random(3)));
		scene.AddObject(std::move(spheres));
	}
	
	vertexbuffer_t verticies = {
	{ glm::vec3(-0.5f, -0.5f, 0.0f) }, // Bottom-left
	{ glm::vec3(0.5f, -0.5f, 0.0f) }, // Bottom-right
	{ glm::vec3(0.5f,  0.5f, 0.0f) }, // Top-right
	{ glm::vec3(-0.5f,  0.5f, 0.0f) }  // Top-left
	};

	//Model model = (Transform(glm::vec3{ 0 }, glm::vec3{ 0 }, glm::vec3{ 1 }), verticies,red );
	
	auto model = std::make_unique<Model>(Transform(glm::vec3{ 40 , 0 , 0 }), verticies, white);
	model->Load("cube.obj");
	scene.AddObject(std::move(model));

	//triangle
	//auto triangle = std::make_unique<Triangle>(glm::vec3{ 0,5,0 }, glm::vec3{ 5,5,0 }, glm::vec3{ 0,0,0 }, blue);
	//scene.AddObject(std::move(triangle));
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