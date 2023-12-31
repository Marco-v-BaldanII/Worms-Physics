#ifndef __Player_H__
#define __Player_H__

#include "SDL/include/SDL.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "Anim.h"

class Weapon;
enum class Direction;

enum Fase { Movimiento, Disparo };

class Player {
public:

	bool dead = false;

	SDL_Texture* playertex;
	SDL_Rect player;
	SDL_Texture* player1;
	SDL_Texture* player2;


	SDL_Rect healthBar;
	SDL_Rect movementBar;
	int HP = 100;
	int movement = 700;
	
	float oldPosX; float posDif;

	Weapon* currentWeapon = nullptr;

	Weapon* myWeapons[2] = { nullptr };

	bool isJumping = false;
	int jumpingCnt = 70;

	// Movement
	RigidBody* rigid;

	Direction myDirection;

	
	int turno = 0;
	
	Fase faseActual = Fase::Movimiento;
	bool shoted = false;
	bool moved = false;

	Anim* currentAnim = nullptr;

	Anim rightIdle, leftIdle;
	Anim rightWalk, leftWalk;
	Anim rightChargeJump, leftChargeJump;
	Anim rightJump, leftJump;
	Anim rightFall, leftFall;
	Anim rightDeath, leftDeath;
	Anim rightShoot, leftShoot;

	SDL_Texture* GunSelection = nullptr;
	Anim* currentSelection = nullptr;
	Anim BabySelected;
	Anim BoxSelected;
	Anim parachuteSelected;
	Anim noneSelected;
};



#endif