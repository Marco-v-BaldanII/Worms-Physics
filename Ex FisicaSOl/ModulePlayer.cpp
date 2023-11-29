#include <iostream>
#include <iomanip>
#include <random>
#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "Animation.h"
#include "ModuleRender.h"
#include "Player.h"

#define ACCELERATION_VALUE 90;
#define JUMP_FORCE 300;
#define MaxSpeed 4;



ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	for (int i = 0; i < NUM_PLAYERS; ++i) {
		myPlayers[i] = new Player();
		myPlayers[i]->rigid = new RigidBody();


		LOG("Loading player");
		myPlayers[i]->player1 = App->textures->Load("Assets/images/Dabomb-Sheet.png");
		//myPlayers[i]player2 = App->textures->Load("Assets/images/PIG.png");
		myPlayers[i]->player = { 10,10,10,10 };


		if (i == 0) { myPlayers[i]->rigid->posRect.x = 50; }
		else { myPlayers[i]->rigid->posRect.x = 1; }
		myPlayers[i]->rigid->posRect.y = 200;
		myPlayers[i]->rigid->CreateCollider(SDL_Rect{ 0,0,65,87 }, ColliderType::PLAYER, this);

	
		myMovement[0] = Movement::POSITION;
		myMovement[1] = Movement::VELOCITY;
		myMovement[2] = Movement::ACCELERATION;
		myMovement[3] = Movement::IMPULSE;
		myMovement[4] = Movement::MOMENTUM;

		myPlayers[i]->myDirection = Direction::RIGHT;

		currentMovement = &myMovement[3];

		Weapon weapon1; weapon1.name = "birdBazooka";
		myPlayers[i]->myWeapons = new Weapon[NUM_WEAPONS];
		
		myPlayers[i]->myWeapons[0] = weapon1;
		myPlayers[i]->myWeapons[0].player = myPlayers[i];

		App->physics->bodies.push_back(myPlayers[i]->rigid);
		myPlayers[i]->rigid->acceleration.y = 0;
		myPlayers[i]->rigid->ID = 2;
		myPlayers[i]->rigid->isGrounded = false;
		myPlayers[i]->shoted = false;


		//Esto est� aqu� porque si no no compila
		myPlayers[i]->rightIdle.PushBack({ 0,  0,64,64 });
		myPlayers[i]->rightIdle.PushBack({ 64, 0,64,64 });
		myPlayers[i]->rightIdle.PushBack({ 128,0,64,64 });
		myPlayers[i]->rightIdle.PushBack({ 192,0,64,64 });
		myPlayers[i]->rightIdle.PushBack({ 256,0,64,64 });
		myPlayers[i]->rightIdle.PushBack({ 320,0,64,64 });
		myPlayers[i]->rightIdle.PushBack({ 384,0,64,64 });
		myPlayers[i]->rightIdle.PushBack({ 448,0,64,64 });
		myPlayers[i]->rightIdle.PushBack({ 512,0,64,64 });
		myPlayers[i]->rightIdle.PushBack({ 576,0,64,64 });
		myPlayers[i]->rightIdle.speed = 0.1f; //provisional
		myPlayers[i]->rightIdle.loop = true;

		myPlayers[i]->currentAnim = &myPlayers[i]->rightIdle;

		myPlayers[i]->leftIdle.PushBack({ 0,64,64,64 });
		myPlayers[i]->leftIdle.PushBack({ 64,64,64,64 });
		myPlayers[i]->leftIdle.PushBack({ 128,64,64,64 });
		myPlayers[i]->leftIdle.PushBack({ 192,64,64,64 });
		myPlayers[i]->leftIdle.PushBack({ 256,64,64,64 });
		myPlayers[i]->leftIdle.PushBack({ 320,64,64,64 });
		myPlayers[i]->leftIdle.PushBack({ 384,64,64,64 });
		myPlayers[i]->leftIdle.PushBack({ 448,64,64,64 });
		myPlayers[i]->leftIdle.PushBack({ 512,64,64,64 });
		myPlayers[i]->leftIdle.PushBack({ 576,64,64,64 });
		myPlayers[i]->leftIdle.speed = 0.2f; //provisional
		myPlayers[i]->leftIdle.loop = true;

		myPlayers[i]->rightShoot.PushBack({ 0,  128,64,64 });
		myPlayers[i]->rightShoot.PushBack({ 64, 128,64,64 });
		myPlayers[i]->rightShoot.PushBack({ 128,128,64,64 });
		myPlayers[i]->rightShoot.PushBack({ 192,128,64,64 });
		myPlayers[i]->rightShoot.PushBack({ 256,128,64,64 });
		myPlayers[i]->rightShoot.PushBack({ 320,128,64,64 });
		myPlayers[i]->rightShoot.PushBack({ 384,128,64,64 });
		myPlayers[i]->rightShoot.PushBack({ 448,128,64,64 });
		myPlayers[i]->rightShoot.PushBack({ 512,128,64,64 });
		myPlayers[i]->rightShoot.PushBack({ 576,128,64,64 });
		myPlayers[i]->rightShoot.PushBack({ 640,128,64,64 });
		myPlayers[i]->rightShoot.speed = 0.2f; //provisional
		myPlayers[i]->rightShoot.loop = false;

		myPlayers[i]->leftShoot.PushBack({ 0,  192,64,64 });
		myPlayers[i]->leftShoot.PushBack({ 64, 192,64,64 });
		myPlayers[i]->leftShoot.PushBack({ 128,192,64,64 });
		myPlayers[i]->leftShoot.PushBack({ 192,192,64,64 });
		myPlayers[i]->leftShoot.PushBack({ 256,192,64,64 });
		myPlayers[i]->leftShoot.PushBack({ 320,192,64,64 });
		myPlayers[i]->leftShoot.PushBack({ 384,192,64,64 });
		myPlayers[i]->leftShoot.PushBack({ 448,192,64,64 });
		myPlayers[i]->leftShoot.PushBack({ 512,192,64,64 });
		myPlayers[i]->leftShoot.PushBack({ 576,192,64,64 });
		myPlayers[i]->leftShoot.PushBack({ 640,192,64,64 });
		myPlayers[i]->leftShoot.speed = 0.2f; //provisional
		myPlayers[i]->leftShoot.loop = false;

		myPlayers[i]->rightWalk.PushBack({ 0,  256,64,64 });
		myPlayers[i]->rightWalk.PushBack({ 64, 256,64,64 });
		myPlayers[i]->rightWalk.PushBack({ 128,256,64,64 });
		myPlayers[i]->rightWalk.PushBack({ 192,256,64,64 });
		myPlayers[i]->rightWalk.PushBack({ 256,256,64,64 });
		myPlayers[i]->rightWalk.PushBack({ 320,256,64,64 });
		myPlayers[i]->rightWalk.PushBack({ 384,256,64,64 });
		myPlayers[i]->rightWalk.PushBack({ 448,256,64,64 });
		myPlayers[i]->rightWalk.speed = 0.2f; //provisional
		myPlayers[i]->rightWalk.loop = true;

		myPlayers[i]->leftWalk.PushBack({ 0,  320,64,64 });
		myPlayers[i]->leftWalk.PushBack({ 64, 320,64,64 });
		myPlayers[i]->leftWalk.PushBack({ 128,320,64,64 });
		myPlayers[i]->leftWalk.PushBack({ 192,320,64,64 });
		myPlayers[i]->leftWalk.PushBack({ 256,320,64,64 });
		myPlayers[i]->leftWalk.PushBack({ 320,320,64,64 });
		myPlayers[i]->leftWalk.PushBack({ 384,320,64,64 });
		myPlayers[i]->leftWalk.PushBack({ 448,320,64,64 });
		myPlayers[i]->leftWalk.speed = 0.2f; //provisional
		myPlayers[i]->leftWalk.loop = true;

		myPlayers[i]->rightFall.PushBack({ 0,  384,64,64 });
		myPlayers[i]->rightFall.PushBack({ 64, 384,64,64 });
		myPlayers[i]->rightFall.PushBack({ 128,384,64,64 });
		myPlayers[i]->rightFall.PushBack({ 192,384,64,64 });
		myPlayers[i]->rightFall.speed = 0.2f; //provisional
		myPlayers[i]->rightFall.loop = true;

		myPlayers[i]->leftFall.PushBack({ 0,  448,64,64 });
		myPlayers[i]->leftFall.PushBack({ 64, 448,64,64 });
		myPlayers[i]->leftFall.PushBack({ 128,448,64,64 });
		myPlayers[i]->leftFall.PushBack({ 192,448,64,64 });
		myPlayers[i]->leftFall.speed = 0.2f; //provisional
		myPlayers[i]->leftFall.loop = true;

		myPlayers[i]->rightDeath.PushBack({ 0,  512,64,64 });
		myPlayers[i]->rightDeath.PushBack({ 64, 512,64,64 });
		myPlayers[i]->rightDeath.PushBack({ 128,512,64,64 });
		myPlayers[i]->rightDeath.PushBack({ 192,512,64,64 });
		myPlayers[i]->rightDeath.PushBack({ 256,512,64,64 });
		myPlayers[i]->rightDeath.PushBack({ 320,512,64,64 });
		myPlayers[i]->rightDeath.speed = 0.2f; //provisional
		myPlayers[i]->rightDeath.loop = false;

		myPlayers[i]->leftDeath.PushBack({ 0,  576,64,64 });
		myPlayers[i]->leftDeath.PushBack({ 64, 576,64,64 });
		myPlayers[i]->leftDeath.PushBack({ 128,576,64,64 });
		myPlayers[i]->leftDeath.PushBack({ 192,576,64,64 });
		myPlayers[i]->leftDeath.PushBack({ 256,576,64,64 });
		myPlayers[i]->leftDeath.PushBack({ 320,576,64,64 });
		myPlayers[i]->leftDeath.speed = 0.2f; //provisional
		myPlayers[i]->leftDeath.loop = false;

		myPlayers[i]->rightChargeJump.PushBack({ 0,  640,64,64 });
		myPlayers[i]->rightChargeJump.PushBack({ 64, 640,64,64 });
		myPlayers[i]->rightChargeJump.PushBack({ 128,640,64,64 });
		myPlayers[i]->rightChargeJump.PushBack({ 192,640,64,64 });
		myPlayers[i]->rightChargeJump.PushBack({ 256,640,64,64 });
		myPlayers[i]->rightChargeJump.PushBack({ 320,640,64,64 });
		myPlayers[i]->rightChargeJump.speed = 0.2f; //provisional
		myPlayers[i]->rightChargeJump.loop = true;

		myPlayers[i]->leftChargeJump.PushBack({ 0,  704,64,64 });
		myPlayers[i]->leftChargeJump.PushBack({ 64, 704,64,64 });
		myPlayers[i]->leftChargeJump.PushBack({ 128,704,64,64 });
		myPlayers[i]->leftChargeJump.PushBack({ 192,704,64,64 });
		myPlayers[i]->leftChargeJump.PushBack({ 256,704,64,64 });
		myPlayers[i]->leftChargeJump.PushBack({ 320,704,64,64 });
		myPlayers[i]->leftChargeJump.speed = 0.2f; //provisional
		myPlayers[i]->leftChargeJump.loop = true;

		myPlayers[i]->rightJump.PushBack({ 0,  768,64,64 });
		myPlayers[i]->rightJump.PushBack({ 64, 768,64,64 });
		myPlayers[i]->rightJump.PushBack({ 128,768,64,64 });
		myPlayers[i]->rightJump.PushBack({ 192,768,64,64 });
		myPlayers[i]->rightJump.PushBack({ 256,768,64,64 });
		myPlayers[i]->rightJump.speed = 0.2f; //provisional
		myPlayers[i]->rightJump.loop = true;

		myPlayers[i]->leftJump.PushBack({ 0,  832,64,64 });
		myPlayers[i]->leftJump.PushBack({ 64, 832,64,64 });
		myPlayers[i]->leftJump.PushBack({ 128,832,64,64 });
		myPlayers[i]->leftJump.PushBack({ 192,832,64,64 });
		myPlayers[i]->leftJump.PushBack({ 256,832,64,64 });
		myPlayers[i]->leftJump.speed = 0.2f; //provisional
		myPlayers[i]->leftJump.loop = true;
	}
	currentPlayer = myPlayers[0];

	return true;
}

bool ModulePlayer::CleanUp() { return true; };

void ModulePlayer::InitAnims()
{
	for (int i = 0; i < NUM_PLAYERS; ++i)
	{
		
		
			myPlayers[i]->rightIdle.PushBack({ 0,  0,64,64 });
			myPlayers[i]->rightIdle.PushBack({ 64, 0,64,64 });
			myPlayers[i]->rightIdle.PushBack({ 128,0,64,64 });
			myPlayers[i]->rightIdle.PushBack({ 192,0,64,64 });
			myPlayers[i]->rightIdle.PushBack({ 256,0,64,64 });
			myPlayers[i]->rightIdle.PushBack({ 320,0,64,64 });
			myPlayers[i]->rightIdle.PushBack({ 384,0,64,64 });
			myPlayers[i]->rightIdle.PushBack({ 448,0,64,64 });
			myPlayers[i]->rightIdle.PushBack({ 512,0,64,64 });
			myPlayers[i]->rightIdle.PushBack({ 576,0,64,64 });
			myPlayers[i]->rightIdle.speed = 0.1f; //provisional
			myPlayers[i]->rightIdle.loop = true;

			myPlayers[i]->currentAnim = &myPlayers[i]->rightIdle;

			myPlayers[i]->leftIdle.PushBack({ 0,64,64,64 });
			myPlayers[i]->leftIdle.PushBack({ 64,64,64,64 });
			myPlayers[i]->leftIdle.PushBack({ 128,64,64,64 });
			myPlayers[i]->leftIdle.PushBack({ 192,64,64,64 });
			myPlayers[i]->leftIdle.PushBack({ 256,64,64,64 });
			myPlayers[i]->leftIdle.PushBack({ 320,64,64,64 });
			myPlayers[i]->leftIdle.PushBack({ 384,64,64,64 });
			myPlayers[i]->leftIdle.PushBack({ 448,64,64,64 });
			myPlayers[i]->leftIdle.PushBack({ 512,64,64,64 });
			myPlayers[i]->leftIdle.PushBack({ 576,64,64,64 });
			myPlayers[i]->leftIdle.speed = 0.2f; //provisional
			myPlayers[i]->leftIdle.loop = true;

			myPlayers[i]->rightShoot.PushBack({ 0,  128,64,64 });
			myPlayers[i]->rightShoot.PushBack({ 64, 128,64,64 });
			myPlayers[i]->rightShoot.PushBack({ 128,128,64,64 });
			myPlayers[i]->rightShoot.PushBack({ 192,128,64,64 });
			myPlayers[i]->rightShoot.PushBack({ 256,128,64,64 });
			myPlayers[i]->rightShoot.PushBack({ 320,128,64,64 });
			myPlayers[i]->rightShoot.PushBack({ 384,128,64,64 });
			myPlayers[i]->rightShoot.PushBack({ 448,128,64,64 });
			myPlayers[i]->rightShoot.PushBack({ 512,128,64,64 });
			myPlayers[i]->rightShoot.PushBack({ 576,128,64,64 });
			myPlayers[i]->rightShoot.PushBack({ 640,128,64,64 });
			myPlayers[i]->rightShoot.speed = 0.2f; //provisional
			myPlayers[i]->rightShoot.loop = false;

			myPlayers[i]->leftShoot.PushBack({ 0,  192,64,64 });
			myPlayers[i]->leftShoot.PushBack({ 64, 192,64,64 });
			myPlayers[i]->leftShoot.PushBack({ 128,192,64,64 });
			myPlayers[i]->leftShoot.PushBack({ 192,192,64,64 });
			myPlayers[i]->leftShoot.PushBack({ 256,192,64,64 });
			myPlayers[i]->leftShoot.PushBack({ 320,192,64,64 });
			myPlayers[i]->leftShoot.PushBack({ 384,192,64,64 });
			myPlayers[i]->leftShoot.PushBack({ 448,192,64,64 });
			myPlayers[i]->leftShoot.PushBack({ 512,192,64,64 });
			myPlayers[i]->leftShoot.PushBack({ 576,192,64,64 });
			myPlayers[i]->leftShoot.PushBack({ 640,192,64,64 });
			myPlayers[i]->leftShoot.speed = 0.2f; //provisional
			myPlayers[i]->leftShoot.loop = false;

			myPlayers[i]->rightWalk.PushBack({ 0,  256,64,64 });
			myPlayers[i]->rightWalk.PushBack({ 64, 256,64,64 });
			myPlayers[i]->rightWalk.PushBack({ 128,256,64,64 });
			myPlayers[i]->rightWalk.PushBack({ 192,256,64,64 });
			myPlayers[i]->rightWalk.PushBack({ 256,256,64,64 });
			myPlayers[i]->rightWalk.PushBack({ 320,256,64,64 });
			myPlayers[i]->rightWalk.PushBack({ 384,256,64,64 });
			myPlayers[i]->rightWalk.PushBack({ 448,256,64,64 });
			myPlayers[i]->rightWalk.speed = 0.2f; //provisional
			myPlayers[i]->rightWalk.loop = true;

			myPlayers[i]->leftWalk.PushBack({ 0,  320,64,64 });
			myPlayers[i]->leftWalk.PushBack({ 64, 320,64,64 });
			myPlayers[i]->leftWalk.PushBack({ 128,320,64,64 });
			myPlayers[i]->leftWalk.PushBack({ 192,320,64,64 });
			myPlayers[i]->leftWalk.PushBack({ 256,320,64,64 });
			myPlayers[i]->leftWalk.PushBack({ 320,320,64,64 });
			myPlayers[i]->leftWalk.PushBack({ 384,320,64,64 });
			myPlayers[i]->leftWalk.PushBack({ 448,320,64,64 });
			myPlayers[i]->leftWalk.speed = 0.2f; //provisional
			myPlayers[i]->leftWalk.loop = true;

			myPlayers[i]->rightFall.PushBack({ 0,  384,64,64 });
			myPlayers[i]->rightFall.PushBack({ 64, 384,64,64 });
			myPlayers[i]->rightFall.PushBack({ 128,384,64,64 });
			myPlayers[i]->rightFall.PushBack({ 192,384,64,64 });
			myPlayers[i]->rightFall.speed = 0.2f; //provisional
			myPlayers[i]->rightFall.loop = true;

			myPlayers[i]->leftFall.PushBack({ 0,  448,64,64 });
			myPlayers[i]->leftFall.PushBack({ 64, 448,64,64 });
			myPlayers[i]->leftFall.PushBack({ 128,448,64,64 });
			myPlayers[i]->leftFall.PushBack({ 192,448,64,64 });
			myPlayers[i]->leftFall.speed = 0.2f; //provisional
			myPlayers[i]->leftFall.loop = true;

			myPlayers[i]->rightDeath.PushBack({ 0,  512,64,64 });
			myPlayers[i]->rightDeath.PushBack({ 64, 512,64,64 });
			myPlayers[i]->rightDeath.PushBack({ 128,512,64,64 });
			myPlayers[i]->rightDeath.PushBack({ 192,512,64,64 });
			myPlayers[i]->rightDeath.PushBack({ 256,512,64,64 });
			myPlayers[i]->rightDeath.PushBack({ 320,512,64,64 });
			myPlayers[i]->rightDeath.speed = 0.2f; //provisional
			myPlayers[i]->rightDeath.loop = false;

			myPlayers[i]->leftDeath.PushBack({ 0,  576,64,64 });
			myPlayers[i]->leftDeath.PushBack({ 64, 576,64,64 });
			myPlayers[i]->leftDeath.PushBack({ 128,576,64,64 });
			myPlayers[i]->leftDeath.PushBack({ 192,576,64,64 });
			myPlayers[i]->leftDeath.PushBack({ 256,576,64,64 });
			myPlayers[i]->leftDeath.PushBack({ 320,576,64,64 });
			myPlayers[i]->leftDeath.speed = 0.2f; //provisional
			myPlayers[i]->leftDeath.loop = false;

			myPlayers[i]->rightChargeJump.PushBack({ 0,  640,64,64 });
			myPlayers[i]->rightChargeJump.PushBack({ 64, 640,64,64 });
			myPlayers[i]->rightChargeJump.PushBack({ 128,640,64,64 });
			myPlayers[i]->rightChargeJump.PushBack({ 192,640,64,64 });
			myPlayers[i]->rightChargeJump.PushBack({ 256,640,64,64 });
			myPlayers[i]->rightChargeJump.PushBack({ 320,640,64,64 });
			myPlayers[i]->rightChargeJump.speed = 0.2f; //provisional
			myPlayers[i]->rightChargeJump.loop = true;

			myPlayers[i]->leftChargeJump.PushBack({ 0,  704,64,64 });
			myPlayers[i]->leftChargeJump.PushBack({ 64, 704,64,64 });
			myPlayers[i]->leftChargeJump.PushBack({ 128,704,64,64 });
			myPlayers[i]->leftChargeJump.PushBack({ 192,704,64,64 });
			myPlayers[i]->leftChargeJump.PushBack({ 256,704,64,64 });
			myPlayers[i]->leftChargeJump.PushBack({ 320,704,64,64 });
			myPlayers[i]->leftChargeJump.speed = 0.2f; //provisional
			myPlayers[i]->leftChargeJump.loop = true;

			myPlayers[i]->rightJump.PushBack({ 0,  768,64,64 });
			myPlayers[i]->rightJump.PushBack({ 64, 768,64,64 });
			myPlayers[i]->rightJump.PushBack({ 128,768,64,64 });
			myPlayers[i]->rightJump.PushBack({ 192,768,64,64 });
			myPlayers[i]->rightJump.PushBack({ 256,768,64,64 });
			myPlayers[i]->rightJump.speed = 0.2f; //provisional
			myPlayers[i]->rightJump.loop = true;

			myPlayers[i]->leftJump.PushBack({ 0,  832,64,64 });
			myPlayers[i]->leftJump.PushBack({ 64, 832,64,64 });
			myPlayers[i]->leftJump.PushBack({ 128,832,64,64 });
			myPlayers[i]->leftJump.PushBack({ 192,832,64,64 });
			myPlayers[i]->leftJump.PushBack({ 256,832,64,64 });
			myPlayers[i]->leftJump.speed = 0.2f; //provisional
			myPlayers[i]->leftJump.loop = true;
		
	}
}

update_status ModulePlayer::Update()
{

	bool turntaken = false;

	for (int i = 0; i < 2; ++i) {

		myPlayers[i]->currentAnim->Update();

		if (myPlayers[i]->currentAnim != nullptr)
		{
			App->renderer->Blit(myPlayers[i]->player1, myPlayers[i]->rigid->posRect.x, myPlayers[i]->rigid->posRect.y, &(myPlayers[i]->currentAnim->GetCurrentFrame()));
		}

		

		myPlayers[i]->healthBar = { myPlayers[i]->rigid->posRect.x -20 , myPlayers[i]->rigid->posRect.y -20, myPlayers[i]->HP , 20 };
		App->renderer->DrawQuad(myPlayers[i]->healthBar, 100, 200, 10, 255);


		if (turntaken) { break; }
		if (myPlayers[i] == currentPlayer) {
			if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
				if (myPlayers[i]->faseActual == Movimiento) {
					myPlayers[i]->rigid->velocity.x = 0;
					myPlayers[i]->faseActual = Fase::Disparo;
				}
				else {
					preview = false;
					myPlayers[i]->faseActual = Fase::Movimiento;
				}
			}

			switch (myPlayers[i]->faseActual) {
			case Movimiento:
				myPlayers[i]->rigid->velocity;
				/*App->renderer->Blit(player1, 50, 200);*/

				if (myPlayers[i]->rigid->isGrounded == false) {
					myPlayers[i]->rigid->acceleration.y = 500;
				}

				if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) {
					m++;
					currentMovement = &myMovement[(m % 5)];
				}

				if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_REPEAT && myPlayers[i]->rigid->isGrounded) {
					myPlayers[i]->rigid->velocity.y -= JUMP_FORCE;
					myPlayers[i]->rigid->isGrounded = false;
					myPlayers[i]->isJumping = true;
				}

				if (myPlayers[i]->isJumping) { myPlayers[i]->jumpingCnt--; }

				// Movement "tp"
				if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
					moved = true;
					myPlayers[i]->currentAnim = &myPlayers[i]->rightWalk;
					if (myPlayers[i]->myDirection != Direction::RIGHT) { ChangeDir(*myPlayers[i]); }
				}
				if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
					moved = true;
					if (myPlayers[i]->myDirection != Direction::LEFT) { ChangeDir(*myPlayers[i]); }
				}
				if (moved) {
					switch (*currentMovement) {
					case Movement::POSITION:
						PositionController(myPlayers[i]->myDirection, myPlayers[i]);
						break;
					case Movement::VELOCITY:
						VelocityController(myPlayers[i]->myDirection, myPlayers[i]);
						break;
					case Movement::ACCELERATION:
						AccelerationController(myPlayers[i]->myDirection, myPlayers[i]);
						break;
					case Movement::MOMENTUM:
						MomentumController(myPlayers[i]->myDirection, myPlayers[i]);
						break;
					case Movement::IMPULSE:
						ImpulseController(myPlayers[i]->myDirection, myPlayers[i]);
						break;
					}
				}

				if (App->input->GetKey(SDL_SCANCODE_LEFT) != KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_RIGHT) != KEY_REPEAT) {
					LOG("NOT MOVING");
					myPlayers[i]->rigid->isMoving = false;
				}
				

				if (moved == false) {
					myPlayers[i]->rigid->velocity.x = 0;
					myPlayers[i]->rigid->isMoving;
				}
				moved = false;
				break;
			case Disparo:
				preview = true;
				if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP) {
					myPlayers[i]->faseActual = Fase::Movimiento;
					myPlayers[i]->myWeapons[0].Shoot(this, App->physics, App->input->GetMouseX(), App->input->GetMouseY());
					currentPlayer = myPlayers[(i + 1) % NUM_PLAYERS];
					turntaken = true;
					preview = false;
					//ranodm windforceX y windforceY entre los numeros del -1 y 1
					
					std::random_device rd;
					std::mt19937 gen(rd());
					std::uniform_real_distribution<> dis(-1, 1);

					windForceX = std::round(dis(gen) * 1e5) / 1e5;
					windForceY = std::round(dis(gen) * 1e5) / 1e5;

					//se pasa al siguiente turno
				}
				break;
			}

			if (preview)
			{
				myPlayers[i]->myWeapons[0].PreviewShot(App->input->GetMouseX(), App->input->GetMouseY(), App->renderer->renderer, App->deltaTime.getDeltaTimeInSeconds());
			}

			//App->renderer->Blit(player2, 230, 355);
			/*App->renderer->Blit(player2, 530, 355);
			App->renderer->Blit(player2, 850, 355);*/
			//LOG(" \nXXXXXXXXXXXXXXXXXXXXXXXXXX : %f", rigid->velocity.x);

			myPlayers[i]->rigid->isGrounded = false;
		}
	}
	return UPDATE_CONTINUE;
}

void ModulePlayer::ChangeDir(Player &p) {
	//rigid->velocity.x = 0;

	if (p.myDirection == Direction::LEFT) { p.myDirection = Direction::RIGHT; }
	else if (p.myDirection == Direction::RIGHT) { p.myDirection = Direction::LEFT; }
}

void ModulePlayer::AccelerationController(Direction dir, Player* p) {

	p->rigid->isMoving = true;
	if (dir == Direction::RIGHT) {
		p->rigid->acceleration.x = ACCELERATION_VALUE;

		if (p->rigid->velocity.x < 200) {
			p->rigid->velocity.x += p->rigid->acceleration.x * App->deltaTime.getDeltaTimeInSeconds();
		}

		
	}
	else if (dir == Direction::LEFT) {
		p->rigid->acceleration.x = -ACCELERATION_VALUE;

		if (p->rigid->velocity.x > -200) {
			p->rigid->velocity.x += p->rigid->acceleration.x * App->deltaTime.getDeltaTimeInSeconds();
		}	
	}
}

void ModulePlayer::PositionController(Direction dir, Player* p) {

	if (dir == Direction::RIGHT) {
		p->rigid->posRect.x = p->rigid->posRect.x + 10;
	}
	else if (dir == Direction::LEFT) {		
		p->rigid->posRect.x = p->rigid->posRect.x -10;
	}
}


void ModulePlayer::VelocityController(Direction dir, Player* p) 
{
	if (dir == Direction::RIGHT) 
	{
		p->rigid->velocity.x = 200 ;
	}
	else if (dir == Direction::LEFT) 
	{
		p->rigid->velocity.x = -200 ;
	}
}

void ModulePlayer::ImpulseController(Direction dir, Player* p) {

	if (dir == Direction::RIGHT) {
		p->rigid->velocity.x = p->rigid->velocity.x + 20;
	}
	else if (dir == Direction::LEFT) {
		p->rigid->velocity.x = p->rigid->velocity.x - 20;
	}
}

float ModulePlayer::CalculateMomentum(Player* p) {
	float mass = 1; 
	vec2 velocity = p->rigid->velocity;

	float momentum = mass * sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));

	LOG(" \n MOmentum %f", momentum);
	return momentum;
}

void ModulePlayer::MomentumController(Direction dir, Player* p) {
	p->rigid->isMoving = true;
	float momentum = CalculateMomentum(p);

	float initialVelocity = 0.5;
	float maxVelocity = 80;

	p->rigid->acceleration.x = momentum;

	if (dir == Direction::RIGHT) {

		p->rigid->velocity.x += p->rigid->acceleration.x;
		if (p->rigid->velocity.x < initialVelocity) {
			p->rigid->velocity.x = initialVelocity;
		}	 
		if (p->rigid->velocity.x > maxVelocity) {
			p->rigid->velocity.x = maxVelocity;
		}                                                                         
	}
	else if (dir == Direction::LEFT) {
		p->rigid->velocity.x += p->rigid->acceleration.x ;

		if (p->rigid->velocity.x > -initialVelocity) {
			p->rigid->velocity.x = -initialVelocity;
		}	 
		if (p->rigid->velocity.x < -maxVelocity) {
			p->rigid->velocity.x = -maxVelocity;
		}
	}
}

void ModulePlayer::OnCollision(RigidBody* c1, RigidBody* c2) {


	if (c2->collider->type == ColliderType::BULLET ) {
		if ( c2->collider->made_explosion == false) {
			c2->StopAllMotion();
			LOG("\n EXPLODE \n");
			Explosion* explode = new Explosion(c2->collider->data.x +(c2->collider->data.w/2), c2->collider->data.y + (c2->collider->data.h/2), 30);
			App->physics->explosions.push_back(explode);
			c2->collider->made_explosion = true;
			explode->mybody = c2;

		}
	}


	if ((c1->collider->type == ColliderType::GROUND || c1->collider->type == ColliderType::BREAKABLE)  && c2->collider->type == ColliderType::PLAYER) {
		for (int i = 0; i < 2; ++i) {
			

			if (myPlayers[i]->rigid == c2) /*Comprobar que el player es el correcto*/ {
				if (!myPlayers[i]->isJumping && myPlayers[i]->rigid->collider->data.y < c1->collider->data.y) {
					myPlayers[i]->rigid->isGrounded = true;
					c2->velocity.y = 0;
					c2->acceleration.y = 0;
					// Negar movimiento vertical en seguida que choca con algo
				}

				int feet = myPlayers[i]->rigid->collider->data.y + myPlayers[i]->rigid->collider->data.h    - 6;
				if (feet < c1->collider->data.y+6)/*Above collider*/ {
					
					
					c2->posRect.y = c1->posRect.y - c2->collider->data.h - 1;
				}
				else if ( myPlayers[i]->rigid->collider->data.x < c1->collider->data.x ) /*LEFT*/ {
					c2->posRect.x -= c2->velocity.x * App->deltaTime.getDeltaTimeInSeconds();
					c2->velocity.x = (c2->velocity.x * 0.1f* 0);
				}
				else if (myPlayers[i]->rigid->collider->data.x > c1->collider->data.x)  /*RIGHT*/ {
					c2->velocity.x = (c2->velocity.x * 0.1f) * 0;
					c2->posRect.x += c2->velocity.x * App->deltaTime.getDeltaTimeInSeconds() +3;
					// Creo que el suelo esta interfiriendo apluicandole algo
					c2->velocity.x = (c2->velocity.x * 0.1f)* 0;
				}
				

				if (myPlayers[i]->jumpingCnt <= 0) {

					myPlayers[i]->isJumping = false;
					myPlayers[i]->jumpingCnt = 70;
				}
			}
		}
		
		
	}
	
	
	if (*App->physics->currentCollisionMethod == CollisionDetection::TELEPORT) {

		if (c2->collider->type == ColliderType::BULLET && c1->collider->type != ColliderType::PLAYER && c2->isMoving) {
			c2->velocity.x = 0;
			c2->velocity.y = 0;
			c2->acceleration.y = 0;

		         if (c2->collider->data.x < c1->collider->data.x) /*LEFT*/ {
		         	c2->posRect.x -= c2->velocity.x * App->deltaTime.getDeltaTimeInSeconds();
		         	c2->velocity.x = (c2->velocity.x * 0.1f * 0);
		         }
		         else if (c2->collider->data.x > c1->collider->data.x)  /*RIGHT*/ {
		         	c2->velocity.x = (c2->velocity.x * 0.1f) * 0;
		         	c2->posRect.x += c2->velocity.x * App->deltaTime.getDeltaTimeInSeconds() + 3;
		         	c2->velocity.x = (c2->velocity.x * 0.1f) * 0;
		         }

			//c2->posRect.y = c1->posRect.y - c2->collider->data.h -1; // Teleportation //
			c2->isGrounded = true;
			c2->isMoving = false;
		}
	}
	if (c1->collider->type == ColliderType::BOUNCER && (c2->collider->type == ColliderType::PLAYER || c2->collider->type == ColliderType::BULLET)) {
		float c1_center_x = c1->posRect.x + c1->collider->data.w / 2;
		float c1_center_y = c1->posRect.y + c1->collider->data.h / 2;
		float c2_center_x = c2->posRect.x + c2->collider->data.w / 2;
		float c2_center_y = c2->posRect.y + c2->collider->data.h / 2;

		float dx = c2_center_x - c1_center_x;
		float dy = c2_center_y - c1_center_y;

		if (abs(dx) > abs(dy)) {
			c2->velocity.x *= -1;
		}
		else {
			c2->velocity.y *= -1;
		}
	}

	LOG("Collision");
}

void ModulePlayer::OnExplosion(RigidBody* c1) {


	for (int i = 0; i < 2; ++i) {
		if (c1 == myPlayers[i]->rigid) {
			myPlayers[i]->HP -= 25;
			
		}
	}

}