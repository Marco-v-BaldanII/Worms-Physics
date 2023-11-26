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

	//primer plataforma//
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

	//subsuelo//
	underground = new RigidBody(SDL_Rect{ 0,490,171,261 });
	underground->CreateCollider(SDL_Rect{ 0,282,63,37 }, ColliderType::GROUND, this);

	underground1 = new RigidBody(SDL_Rect{ 63,525,171,261 });
	underground1->CreateCollider(SDL_Rect{ 0,282,30,35 }, ColliderType::GROUND, this);


	underground2 = new RigidBody(SDL_Rect{ 145,540,171,261 });
	underground2->CreateCollider(SDL_Rect{ 0,282,93,15 }, ColliderType::GROUND, this);

	underground3 = new RigidBody(SDL_Rect{ 235,555,171,261 });
	underground3->CreateCollider(SDL_Rect{ 0,282,20,25 }, ColliderType::GROUND, this);

	underground4 = new RigidBody(SDL_Rect{ 250,573,171,261 });
	underground4->CreateCollider(SDL_Rect{ 0,282,37,15 }, ColliderType::GROUND, this);

	underground5 = new RigidBody(SDL_Rect{ 280,587,171,261 });
	underground5->CreateCollider(SDL_Rect{ 0,282,135,25 }, ColliderType::GROUND, this);

	underground6 = new RigidBody(SDL_Rect{ 463,603,171,261 });
	underground6->CreateCollider(SDL_Rect{ 0,282,100,25 }, ColliderType::GROUND, this);

	underground7 = new RigidBody(SDL_Rect{ 559,587,171,261 });
	underground7->CreateCollider(SDL_Rect{ 0,282,50,15 }, ColliderType::GROUND, this);

	underground8 = new RigidBody(SDL_Rect{ 607,570,171,261 });
	underground8->CreateCollider(SDL_Rect{ 0,282,35,15 }, ColliderType::GROUND, this);

	underground9 = new RigidBody(SDL_Rect{ 638,555,171,261 });
	underground9->CreateCollider(SDL_Rect{ 0,282,82,35 }, ColliderType::GROUND, this);

	underground10 = new RigidBody(SDL_Rect{ 718,587,171,261 });
	underground10->CreateCollider(SDL_Rect{ 0,282,35,15 }, ColliderType::GROUND, this);

	underground11 = new RigidBody(SDL_Rect{ 800,587,171,261 });
	underground11->CreateCollider(SDL_Rect{ 0,282,35,15 }, ColliderType::GROUND, this);

	underground12 = new RigidBody(SDL_Rect{ 832,555,171,261 });
	underground12->CreateCollider(SDL_Rect{ 0,282,35,35 }, ColliderType::GROUND, this);

	underground13 = new RigidBody(SDL_Rect{ 864,538,171,261 });
	underground13->CreateCollider(SDL_Rect{ 0,282,35,20 }, ColliderType::GROUND, this);

	underground14 = new RigidBody(SDL_Rect{ 896,522,171,261 });
	underground14->CreateCollider(SDL_Rect{ 0,282,65,20 }, ColliderType::GROUND, this);


	underground15 = new RigidBody(SDL_Rect{ 960,538,171,261 });
	underground15->CreateCollider(SDL_Rect{ 0,282,18,20 }, ColliderType::GROUND, this);


	App->physics->bodies.push_back(underground);
	App->physics->bodies.push_back(underground1);
	App->physics->bodies.push_back(underground2);
	App->physics->bodies.push_back(underground3);
	App->physics->bodies.push_back(underground4);
	App->physics->bodies.push_back(underground5);
	App->physics->bodies.push_back(underground6);
	App->physics->bodies.push_back(underground7);
	App->physics->bodies.push_back(underground8);
	App->physics->bodies.push_back(underground9);
	App->physics->bodies.push_back(underground10);
	App->physics->bodies.push_back(underground11);
	App->physics->bodies.push_back(underground12);
	App->physics->bodies.push_back(underground13);
	App->physics->bodies.push_back(underground14);
	App->physics->bodies.push_back(underground15);


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

