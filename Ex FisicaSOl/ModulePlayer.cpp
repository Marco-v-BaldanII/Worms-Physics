#include <iostream>
#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"


#define ACCELERATION_VALUE 90;
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

	

	App->physics->bodies.push_back(rigid);
	rigid->acceleration.y;
	rigid->ID = 2;

	return true;
}

bool ModulePlayer::CleanUp() { return true; };

update_status ModulePlayer::Update()
{
	bool moved = false;
	rigid->velocity;
	/*App->renderer->Blit(player1, 50, 200);*/

	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) {
		m++;
		currentMovement = &myMovement[(m % 5)];

	}

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
	else if (App->input->GetKey(SDL_SCANCODE_LEFT) != KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_RIGHT) != KEY_REPEAT){
		LOG("NOT MOVING");
		
		rigid->isMoving = false;
	}
	

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {

	}

	if (moved == false) {
		rigid->velocity.x = 0;
		rigid->isMoving;
	}

	App->renderer->Blit(player1, rigid->posRect.x, rigid->posRect.y);
	App->renderer->Blit(player2, 230, 355);
	App->renderer->Blit(player2, 530, 355);
	App->renderer->Blit(player2, 850, 355);
	LOG(" \nXXXXXXXXXXXXXXXXXXXXXXXXXX : %f", rigid->velocity.x);
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


void ModulePlayer::VelocityController(Direction dir) {

	if (dir == Direction::RIGHT) {
		rigid->velocity.x = 200 ;

	}
	else if (dir == Direction::LEFT) {

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

	if (c1->collider->type == ColliderType::GROUND) {

		isGrounded = true;


	}

	LOG("Collision");

}