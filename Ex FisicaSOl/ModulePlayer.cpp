#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"

#define ACCELERATION_VALUE 35;
#define MaxSpeed 4;

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	player1 = App->textures->Load("Assets/images/tirachinas.png");
	player2 = App->textures->Load("Assets/images/PIG.png");
	player = { 10,10,10,10 };


	rigid.posRect.x = 50;
	rigid.posRect.y = 200;
	rigid.velocity.x = 10;

	myMovement[0] = Movement::POSITION;
	myMovement[1] = Movement::VELOCITY;
	myMovement[2] = Movement::ACCELERATION;
	myMovement[3] = Movement::IMPULSE;
	myMovement[4] = Movement::MOMENTUM;

	myDirection = Direction::RIGHT;

	currentMovement = &myMovement[2];
	return true;
}

bool ModulePlayer::CleanUp() { return true; };

update_status ModulePlayer::Update()
{

	/*App->renderer->Blit(player1, 50, 200);*/

	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) {
		m++;
		currentMovement = &myMovement[(m % 5)];

	}

	// MOVEMENT
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		if (myDirection != Direction::RIGHT) { ChangeDir(); }
		if (*currentMovement == Movement::ACCELERATION) {
			AccelerationController(Direction::RIGHT);
		}



	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		if (myDirection != Direction::LEFT) { ChangeDir(); }
		if (*currentMovement == Movement::ACCELERATION) {
			AccelerationController(Direction::LEFT);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {

	}



	App->renderer->Blit(player1, rigid.posRect.x, rigid.posRect.y);
	App->renderer->Blit(player2, 230, 355);
	App->renderer->Blit(player2, 530, 355);
	App->renderer->Blit(player2, 850, 355);
	return UPDATE_CONTINUE;
}

void ModulePlayer::ChangeDir() {
	rigid.velocity.x = 0;

	if (myDirection == Direction::LEFT) { myDirection = Direction::RIGHT; }
	else if (myDirection == Direction::RIGHT) { myDirection = Direction::LEFT; }
}

void ModulePlayer::AccelerationController(Direction dir) {

	if (dir == Direction::RIGHT) {
		rigid.acceleration.x = ACCELERATION_VALUE;

		if (rigid.velocity.x < 90) {
			rigid.velocity.x += rigid.acceleration.x * App->deltaTime.getDeltaTimeInSeconds();
		}

		rigid.posRect.x += rigid.velocity.x * App->deltaTime.getDeltaTimeInSeconds();
	}
	else if (dir == Direction::LEFT) {
		rigid.acceleration.x = -ACCELERATION_VALUE;

		if (rigid.velocity.x > -90) {
			rigid.velocity.x += rigid.acceleration.x * App->deltaTime.getDeltaTimeInSeconds();
		}

		rigid.posRect.x += rigid.velocity.x * App->deltaTime.getDeltaTimeInSeconds();
	}

}

