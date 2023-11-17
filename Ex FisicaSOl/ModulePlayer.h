#pragma once
#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModulePhysics.h"

#include "Application.h"
#include <string>

#define NUM_WEAPONS 1

class Weapon;

struct Object
{
	SDL_Texture* graphic;
	uint fx;

	Object() : graphic(NULL)
	{}
};

struct iPoint {
	int x;
	int y;
};

struct fPoint {
	float x;
	float y;
};

enum Movement {
	POSITION,
	VELOCITY,
	ACCELERATION,
	MOMENTUM,
	IMPULSE
};


enum class Direction {
	RIGHT,
	LEFT


};

class Bullet : public RigidBody {

	SDL_Texture* texture = nullptr;

};




class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();
	SDL_Texture* playertex;
	SDL_Rect player;
	SDL_Texture* player1;
	SDL_Texture* player2;

	Movement myMovement[5];
	Movement* currentMovement;
	int m = 2;

	Weapon* myWeapons;

	bool isJumping = false;
	int jumpingCnt = 90;

public:
	// Movement
	RigidBody* rigid;

	Direction myDirection;

	void ChangeDir();
	void AccelerationController(Direction dir);
	void PositionController(Direction dir);
	void VelocityController(Direction dir);
	void ImpulseController(Direction dir);
	void MomentumController(Direction dir);
	void OnCollision(RigidBody* c1, RigidBody* c2);

	void Shoot();


	float CalculateMomentum();
};

class Weapon {
public:

	const char* name;

	std::list<Bullet*> bodies;

	virtual void Shoot(ModulePlayer* player, ModulePhysics* physics) {




		Bullet* bullet = new Bullet;
		bullet->isMoving = true;
		bullet->posRect.x = player->rigid->posRect.x;
		bullet->posRect.y = player->rigid->posRect.y;
		bullet->posRect.w = 10;
		bullet->posRect.h = 10;
		float initialSpeed = 400.0f;
		float angle = 45;
		angle = angle * M_PI / 180.0f;
		bullet->velocity.x = initialSpeed * cos(angle);
		bullet->velocity.y = -initialSpeed * sin(angle);
		bullet->acceleration = { 0,981 };
		SDL_Rect r = { 0,0,40,40 };
		bullet->CreateCollider(r, ColliderType::BULLET, player);
		physics->bodies.push_back(bullet);

		static char title[400];
		
	}

};
