#include "Entity.h"
using namespace std;

Entity::Entity(): // this is an initializer list
	surface(nullptr), 
	texture(nullptr),
	mass(1.0f),
	initVel(Vec3(0.0f,0.0f,0.0f)),
	vel(initVel),
	acc(Vec3(0.0f,0.0f,0.0f)),
	angleDeg(0.0f)
{
}

Entity::~Entity() {
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	surface = nullptr;
	texture = nullptr;
}

Vec3 applyNewtonsLaw(Entity* entity1, Entity* entity2) {
	Vec3 radius = entity2->pos - entity1->pos;
	float radiusSquare = pow(radius.x,2) + pow(radius.y,2);
	float forceAbs = entity1->mass * entity2->mass / radiusSquare;

	Vec3 normalized = radius / sqrt(radiusSquare);

	return normalized * forceAbs;
}

Vec3 calculateGravityForce(Entity* planet, Entity* star1, Entity* star2) {
	Vec3 force1 = applyNewtonsLaw(planet, star1);
	Vec3 force2 = applyNewtonsLaw(planet, star2);

	return force1 + force2;
}

void Entity::SetImage(const char* filename, SDL_Renderer* renderer) {
	surface = IMG_Load(filename);
	if (surface == nullptr) {
		std::cerr << "Did you spell the file right?" << std::endl;
	}
	texture = SDL_CreateTextureFromSurface(renderer, surface);
}

void Entity::ApplyForce(Vec3 force)
{
	// Apply other forces
    acc = Vec3(force.x / mass, force.y / mass, force.z / mass);
}

void Entity::Update(float deltaTime, Entity* star1, Entity* star2)
{
	pos += Vec3(
		vel.x * deltaTime + 0.5f * pow(deltaTime, 2) * acc.x,
		vel.y * deltaTime + 0.5f * pow(deltaTime, 2) * acc.y,
		0
	);
    vel += acc * deltaTime;

	Vec3 force = calculateGravityForce(this, star1, star2);
	ApplyForce(force);
}
