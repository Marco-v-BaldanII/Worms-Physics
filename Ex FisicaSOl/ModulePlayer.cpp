#include <iostream>
#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"

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
	rigid = new RigidBody();


	LOG("Loading player");
	player1 = App->textures->Load("Assets/images/tirachinas.png");
	player2 = App->textures->Load("Assets/images/PIG.png");
	player = { 10,10,10,10 };


	rigid->posRect.x = 50;
	rigid->posRect.y = 200;
	rigid->CreateCollider(SDL_Rect{ 0,0,65,87 }, ColliderType::PLAYER, this);


	myMovement[0] = Movement::POSITION;
	myMovement[1] = Movement::VELOCITY;
	myMovement[2] = Movement::ACCELERATION;
	myMovement[3] = Movement::IMPULSE;
	myMovement[4] = Movement::MOMENTUM;

	myDirection = Direction::RIGHT;

	currentMovement = &myMovement[3];

	Weapon weapon1; weapon1.name = "birdBazooka";
	myWeapons = new Weapon[NUM_WEAPONS];
	myWeapons[0] = weapon1;

	App->physics->bodies.push_back(rigid);
	rigid->acceleration.y = 90;
	rigid->ID = 2;
	shoted = false;

	return true;
}

bool ModulePlayer::CleanUp() { return true; };

update_status ModulePlayer::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		if (faseActual == Movimiento) {
			rigid->velocity.x = 0;
			faseActual = Disparo;
		}
		else {
			faseActual = Movimiento;
		}
	}
	
	switch (faseActual) {
	case Movimiento:
		rigid->velocity;
		/*App->renderer->Blit(player1, 50, 200);*/

		if (rigid->isGrounded == false) {
			rigid->acceleration.y = 500;
		}

		if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) {
			m++;
			currentMovement = &myMovement[(m % 5)];
		}

		if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_REPEAT && rigid->isGrounded) {
			rigid->velocity.y -= JUMP_FORCE;
			rigid->isGrounded = false;
			isJumping = true;
		}

		if (isJumping) { jumpingCnt--; }

		// Movement "tp"
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
			moved = true;
			if (myDirection != Direction::RIGHT) { ChangeDir(); }
			if (*currentMovement == Movement::POSITION) {
				PositionController(Direction::RIGHT);
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
			moved = true;
			if (myDirection != Direction::LEFT) { ChangeDir(); }
			if (*currentMovement == Movement::POSITION) {
				PositionController(Direction::LEFT);
			}
		}

		// Movement "velocity set"
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
			moved = true;
			if (myDirection != Direction::RIGHT) { ChangeDir(); }
			if (*currentMovement == Movement::VELOCITY) {
				VelocityController(Direction::RIGHT);
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {
			moved = true;
			if (myDirection != Direction::LEFT) { ChangeDir(); }
			if (*currentMovement == Movement::VELOCITY) {
				VelocityController(Direction::LEFT);
			}
		}

		// Movement "Impulse change"
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
			moved = true;
			if (myDirection != Direction::RIGHT) { ChangeDir(); }
			if (*currentMovement == Movement::IMPULSE) {
				ImpulseController(Direction::RIGHT);
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
			moved = true;
			if (myDirection != Direction::LEFT) { ChangeDir(); }
			if (*currentMovement == Movement::IMPULSE) {
				ImpulseController(Direction::LEFT);
			}
		}

		// Momentum
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
			moved = true;
			if (myDirection != Direction::RIGHT) { ChangeDir(); }
			if (*currentMovement == Movement::MOMENTUM) {
				MomentumController(Direction::RIGHT);
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
			moved = true;
			if (myDirection != Direction::LEFT) { ChangeDir(); }
			if (*currentMovement == Movement::MOMENTUM) {
				MomentumController(Direction::LEFT);
			}
		}

		// MOVEMENT Acelerando Progresivo
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
			moved = true;
			if (myDirection != Direction::RIGHT) { ChangeDir(); }
			if (*currentMovement == Movement::ACCELERATION) {
				AccelerationController(Direction::RIGHT);
			}
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
			moved = true;
			if (myDirection != Direction::LEFT) { ChangeDir(); }
			if (*currentMovement == Movement::ACCELERATION) {
				AccelerationController(Direction::LEFT);
			}
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) != KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_RIGHT) != KEY_REPEAT) {
			LOG("NOT MOVING");
			rigid->isMoving = false;
		}

		if (moved == false) {
			rigid->velocity.x = 0;
			rigid->isMoving;
		}
		break;
	case Disparo:
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
			//+= 1 al angulo
		}
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
			//-= 1 al angulo
		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
			//+= 1 al power
		}
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP) {
			myWeapons[0].Shoot(this, App->physics); //aqui disparamos el arma
			//power = 0; //y aqui reseteariamos la potencia a 0
			//metdo para pasar al siguiente player
		}
		break;
	}

	App->renderer->Blit(player1, rigid->posRect.x, rigid->posRect.y);
	App->renderer->Blit(player2, 230, 355);
	App->renderer->Blit(player2, 530, 355);
	App->renderer->Blit(player2, 850, 355);
	LOG(" \nXXXXXXXXXXXXXXXXXXXXXXXXXX : %f", rigid->velocity.x);

	rigid->isGrounded = false;

	return UPDATE_CONTINUE;
}

void ModulePlayer::ChangeDir() {
	//rigid->velocity.x = 0;

	if (myDirection == Direction::LEFT) { myDirection = Direction::RIGHT; }
	else if (myDirection == Direction::RIGHT) { myDirection = Direction::LEFT; }
}

void ModulePlayer::AccelerationController(Direction dir) {

	rigid->isMoving = true;
	if (dir == Direction::RIGHT) {
		rigid->acceleration.x = ACCELERATION_VALUE;

		if (rigid->velocity.x < 200) {
			rigid->velocity.x += rigid->acceleration.x * App->deltaTime.getDeltaTimeInSeconds();
		}

		
	}
	else if (dir == Direction::LEFT) {
		rigid->acceleration.x = -ACCELERATION_VALUE;

		if (rigid->velocity.x > -200) {
			rigid->velocity.x += rigid->acceleration.x * App->deltaTime.getDeltaTimeInSeconds();
		}	
	}
}

void ModulePlayer::PositionController(Direction dir) {

	if (dir == Direction::RIGHT) {
		rigid->posRect.x = rigid->posRect.x + 10;
	}
	else if (dir == Direction::LEFT) {		
		rigid->posRect.x = rigid->posRect.x -10;
	}
}


void ModulePlayer::VelocityController(Direction dir) 
{
	if (dir == Direction::RIGHT) 
	{
		rigid->velocity.x = 200 ;
	}
	else if (dir == Direction::LEFT) 
	{
		rigid->velocity.x = -200 ;
	}
}

void ModulePlayer::ImpulseController(Direction dir) {

	if (dir == Direction::RIGHT) {
		rigid->velocity.x = rigid->velocity.x + 20;
	}
	else if (dir == Direction::LEFT) {
		rigid->velocity.x = rigid->velocity.x - 20;
	}
}

float ModulePlayer::CalculateMomentum() {
	float mass = 1; 
	vec2 velocity = rigid->velocity;

	float momentum = mass * sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));

	LOG(" \n MOmentum %f", momentum);
	return momentum;
}

void ModulePlayer::MomentumController(Direction dir) {
	rigid->isMoving = true;
	float momentum = CalculateMomentum();

	float initialVelocity = 0.5;
	float maxVelocity = 80;

	rigid->acceleration.x = momentum;

	if (dir == Direction::RIGHT) {

		rigid->velocity.x += rigid->acceleration.x;
		if (rigid->velocity.x < initialVelocity) {
			rigid->velocity.x = initialVelocity;
		}
		if (rigid->velocity.x > maxVelocity) {
			rigid->velocity.x = maxVelocity;
		} 
	}
	else if (dir == Direction::LEFT) {
		rigid->velocity.x += rigid->acceleration.x ;

		if (rigid->velocity.x > -initialVelocity) {
			rigid->velocity.x = -initialVelocity;
		}
		if (rigid->velocity.x < -maxVelocity) {
			rigid->velocity.x = -maxVelocity;
		}
	}
}

void ModulePlayer::OnCollision(RigidBody* c1, RigidBody* c2) {

	if (c1->collider->type == ColliderType::GROUND && c2->collider->type != ColliderType::BULLET && c1->collider->type != ColliderType::BULLET) {
		if (!isJumping) {
			rigid->isGrounded = true;
			c2->velocity.y = 0;
			c2->acceleration.y = 0;

		}
		
			c2->posRect.y = c1->posRect.y - c2->collider->data.h + 1;/*altura player*/
		
		LOG("MAKAKOOOOOOOOOOOOOOOOOOO");
		if (jumpingCnt <= 0) {
			
			isJumping = false;
			jumpingCnt = 90;
		}
	}
	if (*App->physics->currentCollisionMethod == CollisionDetection::TELEPORT) {

		if (c2->collider->type == ColliderType::BULLET && c1->collider->type != ColliderType::PLAYER && c2->isMoving) {
			c2->velocity.x = 0;
			c2->velocity.y = 0;
			c2->acceleration.y = 0;
			c2->posRect.y = c1->posRect.y - c2->collider->data.h -1; // Teleportation //
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