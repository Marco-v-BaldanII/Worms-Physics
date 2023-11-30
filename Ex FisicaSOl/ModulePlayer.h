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


	int m = 2;

	bool shoted = false;
	bool moved = false;
	bool preview = false;

	float windForceX = 0.1f;
	float windForceY = 0.1f;

	float CalculateMomentum(Player* p);
};

class Weapon {
public:

	const char* name;
	Player* player;
	std::list<Bullet*> bodies;
	std::list<parachuteBomb*> bombs;

	virtual void MissileIncoming(ModulePlayer* pManager, ModulePhysics* physics, int mouseX, int mouseY) {

		parachuteBomb* bomb = new parachuteBomb;
		bomb->isMoving = true;
		bomb->posRect.x = mouseX;
		bomb->posRect.y = 0;
		bomb->posRect.w = 10;
		bomb->posRect.h = 10;

		float speed = 200;

		bomb->velocity.y = speed;

		bomb->acceleration = { 0,0 };
		SDL_Rect r = { 0,0,40,40 };
		bomb->CreateCollider(r, ColliderType::BULLET, pManager);
		bomb->collider->made_explosion = false;
		physics->bombs.push_back(bomb);
	}

	virtual void Shoot(ModulePlayer* pManager, ModulePhysics* physics, int mouseX, int mouseY) {

		Bullet* bullet = new Bullet;
		bullet->isMoving = true;
		bullet->posRect.x = player->rigid->posRect.x;
		bullet->posRect.y = player->rigid->posRect.y-50;
		bullet->posRect.w = 10;
		bullet->posRect.h = 10;

		float dx = mouseX - bullet->posRect.x;
		float dy = bullet->posRect.y - mouseY + 50;
		float mag = std::sqrt(dx * dx + dy * dy);

		float speed = std::sqrt(2 * 981 * mag);
		float maxSpeed = 1000;
		if (speed > maxSpeed) {
			speed = maxSpeed;
		}
		float angle = std::atan2(dy, dx);

		bullet->velocity.x = speed * std::cos(angle);
		bullet->velocity.y = -speed * std::sin(angle);

		bullet->acceleration = { 0,981 };
		SDL_Rect r = { 0,0,40,40 };
		bullet->CreateCollider(r, ColliderType::BULLET, pManager);
		bullet->collider->made_explosion = false;
		physics->bodies.push_back(bullet);

		static char title[400];
	}

	virtual void PreviewShot(int mouseX, int mouseY, SDL_Renderer* renderer, float delta) {

		float x = player->rigid->posRect.x;
		float y = player->rigid->posRect.y;

		float dx = mouseX - x;
		float dy = y - mouseY + 30;
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
			x += vx * dt;
			y += vy * dt;
			vx += ax * dt;
			vy += ay * dt;

			if (vy > 0) {
				fallingSteps++;
			}

			if (fallingSteps > 5) {
				break;
			}

			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			for (int dx = -1; dx <= 1; dx++) {
				for (int dy = -1; dy <= 1; dy++) {
					SDL_RenderDrawPoint(renderer, x + dx, y + dy);
				}
			}
		}
	}
};

#endif