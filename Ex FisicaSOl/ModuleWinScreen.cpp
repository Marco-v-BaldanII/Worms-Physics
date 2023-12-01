#include "Globals.h"
#include "Application.h"
#include "ModuleWinScreen.h"
#include "ModulePhysics.h"

#define RESULT_TIME 6

ModuleWinScreen::ModuleWinScreen(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleWinScreen::~ModuleWinScreen()
{}

// Load assets
bool ModuleWinScreen::Start()
{
	
	P1texture = App->textures->Load("Assets/images/win screen.png");
	P2texture = App->textures->Load("Assets/images/win screen2.png");

	return true;
}

// Load assets
bool ModuleWinScreen::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleWinScreen::Update()
{

	if (P1Win) {
		App->renderer->Blit(P1texture, 0, 0);
	}
	if (P2Win) {
		App->renderer->Blit(P2texture, 0, 0);
	}

	if ((P1Win || P2Win ) && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		P1Win = false;
		P2Win = false;
		App->player->ResetMatch();
	}

	return UPDATE_CONTINUE;
}


void ModuleWinScreen::Player1Wins() {

	P1Win = true;


}

void ModuleWinScreen::Player2Wins() {
	P2Win = true;
}
