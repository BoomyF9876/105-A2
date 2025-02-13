#include "Scene0.h"
#include <SDL.h>
#include <SDL_image.h>
#include <MMath.h>
#include <iostream>
#include "Entity.h"

Scene0::Scene0(SDL_Window* sdlWindow_){
	window = sdlWindow_;
	renderer = nullptr;
	ball = nullptr;
	xAxis = 30.0f;
	yAxis = 15.0f;
	IMG_Init(IMG_INIT_PNG);
}

Scene0::~Scene0(){
	IMG_Quit();
}

bool Scene0::OnCreate() {
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
	ball = new Entity();
	ball->pos = Vec3(5.0f, 10.0f, 0.0f);
	ball->SetImage("textures/ball.png", renderer);

	flappyBird = new Entity();
	flappyBird->pos = Vec3(12.0f, 5.0f, 0.0f);
	flappyBird->SetImage("textures/flappyBird.png", renderer);

	return true;
}

void Scene0::OnDestroy() {

	if (renderer) {
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}

	delete ball;
	ball = nullptr;

}

void Scene0::HandleEvents(const SDL_Event& event)
{
	switch (event.type) {
	case SDL_KEYDOWN:
		// Change angle of the ball
		if (event.key.keysym.scancode == SDL_SCANCODE_O) {
			ball->angleDeg -= 10.0f;
		}
		if (event.key.keysym.scancode == SDL_SCANCODE_P) {
			ball->angleDeg += 10.0f;
		}
		break;

	default:
		break;
	}
}


void Scene0::Update(const float deltaTime) {
	/// Physics goes here
	

}

void Scene0::Render() const {
	SDL_RenderClear(renderer);

	// Display the ball object on the screen
	Vec3 screenCoords = projectionMatrix * ball->pos;
	SDL_Rect square;
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = ball->GetSurface()->w;
	square.h = ball->GetSurface()->h;
	SDL_RenderCopyEx(renderer, ball->GetTexture(), nullptr, &square, ball->angleDeg, nullptr, SDL_FLIP_NONE);

	// Display the flappy bird object on the screen
	screenCoords = projectionMatrix * flappyBird->pos;
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = flappyBird->GetSurface()->w * flappyBirdScale; // we will scale the bird 
	square.h = flappyBird->GetSurface()->h * flappyBirdScale;
	SDL_RenderCopyEx(renderer, flappyBird->GetTexture(), nullptr, &square, flappyBird->angleDeg, nullptr, SDL_FLIP_NONE);

	// Update the screen
	SDL_RenderPresent(renderer);


}