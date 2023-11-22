#ifndef __Player_H__
#define __Player_H__

#include "SDL/include/SDL.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"

class Weapon;
enum class Direction;

enum Fase { Movimiento, Disparo };

class Player {
public:

	

	SDL_Texture* playertex;
	SDL_Rect player;
	SDL_Texture* player1;
	SDL_Texture* player2;

	

	Weapon* myWeapons;

	bool isJumping = false;
	int jumpingCnt = 90;

	// Movement
	RigidBody* rigid;

	Direction myDirection;

	
	int turno = 0;
	
	Fase faseActual = Fase::Movimiento;
	bool shoted = false;
	bool moved = false;


};

#endif