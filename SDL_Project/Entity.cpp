#include "Entity.h"
using namespace std;

Entity::Entity(): // this is an initializer list
	surface(nullptr), 
	texture(nullptr), 
	angleDeg(0.0f),
	mass(1.0f),
	vel(Vec3(0.0f,0.0f,0.0f)),
	acc(Vec3(0.0f,0.0f,0.0f)),
	constant(DEFAULT_DRAG_FORCE_CONSTANT),
	r(1.0f)
{
}

Entity::~Entity() {
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	surface = nullptr;
	texture = nullptr;
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
	// Apply drag force
	acc = vel * -constant / mass;

	// Apply other forces
    acc += Vec3(force.x / mass, force.y / mass, force.z / mass);
}

void Entity::Update(float deltaTime)
{
	pos += Vec3(
		vel.x * deltaTime + 0.5f * pow(deltaTime, 2) * acc.x,
		vel.y * deltaTime + 0.5f * pow(deltaTime, 2) * acc.y,
		0
	);
    vel += acc * deltaTime;

	ApplyForce(initialForce);
}
