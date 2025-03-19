#include "Scene2.h"
#include <SDL.h>
#include <SDL_image.h>
#include <MMath.h>
#include <cmath>
#include <iostream>
#include "Entity.h"
using namespace std;

Scene2::Scene2(SDL_Window* sdlWindow_){
	window = sdlWindow_;
	renderer = nullptr;
	planet = nullptr;
	star1 = nullptr;
	star2 = nullptr;
	xAxis = 25.0f;
	yAxis = 40.0f;
	IMG_Init(IMG_INIT_PNG);
}

Scene2::~Scene2(){
	IMG_Quit();
}

void Scene2::initPlanet(Vec3 initVel) {
	planet->acc = Vec3(0.0f, 0.0f, 0.0f);
	planet->pos = Vec3(10.0f, 20.0f, 0.0f);
	planet->vel = initVel;
}

bool Scene2::OnCreate() {
	// Create a project matrix that moves positions from physics/world space 
	// to screen/pixel space
	int w, h;
	SDL_GetWindowSize(window,&w,&h);
	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, -1.0f, 1.0f);
	projectionMatrix = ndc * ortho;

	//Create screen renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		std::cerr << "SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}
	//Initialize renderer color (black)
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	
	// Create the objects that will be rednered on the screen
	planet = new Entity();
	planet->SetImage("textures/planet.png", renderer);
	initPlanet(planet->initVel);

    star1 = new Entity();
	star1->pos = Vec3(5.0f, 10.0f, 0.0f);
    star1->mass = 100.0f;
	star1->SetImage("textures/star.png", renderer);

	star2 = new Entity();
	star2->pos = Vec3(15.0f, 25.0f, 0.0f);
    star2->mass = 150.0f;
	star2->SetImage("textures/star.png", renderer);
	
	return true;
}

void Scene2::OnDestroy() {

	if (renderer) {
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}

	delete planet;
	delete star1;
	delete star2;
}

void Scene2::HandleEvents(const SDL_Event& event)
{
	Vec3 vel = planet->initVel;
	switch (event.type) {
	case SDL_KEYDOWN:
		// Change angle of the ball
		if (event.key.keysym.scancode == SDL_SCANCODE_A) {
			vel.x -= 0.2f;
		}
		if (event.key.keysym.scancode == SDL_SCANCODE_D) {
			vel.x += 0.2f;
		}
		if (event.key.keysym.scancode == SDL_SCANCODE_W) {
			vel.y += 0.2f;
		}
		if (event.key.keysym.scancode == SDL_SCANCODE_S) {
			vel.y -= 0.2f;
		}

		cout << "Initial Velocity: ";
		vel.print();
		planet->initVel = vel;
		initPlanet(vel);
		break;

	default:
		break;
	}
}


void Scene2::Update(const float deltaTime) {
	/// Physics goes here
	planet->Update(deltaTime, star1, star2);
}

void Scene2::Render() const {
	SDL_RenderClear(renderer);

	// Display the star1 object on the screen
	Vec3 screenCoords = projectionMatrix * star1->pos;
	SDL_Rect square;
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = star1->GetSurface()->w * starScale;
	square.h = star1->GetSurface()->h * starScale;

    square.x -= square.w / 2;
	square.y -= square.h / 2;
	SDL_RenderCopyEx(renderer, star1->GetTexture(), nullptr, &square, star1->angleDeg, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * star2->pos;
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = star2->GetSurface()->w * starScale;
	square.h = star2->GetSurface()->h * starScale;

	square.x -= square.w / 2;
	square.y -= square.h / 2;
	SDL_RenderCopyEx(renderer, star2->GetTexture(), nullptr, &square, star2->angleDeg, nullptr, SDL_FLIP_NONE);

	// Display the flappy bird object on the screen
	screenCoords = projectionMatrix * planet->pos;
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = planet->GetSurface()->w * planetScale; // we will scale the bird 
	square.h = planet->GetSurface()->h * planetScale;

	square.x -= square.w / 2;
	square.y -= square.h / 2;
	SDL_RenderCopyEx(renderer, planet->GetTexture(), nullptr, &square, planet->angleDeg, nullptr, SDL_FLIP_NONE);

	// Update the screen
	SDL_RenderPresent(renderer);


}