#pragma once
#include "Module.h"
#include "Animation.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Anim.h"

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

struct Box {

	
	RigidBody* rigid;

	Box(SDL_Texture* text, SDL_Rect rect) {
		
		rigid = new RigidBody(rect);
		rigid->bird = text;
	}

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

	void OnExplosion(RigidBody* c1);

	void PlaceBoxes();

	void OnCollision(RigidBody* c1, RigidBody* c2);

	int currentPlayer = 0;

	Anim rightFlying, leftFlying;

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
	

	RigidBody* aground;
	RigidBody* aground1;
	RigidBody* aground2;
	RigidBody* aground3;
	RigidBody* aground4;
	
	Box* box1;
	Box* box2;
	Box* box3;
	Box* box4;
	Box* box5;
	Box* box6;
	Box* box7;
	Box* box8;

	Box* box9;
	Box* box10;
	Box* box11;
	Box* box12;

	SDL_Texture* box_texture = nullptr;


	RigidBody* bouncer1;
	RigidBody* bouncer2;

	

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
