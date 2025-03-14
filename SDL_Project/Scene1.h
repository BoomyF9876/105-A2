#pragma once
#include <SDL.h>
#include <Matrix.h>
#include "Scene.h"
#include "Entity.h"

using namespace MATH;
class Scene1 : public Scene {
private:
	SDL_Window *window;
	float xAxis;
	float yAxis;

	Matrix4 projectionMatrix;
	SDL_Renderer* renderer;

	Entity* cliff;
	Entity* ball;
	Entity* flappyBird;

	float cliffScale = 0.6f; // a scale to make the texture smaller or larger
	float ballScale = 0.8f;
	float flappyBirdScale = 1.0f; // a scale to make the texture smaller or larger
	float constantScale = 1.0f;

public:
	Scene1(SDL_Window* sdlWindow);
	~Scene1();
	bool OnCreate() override;
	void OnDestroy() override;
	void HandleEvents(const SDL_Event& event) override;
	void Update(const float time) override;
	void Render() const override;
};


