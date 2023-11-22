#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#pragma once
#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModulePhysics.h"
#include "Player.h"
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
	
	Player* myPlayers[2];
	Player* currentPlayer;

public:
	// Movement
	

	void ChangeDir(Player &p);
	void AccelerationController(Direction dir , Player* p);
	void PositionController(Direction dir, Player* p);
	void VelocityController(Direction dir, Player* p);
	void ImpulseController(Direction dir , Player* p);
	void MomentumController(Direction dir, Player* p);
	void OnCollision(RigidBody* c1, RigidBody* c2);

	void Shoot();
	int turno = 0;
	
	Movement myMovement[5];
	Movement* currentMovement;


	int m = 2;

	bool shoted = false;
	bool moved = false;

	float CalculateMomentum(Player* p);
};

class Weapon {
public:

	const char* name;
	Player* player;
	std::list<Bullet*> bodies;

	virtual void Shoot(ModulePlayer* pManager, ModulePhysics* physics) {




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
		bullet->CreateCollider(r, ColliderType::BULLET, pManager);
		physics->bodies.push_back(bullet);

		static char title[400];
		
	}

};

#endif