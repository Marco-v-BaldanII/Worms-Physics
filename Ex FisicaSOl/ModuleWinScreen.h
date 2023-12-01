#pragma once
#include "Module.h"
#include "Animation.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Anim.h"
#include "Timer.h"


class ModuleWinScreen : public Module
{
public:
	ModuleWinScreen(Application* app, bool start_enabled = true);
	~ModuleWinScreen();

	bool Start();
	update_status Update();
	bool CleanUp();


	SDL_Texture* P1texture;
	SDL_Texture* P2texture;
	
	void Player1Wins();
	void Player2Wins();

	Timer P1timer;
	Timer P2Timer;

	bool P1Win = false;
	bool P2Win = false;

};