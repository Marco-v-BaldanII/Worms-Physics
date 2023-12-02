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


	/*ground = new RigidBody(SDL_Rect{0, 500, 1800, 800});
	ground->CreateCollider(SDL_Rect{ 0,400,1800,800 }, ColliderType::GROUND, this);
	App->physics->bodies.push_back(ground);*/

	ground2 = new RigidBody(SDL_Rect{ 300,300,100,100 });
	ground2->CreateCollider(SDL_Rect{ 300,300,100,100 }, ColliderType::BREAKABLE, this);
	App->physics->bodies.push_back(ground2);

	//primer plataforma//
	ground = new RigidBody(SDL_Rect{ 0,480,171,261 });
	ground->CreateCollider(SDL_Rect{ 0,282,115,20 }, ColliderType::GROUND, this);

	ground2 = new RigidBody(SDL_Rect{ 110,495,171,261 });
	ground2->CreateCollider(SDL_Rect{ 110,282,130,40 }, ColliderType::GROUND, this);

	ground3 = new RigidBody(SDL_Rect{ 230,525,171,261 });
	ground3->CreateCollider(SDL_Rect{ 240,282,75,40 }, ColliderType::GROUND, this);


	ground4 = new RigidBody(SDL_Rect{305,560,171,261});
	ground4->CreateCollider(SDL_Rect{305,282,65,40 }, ColliderType::GROUND, this);

	ground5 = new RigidBody(SDL_Rect{ 365,590,171,261 });
	ground5->CreateCollider(SDL_Rect{ 0,282,52,75 }, ColliderType::GROUND, this);
	
	ground6 = new RigidBody(SDL_Rect{ 400,655,171,261 });
	ground6->CreateCollider(SDL_Rect{ 0,282,65,40 }, ColliderType::GROUND, this);

	ground7 = new RigidBody(SDL_Rect{ 565,690,171,261 });
	ground7->CreateCollider(SDL_Rect{ 0,282,75,20 }, ColliderType::GROUND, this);

	ground8 = new RigidBody(SDL_Rect{ 630,675,171,261 });
	ground8->CreateCollider(SDL_Rect{ 0,282,105,20 }, ColliderType::GROUND, this);

	ground9 = new RigidBody(SDL_Rect{ 725,640,171,261 });
	ground9->CreateCollider(SDL_Rect{ 0,282,90,40 }, ColliderType::GROUND, this);

	ground10 = new RigidBody(SDL_Rect{ 806,608,171,261 });
	ground10->CreateCollider(SDL_Rect{ 0,282,60,40 }, ColliderType::GROUND, this);

	ground11 = new RigidBody(SDL_Rect{ 855,575,171,261 });
	ground11->CreateCollider(SDL_Rect{ 0,282,40,40 }, ColliderType::GROUND, this);

	ground12 = new RigidBody(SDL_Rect{ 885,560,171,261 });
	ground12->CreateCollider(SDL_Rect{ 0,282,100,15 }, ColliderType::GROUND, this);

	



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


	////segunda plataforma//

	aground = new RigidBody(SDL_Rect{1530,330,171,261}); 
	aground->CreateCollider(SDL_Rect{ 0,282,272,80 }, ColliderType::GROUND, this);

	aground1= new RigidBody(SDL_Rect{ 1465,395,171,261 });
	aground1->CreateCollider(SDL_Rect{ 0,282,80 ,100 }, ColliderType::GROUND, this);

	aground2 = new RigidBody(SDL_Rect{ 1370,475,171,261 });
	aground2->CreateCollider(SDL_Rect{ 0,282,100,80 }, ColliderType::GROUND, this);

	aground3 = new RigidBody(SDL_Rect{ 1258 ,540,171,261 });
	aground3->CreateCollider(SDL_Rect{ 0,282,120,40 }, ColliderType::GROUND, this);

	aground4 = new RigidBody(SDL_Rect{1190 ,575,171,261 });
	aground4->CreateCollider(SDL_Rect{ 0,282,100,31 }, ColliderType::GROUND, this);

	



	App->physics->bodies.push_back(aground);
	App->physics->bodies.push_back(aground1);
	App->physics->bodies.push_back(aground2);
	App->physics->bodies.push_back(aground3);
	App->physics->bodies.push_back(aground4);
	


	bouncer1 = App->physics->createBouncer(450,690, 120, 200);
	App->physics->bodies.push_back(bouncer1);

	bouncer2 = App->physics->createBouncer(970, 575, 230, 200);
	App->physics->bodies.push_back(bouncer2);


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


void ModuleSceneIntro::OnExplosion(RigidBody* c1)  {

	if (c1->collider->type == ColliderType::BREAKABLE) {
		for (int i = 0; i < 50; ++i) {
			if (App->physics->corpses[i] == nullptr) {
				App->physics->corpses[i] = c1;
				break;
			}
		}
	}

}
