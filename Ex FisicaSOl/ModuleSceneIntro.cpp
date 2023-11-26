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
	ground->CreateCollider(SDL_Rect{ 0,282,225,40 }, ColliderType::GROUND, this);

	ground2 = new RigidBody(SDL_Rect{ 290,330,171,261 });
	ground2->CreateCollider(SDL_Rect{ 0,282,140,5 }, ColliderType::GROUND, this);

	ground3 = new RigidBody(SDL_Rect{ 335,315,171,261 });
	ground3->CreateCollider(SDL_Rect{ 0,282,63,20 }, ColliderType::GROUND, this);


	ground4 = new RigidBody(SDL_Rect{ 65,235,171,261 });
	ground4->CreateCollider(SDL_Rect{ 0,282,80,65 }, ColliderType::GROUND, this);

	ground5 = new RigidBody(SDL_Rect{ 145,265,171,261 });
	ground5->CreateCollider(SDL_Rect{ 0,282,30,35 }, ColliderType::GROUND, this);
	
	ground6 = new RigidBody(SDL_Rect{ 495,330,171,261 });
	ground6->CreateCollider(SDL_Rect{ 0,282,50,20 }, ColliderType::GROUND, this);

	ground7 = new RigidBody(SDL_Rect{ 545,315,171,261 });
	ground7->CreateCollider(SDL_Rect{ 0,282,20,20 }, ColliderType::GROUND, this);

	ground8 = new RigidBody(SDL_Rect{ 560,300,171,261 });
	ground8->CreateCollider(SDL_Rect{ 0,282,80,40 }, ColliderType::GROUND, this);

	ground9 = new RigidBody(SDL_Rect{ 640,330,171,261 });
	ground9->CreateCollider(SDL_Rect{ 0,282,62,65 }, ColliderType::GROUND, this);

	ground10 = new RigidBody(SDL_Rect{ 63,335,171,261 });
	ground10->CreateCollider(SDL_Rect{ 0,282,20,75 }, ColliderType::GROUND, this);

	ground11 = new RigidBody(SDL_Rect{ 80,410,171,261 });
	ground11->CreateCollider(SDL_Rect{ 0,282,558,33 }, ColliderType::GROUND, this);

	ground12 = new RigidBody(SDL_Rect{ 640,395,171,261 });
	ground12->CreateCollider(SDL_Rect{ 0,282,30,15 }, ColliderType::GROUND, this);

	ground13 = new RigidBody(SDL_Rect{ 112,443,171,261 });
	ground13->CreateCollider(SDL_Rect{ 0,282,430,15 }, ColliderType::GROUND, this);

	App->physics->bodies.push_back(ground);
	App->physics->bodies.push_back(ground2);
	App->physics->bodies.push_back(ground3);
	App->physics->bodies.push_back(ground4);
	App->physics->bodies.push_back(ground5);
	App->physics->bodies.push_back(ground6);
	App->physics->bodies.push_back(ground7);
	App->physics->bodies.push_back(ground8);
	App->physics->bodies.push_back(ground9);
	App->physics->bodies.push_back(ground10);
	App->physics->bodies.push_back(ground11);
	App->physics->bodies.push_back(ground12);
	App->physics->bodies.push_back(ground13);



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

