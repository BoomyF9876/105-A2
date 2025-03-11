#pragma once

#include <Vector.h>
#include <SDL_image.h>
#include <SDL.h>
using namespace MATH;

class Entity {
private:
	// Keep these private as we should only build them in the setImage method
	SDL_Surface* surface = nullptr; // Used to get the width and height of the image
	SDL_Texture* texture = nullptr; // Used to render the image

public:
	Vec3 pos;
	Vec3 vel;
	Vec3 acc;
	float mass;

	Entity();
	~Entity();
	void SetImage(const char* filename, SDL_Renderer* renderer);

	// Just like assignment 1!
	void ApplyForce(Vec3 force);
	void Update(float deltaTime);

	// Need getters for private member variables
	SDL_Surface* GetSurface() const { return surface; }
	SDL_Texture* GetTexture() const { return texture; }
};

