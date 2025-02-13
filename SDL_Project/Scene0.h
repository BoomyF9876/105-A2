#pragma once
#include <SDL.h>
#include <Matrix.h>
#include "Scene.h"
#include "Entity.h"

using namespace MATH;
class Scene0 : public Scene {
private:
	SDL_Window *window;
	float xAxis;
	float yAxis;

	Matrix4 projectionMatrix;
	SDL_Renderer* renderer;

	Entity* ball;
	Entity* flappyBird;

	float flappyBirdScale = 2.0f; // a scale to make the texture smaller or larger
	
public:
	Scene0(SDL_Window* sdlWindow);
	~Scene0();
	bool OnCreate() override;
	void OnDestroy() override;
	void HandleEvents(const SDL_Event& event) override;
	void Update(const float time) override;
	void Render() const override;
};


