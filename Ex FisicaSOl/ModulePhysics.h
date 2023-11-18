#pragma once
#include "Module.h"
#include "Globals.h"
#include <list>


enum ColliderType {

    GROUND,
    BULLET,
    PLAYER


};

enum CollisionDetection {
    TELEPORT,
    ITERATIVE,
    RAYCAST
};


struct vec2 {
    float x;
    float y;
};
class Collider {
public:
    
    SDL_Rect data;
    ColliderType type;

    Collider(SDL_Rect r, ColliderType type, Module* Listener) {
        data = r;
        this->type = type;
        listener = Listener;
    }

    bool Intersects(SDL_Rect* r) const
    {
        if (r != nullptr && &data != nullptr) {
            if (data.w != 0 && data.h != 0 && r->w != 0 && r->h != 0) {
                // returns true if there has been an intersection
                return (data.x < r->x + r->w &&
                    data.x + data.w > r->x &&
                    data.y < r->y + r->h &&
                    data.h + data.y > r->y);
            }
        }

        else {
            return false;
        }

    }

    Module* listener;
    
};


class RigidBody {

public:

    RigidBody(SDL_Rect posRect) {
        this->posRect = posRect;
    }

    RigidBody() {};

    SDL_Rect posRect;
    vec2 velocity;
    vec2 acceleration;
    SDL_Texture* bird;
    
    bool isMoving = false;

    bool isGrounded = true;

    int ID = 0;
    Collider* collider = nullptr;

    void CreateCollider(SDL_Rect& col, ColliderType type, Module* listener) {
        
        collider = new Collider(col , type, listener);
        
    }

    void StopAllMotion() {
        velocity.x = 0; velocity.y = 0;
        acceleration.x = 0; acceleration.y = 0;
        isMoving = false;
    }


};



class Target {

public:
    RigidBody rigid;



};


class ModulePhysics : public Module
{
public:
    ModulePhysics(Application* app, bool start_enabled = true);
    ~ModulePhysics();

    void IntegratorEuler(float deltaTime, SDL_Rect& rect, vec2& velocity, vec2& acceleration);
    void IntegratorEuler2(float deltaTime, SDL_Rect& rect, vec2& velocity, vec2& acceleration);
    void IntegratorVerlet(float deltaTime, SDL_Rect& rect, vec2& velocity, vec2& acceleration);

    //void UpdateBullets(list<RigidBody>bullets);

    bool Start();
    update_status PreUpdate();
    update_status PostUpdate();
    bool CleanUp();
    void IterativeCollisionIntegration(RigidBody* c1, RigidBody* c2);
    void RayCast(RigidBody* c1);

    std::list<RigidBody*> bodies;
    RigidBody* corpses[50] = { nullptr };

    CollisionDetection collisionMethod[3];
    CollisionDetection* currentCollisionMethod;

    int c = 2;

private:
    vec2 canon = {10, 150};
    bool debug;
    SDL_Texture* bird;
};