#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModulePhysics.h"



ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	graphics = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	backgound = App->textures->Load("Assets/images/fondo.png");
	App->renderer->camera.x = App->renderer->camera.y = 0;
	//pigTexture = App->textures->Load("Assets/images/PIG.png");

	SDL_Rect Trect1 = { 200,200,60,60 };
	//AddTarget(Trect1, Trect1);
	ground = new RigidBody(SDL_Rect{ 65,300,171,261 });
	ground->CreateCollider(SDL_Rect{ 0,282,225,80 }, ColliderType::GROUND, this);

	ground2 = new RigidBody(SDL_Rect{ 290,330,171,261 });
	ground2->CreateCollider(SDL_Rect{ 0,282,140,80 }, ColliderType::GROUND, this);

	App->physics->bodies.push_back(ground);
	App->physics->bodies.push_back(ground2);
	bouncer1 = App->physics->createBouncer(10, 10, 10, 10);
	App->physics->bodies.push_back(bouncer1);
	currentPlayer = 0;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	//metodo para cambiar para jugadrres

	
	App->renderer->Blit(backgound, 0, 0);

	return UPDATE_CONTINUE;
}


void AddTarget(SDL_Rect posRect, SDL_Rect collid) {






}

