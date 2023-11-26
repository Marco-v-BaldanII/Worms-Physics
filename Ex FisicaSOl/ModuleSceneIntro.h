#pragma once
#include "Module.h"
#include "Animation.h"
#include "p2DynArray.h"
#include "Globals.h"

#define BOUNCER_TIME 200

enum lightTypes
{
	tiny,
	medium,
	big
};

class ModuleSceneIntro;

struct Light
{
	Light() : texture(NULL), on(false), fx(0)
	{}

	Light(ModuleSceneIntro* physics, int x, int y, lightTypes type);

	lightTypes type;
	SDL_Texture* texture;
	bool on;
	uint fx;
	int x, y;
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();

	void AddTarget(SDL_Rect posRect, SDL_Rect collid);

	int currentPlayer = 0;

public:

	RigidBody* ground;
	RigidBody* ground2;
	RigidBody* ground3;
	RigidBody* ground4;
	RigidBody* ground5;
	RigidBody* ground6;
	RigidBody* ground7;
	RigidBody* ground8;
	RigidBody* ground9;
	RigidBody* ground10;
	RigidBody* ground11;
	RigidBody* ground12;	
	RigidBody* ground13;






	RigidBody* underground;
	RigidBody* underground1;
	RigidBody* underground2;
	RigidBody* underground3;
	RigidBody* underground4;
	RigidBody* underground5;
	RigidBody* underground6;
	RigidBody* underground7;
	RigidBody* underground8;
	RigidBody* underground9;
	RigidBody* underground10;
	RigidBody* underground11;
	RigidBody* underground12;
	RigidBody* underground13;
	RigidBody* underground14;
	RigidBody* underground15;




	RigidBody* bouncer1;

	SDL_Texture* backgound;
	SDL_Texture* graphics;

	SDL_Texture* tex_light_tiny;
	SDL_Texture* tex_light_medium;
	SDL_Texture* tex_light_big;
	SDL_Texture* pigTexture;
	
	uint fx_light_tiny;
	uint fx_light_medium;
	uint fx_light_big;

	p2DynArray<Light> lights;

	uint player_lose_fx;
};
