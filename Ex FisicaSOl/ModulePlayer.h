#pragma once
#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"

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

struct RigidBody {
	fPoint position;
	fPoint acceleration;
	fPoint speed;
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
	int m = 0;

public:
	// Movement
	RigidBody rigid;

	void AccelerationController();


};