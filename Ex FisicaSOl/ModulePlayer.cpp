#include <iostream>
#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
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
	for (int i = 0; i < 2; ++i) {
		myPlayers[i] = new Player();
		myPlayers[i]->rigid = new RigidBody();


		LOG("Loading player");
		myPlayers[i]->player1 = App->textures->Load("Assets/images/tirachinas.png");
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
		myPlayers[i]->rigid->acceleration.y = 90;
		myPlayers[i]->rigid->ID = 2;
		myPlayers[i]->shoted = false;
	}
	return true;
}

bool ModulePlayer::CleanUp() { return true; };

update_status ModulePlayer::Update()
{
	for (int i = 0; i < 2; ++i) {

		if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
			if (myPlayers[i]->faseActual == Movimiento) {
				myPlayers[i]->rigid->velocity.x = 0;
				myPlayers[i]->faseActual = Fase::Disparo;
			}
			else {
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
				if (myPlayers[i]->myDirection != Direction::RIGHT) { ChangeDir(*myPlayers[i]); }
				if (*currentMovement == Movement::POSITION) {
					PositionController(Direction::RIGHT, myPlayers[i]);
				}
			}
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
				moved = true;
				if (myPlayers[i]->myDirection != Direction::LEFT) { ChangeDir(*myPlayers[i]); }
				if (*currentMovement == Movement::POSITION) {
					PositionController(Direction::LEFT, myPlayers[i]);
				}
			}

			// Movement "velocity set"
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
				moved = true;
				if (myPlayers[i]->myDirection != Direction::RIGHT) { ChangeDir(*myPlayers[i]); }
				if (*currentMovement == Movement::VELOCITY) {
					VelocityController(Direction::RIGHT, myPlayers[i]);
				}
			}
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {
				moved = true;
				if (myPlayers[i]->myDirection != Direction::LEFT) { ChangeDir(*myPlayers[i]); }
				if (*currentMovement == Movement::VELOCITY) {
					VelocityController(Direction::LEFT, myPlayers[i]);
				}
			}

			// Movement "Impulse change"
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
				moved = true;
				if (myPlayers[i]->myDirection != Direction::RIGHT) { ChangeDir(*myPlayers[i]); }
				if (*currentMovement == Movement::IMPULSE) {
					ImpulseController(Direction::RIGHT, myPlayers[i]);
				}
			}
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
				moved = true;
				if (myPlayers[i]->myDirection != Direction::LEFT) { ChangeDir(*myPlayers[i]); }
				if (*currentMovement == Movement::IMPULSE) {
					ImpulseController(Direction::LEFT, myPlayers[i]);
				}
			}

			// Momentum
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
				moved = true;
				if (myPlayers[i]->myDirection != Direction::RIGHT) { ChangeDir(*myPlayers[i]); }
				if (*currentMovement == Movement::MOMENTUM) {
					MomentumController(Direction::RIGHT, myPlayers[i]);
				}
			}
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
				moved = true;
				if (myPlayers[i]->myDirection != Direction::LEFT) { ChangeDir(*myPlayers[i]); }
				if (*currentMovement == Movement::MOMENTUM) {
					MomentumController(Direction::LEFT, myPlayers[i]);
				}
			}

			// MOVEMENT Acelerando Progresivo
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
				moved = true;
				if (myPlayers[i]->myDirection != Direction::RIGHT) { ChangeDir(*myPlayers[i]); }
				if (*currentMovement == Movement::ACCELERATION) {
					AccelerationController(Direction::RIGHT, myPlayers[i]);
				}
			}
			else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
				moved = true;
				if (myPlayers[i]->myDirection != Direction::LEFT) { ChangeDir(*myPlayers[i]); }
				if (*currentMovement == Movement::ACCELERATION) {
					AccelerationController(Direction::LEFT, myPlayers[i]);
				}
			}
			else if (App->input->GetKey(SDL_SCANCODE_LEFT) != KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_RIGHT) != KEY_REPEAT) {
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
				myPlayers[i]->myWeapons[0].Shoot(this, App->physics); //aqui disparamos el arma
				//power = 0; //y aqui reseteariamos la potencia a 0
				//metdo para pasar al siguiente player
			}
			break;
		}

		App->renderer->Blit(myPlayers[i]->player1, myPlayers[i]->rigid->posRect.x, myPlayers[i]->rigid->posRect.y);
		//App->renderer->Blit(player2, 230, 355);
		/*App->renderer->Blit(player2, 530, 355);
		App->renderer->Blit(player2, 850, 355);*/
		//LOG(" \nXXXXXXXXXXXXXXXXXXXXXXXXXX : %f", rigid->velocity.x);

		myPlayers[i]->rigid->isGrounded = false;
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

	if (c1->collider->type == ColliderType::GROUND && c2->collider->type != ColliderType::BULLET && c1->collider->type != ColliderType::BULLET) {
		for (int i = 0; i < 2; ++i) {
			if (!myPlayers[i]->isJumping) {
				myPlayers[i]->rigid->isGrounded = true;
				c2->velocity.y = 0;
				c2->acceleration.y = 0;

			}
			c2->posRect.y = c1->posRect.y - c2->collider->data.h + 1;/*altura player*/

			LOG("MAKAKOOOOOOOOOOOOOOOOOOO");
			if (myPlayers[i]->jumpingCnt <= 0) {

				myPlayers[i]->isJumping = false;
				myPlayers[i]->jumpingCnt = 90;
			}
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