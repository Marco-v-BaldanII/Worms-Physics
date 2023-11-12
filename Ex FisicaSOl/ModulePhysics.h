#pragma once
#include "Module.h"
#include "Globals.h"
#include <list>

struct vec2 {
    float x;
    float y;
};
class Collider {
public:
    
    SDL_Rect data;

    Collider(SDL_Rect r) {
        data = r;
    }

};


class RigidBody {

public:
    SDL_Rect posRect;
    vec2 velocity;
    vec2 acceleration;
    SDL_Texture* bird;
    
    bool isMoving = false;

    int ID = 0;
    Collider* collider = nullptr;

    void CreateCollider(SDL_Rect& col) {
        collider = new Collider(col);
    }

};

class Bullet : public RigidBody {
public:

};




class ModulePhysics : public Module
{
public:
    ModulePhysics(Application* app, bool start_enabled = true);
    ~ModulePhysics();

    void IntegratorEuler(float deltaTime, SDL_Rect& rect, vec2& velocity, vec2& acceleration);
    void IntegratorEuler2(float deltaTime, SDL_Rect& rect, vec2& velocity, vec2& acceleration);
    void IntegratorVerlet(float deltaTime, SDL_Rect& rect, vec2& velocity, vec2& acceleration);

    bool Start();
    update_status PreUpdate();
    update_status PostUpdate();
    bool CleanUp();
    std::list<RigidBody*> bodies;

private:
    vec2 canon = {10, 150};
    bool debug;
    SDL_Texture* bird;
};