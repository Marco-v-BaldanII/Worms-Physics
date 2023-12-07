#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#pragma once
#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModulePhysics.h"
#include "Player.h"
#include "Application.h"
#include <string>
#include <cmath>
#include "Timer.h"


#define NUM_WEAPONS 1

class Weapon;

struct Object
{
	SDL_Texture* graphic;
	uint fx;

	Object() : graphic(NULL)
	{}
};

struct iPoint {
	int x;
	int y;
};

struct fPoint {
	float x;
	float y;
};



enum Movement {
	POSITION,
	VELOCITY,
	ACCELERATION,
	MOMENTUM,
	IMPULSE
};


enum class Direction {
	RIGHT,
	LEFT


};


class parachuteBomb : public RigidBody {

	circle explosionArea;

public:
	Collider* explosionCollider;
	SDL_Texture* texture = nullptr;

	void SetExplosionRadius(int rad) { explosionArea.r = rad; }

	parachuteBomb() :RigidBody() {
		explosionArea.r = 2;
		explosionArea.x = 0;
		explosionArea.y = 0;
	}

	parachuteBomb(int radius) :RigidBody() {
		explosionArea.r = radius;
	}

	void CollisionBehaviour(Module* listener) override {

		/*SDL_Rect explosion = { collider->data.x, collider->data.y, collider->data.w * 2, collider->data.h * 2 };
				explosionCollider = new Collider(explosion, ColliderType::GROUND, listener);*/
	}

};

class Bullet : public RigidBody {

	circle explosionArea;

public:
	Collider* explosionCollider;
	SDL_Texture* texture = nullptr;

	void SetExplosionRadius(int rad) { explosionArea.r = rad; }

	Bullet() :RigidBody() {
		explosionArea.r = 2;
		explosionArea.x = 0;
		explosionArea.y = 0;
	}

	Bullet(int radius) :RigidBody() {
		explosionArea.r = radius;
	}

	

	void CollisionBehaviour(Module* listener) override{

		/*SDL_Rect explosion = { collider->data.x, collider->data.y, collider->data.w * 2, collider->data.h * 2 };
		explosionCollider = new Collider(explosion, ColliderType::GROUND, listener);*/


	}

};



class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();
	void AnimationLogic();
	
	Player* myPlayers[2];
	Player* currentPlayer;
	int font;

	void ResetMatch();

public:
	// Movement
	

	void ChangeDir(Player &p);
	void AccelerationController(Direction dir , Player* p);
	void PositionController(Direction dir, Player* p);
	void VelocityController(Direction dir, Player* p);
	void ImpulseController(Direction dir , Player* p);
	void MomentumController(Direction dir, Player* p);
	void OnCollision(RigidBody* c1, RigidBody* c2);
	void OnExplosion(RigidBody* c1);

	void DebugText();

	void Shoot();
	int turno = 0;
	
	

	Movement myMovement[5];
	Movement* currentMovement;

	void ChangeTurn();

	int m = 2;

	bool shoted = false;
	bool moved = false;
	bool preview = false;

	bool firstTurn = false;

	float windForceX = 1.1f;
	float windForceY = 1.1f;

	const float vell = 0.05;

	float CalculateMomentum(Player* p);
};

class Weapon {
public:

	const char* name;
	Player* player;
	std::list<Bullet*> bodies;
	std::list<parachuteBomb*> bombs;
	float apuntarx;
	float apuntary;

	virtual void MissileIncoming(ModulePlayer* pManager, ModulePhysics* physics, int mouseX, int mouseY) {

		parachuteBomb* bomb = new parachuteBomb;
		bomb->isMoving = true;
		bomb->posRect.x = mouseX;
		bomb->posRect.y = 0;
		bomb->posRect.w = 10;
		bomb->posRect.h = 10;

		float speed = 2;

		bomb->velocity.y = speed;

		bomb->acceleration = { 0,0 };
		SDL_Rect r = { 0,0,64,64 };
		bomb->CreateCollider(r, ColliderType::AID, pManager);
		bomb->collider->made_explosion = false;
		physics->bombs.push_back(bomb);
	}

	virtual void Shoot(ModulePlayer* pManager, ModulePhysics* physics, int mouseX, int mouseY) {

		Bullet* bullet = new Bullet;
		bullet->isMoving = true;

		if (mouseX > player->rigid->posRect.x)
		{
			apuntarx = player->rigid->posRect.x + 70;
			apuntary = player->rigid->posRect.y - 5;
		}
		else if (mouseX < player->rigid->posRect.x)
		{
			apuntarx = player->rigid->posRect.x - 50;
			apuntary = player->rigid->posRect.y - 5;
		}

		bullet->posRect.x = apuntarx;
		bullet->posRect.y = apuntary;
		bullet->posRect.w = 10;
		bullet->posRect.h = 10;

		float dx = mouseX - apuntarx;
		float dy = apuntary - mouseY + 50;
		float mag = std::sqrt(dx * dx + dy * dy);

		float speed = std::sqrt(2 * PIXELS_TO_METERS(981) * mag);
		float maxSpeed = PIXELS_TO_METERS(1000);
		if (speed > maxSpeed) {
			speed = maxSpeed;
		}
		float angle = std::atan2(dy, dx);

		bullet->velocity.x = speed * std::cos(angle);
		bullet->velocity.y = -speed * std::sin(angle);

		bullet->acceleration = { 0, PIXELS_TO_METERS(981) };
		SDL_Rect r = { 0,0,32,32 };
		bullet->CreateCollider(r, ColliderType::BULLET, pManager);
		bullet->collider->made_explosion = false;
		physics->bodies.push_back(bullet);

	}

	virtual void PreviewShot(int mouseX, int mouseY, SDL_Renderer* renderer, float delta) {

		if (mouseX > player->rigid->posRect.x)
		{
			apuntarx = player->rigid->posRect.x + 70;
			apuntary = player->rigid->posRect.y + 10;
		}
		else
		{
			apuntarx = player->rigid->posRect.x - 6;
			apuntary = player->rigid->posRect.y + 10;
		}

		float dx = mouseX - apuntarx;
		float dy = apuntary - mouseY + 30;
		float mag = std::sqrt(dx * dx + dy * dy);
		float speed = std::sqrt(2 * 981 * mag);
		float maxSpeed = 1000;
		if (speed > maxSpeed) {
			speed = maxSpeed;
		}
		float angle = std::atan2(dy, dx);
		float vx = speed * std::cos(angle);
		float vy = -speed * std::sin(angle);

		float ax = 0;
		float ay = 981;

		float dt = delta * 5;

		int fallingSteps = 0;

		for (int i = 0; i < 1000; i++) {
			apuntarx += vx * dt;
			apuntary += vy * dt;
			vx += ax * dt;
			vy += ay * dt;

			if (vy > 0) {
				fallingSteps++;
			}

			if (fallingSteps > 3) {
				break;
			}

			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			for (int dx = -1; dx <= 1; dx++) {
				for (int dy = -1; dy <= 1; dy++) {
					SDL_RenderDrawPoint(renderer, apuntarx + dx, apuntary + dy);
				}
			}
		}
	}
};

#endif