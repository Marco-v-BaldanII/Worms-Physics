#pragma once
#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModulePhysics.h"

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

public:
	// Movement
	RigidBody* rigid;

	Direction myDirection;

	void ChangeDir();
	void AccelerationController(Direction dir);
	void PositionController(Direction dir);
	void VelocityController(Direction dir);
	void ImpulseController(Direction dir);

};