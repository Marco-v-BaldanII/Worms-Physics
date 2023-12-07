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
	box_texture = App->textures->Load("Assets/images/BloquePhys-Sheet.png");

	App->renderer->camera.x = App->renderer->camera.y = 0;
	//pigTexture = App->textures->Load("Assets/images/PIG.png");
	App->audio->PlayMusic("Assets/sounds/battlefield.ogg");
	SDL_Rect Trect1 = { 200,200,60,60 };
	//AddTarget(Trect1, Trect1);


	/*ground = new RigidBody(SDL_Rect{0, 500, 1800, 800});
	ground->CreateCollider(SDL_Rect{ 0,400,1800,800 }, ColliderType::GROUND, this);
	App->physics->bodies.push_back(ground);*/

	/*ground2 = new RigidBody(SDL_Rect{ 300,300,100,100 });
	ground2->CreateCollider(SDL_Rect{ 300,300,100,100 }, ColliderType::BREAKABLE, this);
	App->physics->bodies.push_back(ground2);*/

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

	aground = new RigidBody(SDL_Rect{1530,335,171,261}); 
	aground->CreateCollider(SDL_Rect{ 0,282,272,80 }, ColliderType::GROUND, this);

	aground1= new RigidBody(SDL_Rect{ 1465,400,171,261 });
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

	bouncer2 = App->physics->createBouncer(970, 585, 230, 200);
	App->physics->bodies.push_back(bouncer2);


	currentPlayer = 0;

	PlaceBoxes();

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

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		App->deltaTime.ChangeFPS(15);
	}
	else if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		App->deltaTime.ChangeFPS(30);
	}
	else if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		App->deltaTime.ChangeFPS(60);
	}
	else if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		App->deltaTime.toggleDeltaTimeMode();
	}
	
	static char title[400];

	sprintf_s(title, 400, "Deltatime: %f  FPS: %f",
	App->deltaTime.delta, App->deltaTime.FPS);
	App->window->SetTitle(title);
	App->renderer->Blit(backgound, 0, 0);
	if (!box1->rigid->destroyed) { App->renderer->Blit(box1->rigid->bird, box1->rigid->posRect.x, box1->rigid->posRect.y); box1->rigid->acceleration.y = 9.81;}
	if (!box2->rigid->destroyed) { App->renderer->Blit(box2->rigid->bird, box2->rigid->posRect.x, box2->rigid->posRect.y); box2->rigid->acceleration.y = 9.81;}
	if (!box3->rigid->destroyed) { App->renderer->Blit(box3->rigid->bird, box3->rigid->posRect.x, box3->rigid->posRect.y); box3->rigid->acceleration.y = 9.81;}
	if (!box4->rigid->destroyed) { App->renderer->Blit(box4->rigid->bird, box4->rigid->posRect.x, box4->rigid->posRect.y); box4->rigid->acceleration.y = 9.81;}
	if (!box5->rigid->destroyed) { App->renderer->Blit(box5->rigid->bird, box5->rigid->posRect.x, box5->rigid->posRect.y); box5->rigid->acceleration.y = 9.81;}
	if (!box6->rigid->destroyed) { App->renderer->Blit(box6->rigid->bird, box6->rigid->posRect.x, box6->rigid->posRect.y); box6->rigid->acceleration.y = 9.81;}
	if (!box7->rigid->destroyed) { App->renderer->Blit(box7->rigid->bird, box7->rigid->posRect.x, box7->rigid->posRect.y); box7->rigid->acceleration.y = 9.81;}
	if (!box9->rigid->destroyed) { App->renderer->Blit(box9->rigid->bird, box9->rigid->posRect.x, box9->rigid->posRect.y); box9->rigid->acceleration.y = 9.81; }
	if (!box10->rigid->destroyed) { App->renderer->Blit(box10->rigid->bird, box10->rigid->posRect.x, box10->rigid->posRect.y); box10->rigid->acceleration.y = 9.81; }
	
	
	/*if (App->player->currentPlayer->currentWeapon->name == "birdBazooka") {
		App->renderer->Blit(GunSelection, 100, 700);

	}*/
	
	return UPDATE_CONTINUE;
}

void AddTarget(SDL_Rect posRect, SDL_Rect collid) {}

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

void ModuleSceneIntro::PlaceBoxes() {
	
	box1 = new Box(box_texture, SDL_Rect{ 951, 530, 32, 32 });
	box1->rigid->CreateCollider(SDL_Rect{ 951, 530, 32, 28 }, ColliderType::BREAKABLE, App->player);
	box2 = new Box(box_texture, SDL_Rect{ 951, 498, 32, 32 });
	box2->rigid->CreateCollider(SDL_Rect{ 951, 498, 32, 28 }, ColliderType::BREAKABLE, App->player);
	box3 = new Box(box_texture, SDL_Rect{ 951, 498 -32, 32, 32 });
	box3->rigid->CreateCollider(SDL_Rect{ 951, 498 -32, 28, 32 }, ColliderType::BREAKABLE, App->player);
	box4 = new Box(box_texture, SDL_Rect{ 951, 498 - 64, 32, 32 });
	box4->rigid->CreateCollider(SDL_Rect{ 951, 498 - 64, 28, 32 }, ColliderType::BREAKABLE, App->player);
	box5 = new Box(box_texture, SDL_Rect{ 216, 471, 32, 32 });
	box5->rigid->CreateCollider(SDL_Rect{ 216, 471, 28, 32 }, ColliderType::BREAKABLE, App->player);
	box6 = new Box(box_texture, SDL_Rect{ 216, 471-32, 32, 32 });
	box6->rigid->CreateCollider(SDL_Rect{ 216, 471-32, 28, 32 }, ColliderType::BREAKABLE, App->player);
	box7 = new Box(box_texture, SDL_Rect{ 951-40, 530 , 32, 32 });
	box7->rigid->CreateCollider(SDL_Rect{ 951-40, 530 , 28, 32 }, ColliderType::BREAKABLE, App->player);

	/*box8 = new Box(box_texture, SDL_Rect{  951,  498 - 96, 32 });
	box8->rigid->CreateCollider(SDL_Rect{  951,  498 - 96, 32 }, ColliderType::BREAKABLE, App->player);*/
	box9 = new Box(box_texture, SDL_Rect{  951,  498 - 96-33, 32, 32 });
	box9->rigid->CreateCollider(SDL_Rect{  951,  498 - 96-33, 28, 32 }, ColliderType::BREAKABLE, App->player);
	box10 = new Box(box_texture, SDL_Rect{ 951,  498 - 96 - 65 , 32, 32 });
	box10->rigid->CreateCollider(SDL_Rect{ 951,  498 - 96 - 65 , 28, 32 }, ColliderType::BREAKABLE, App->player);
	
	box1->rigid->acceleration.y = 9.81; box2->rigid->acceleration.y = 9.81; box3->rigid->acceleration.y = 9.81; box4->rigid->acceleration.y = 9.81;
	box5->rigid->acceleration.y = 9.81; box6->rigid->acceleration.y = 9.81; box7->rigid->acceleration.y = 9.81; 
	box9->rigid->acceleration.y = 9.81; box10->rigid->acceleration.y = 9.81;

	// boxes
	App->physics->bodies.push_back(box1->rigid);
	App->physics->bodies.push_back(box2->rigid);
	App->physics->bodies.push_back(box3->rigid);
	App->physics->bodies.push_back(box4->rigid);
	App->physics->bodies.push_back(box5->rigid);
	App->physics->bodies.push_back(box6->rigid);
	App->physics->bodies.push_back(box7->rigid);
	//App->physics->bodies.push_back(box8->rigid);
	App->physics->bodies.push_back(box9->rigid);
	App->physics->bodies.push_back(box10->rigid);
	
}

void ModuleSceneIntro::OnCollision(RigidBody* c1, RigidBody* c2) {

	/*if (c1->collider->type == ColliderType::BREAKABLE && c2->collider->type != ColliderType::PLAYER) {
		c1->acceleration.y = 0;
	}
	if (c2->collider->type == ColliderType::BREAKABLE && c1->collider->type != ColliderType::PLAYER) {
		c2->acceleration.y = 0;
	}*/






}
