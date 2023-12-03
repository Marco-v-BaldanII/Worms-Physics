#pragma once
#include "Module.h"
#include "Globals.h"
#include <list>
#include "Timer.h"
#include "ModulePlayer.h"
#include "Anim.h"
#include <chrono>
using namespace std::chrono;

#define METERS_TO_PIXELS(meters) ((int)((meters) * 50)) // Assuming 1 meter = 50 pixels
#define PIXELS_TO_METERS(pixels) ((float)(pixels) / 50)


enum ColliderType {

    GROUND,
    BULLET,
    PLAYER,
    BOUNCER,
    TESTGROUND,
    BREAKABLE

};
enum ColliderShape {
    QUAD,
    CIRCLE
};

enum CollisionDetection {
    TELEPORT,
    ITERATIVE,
    RAYCAST
};

enum Integrators {
    EULER,
    SIMPLETIC_EULER,
    VERLET
};

struct vec2 {
    float x;
    float y;

    float LengthSquared() const {
        return x * x + y * y;
    }

    float Length() const {
        return sqrt(LengthSquared());
    }

    vec2 Normalize() const {
        float length = Length();
        return { x / length, y / length };
    }
};

struct circle {
    int x;
    int y;
    int r;
};

struct ResitutionObj {
    int x, y; 
    int dx, dy;
};

class Collider {
public:
    
    SDL_Rect data;
    ColliderType type;
    ColliderShape shape = QUAD;

    bool made_explosion = false;

    Collider(SDL_Rect r, ColliderType type, Module* Listener, ColliderShape myShape = ColliderShape::QUAD) {
        data = r;
        this->type = type;
        listener = Listener;
        shape = myShape;
    }

    bool Intersects(SDL_Rect* r) const
    {
        if (shape == ColliderShape::QUAD) {
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
        else {

        }
    }

    Module* listener;
    
};

class Explosion {

public:
    Timer myTimer;
    circle shape;

    bool done = false;

    Explosion(int x, int y, int rad) {
        myTimer.Start();
        shape.x = x;
        shape.y = y;
        shape.r = rad;
        // add explosion to explosion list
    }

    RigidBody* mybody;
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

    float dragCoefficient;
    vec2 windForce;

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

    float LengthSquared() const {
        return velocity.x * velocity.x + velocity.y * velocity.y;
    }

    float Length() const {
		return sqrt(LengthSquared());
	}

    vec2 Normalize() const {
		float length = Length();
		return { velocity.x / length, velocity.y / length };
	}

    virtual void CollisionBehaviour(Module* listener) {

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
    void AnimationLogic();
    update_status PreUpdate();
    update_status PostUpdate();
    bool CleanUp();
    void IterativeCollisionIntegration(RigidBody* c1, RigidBody* c2);
    void RayCast(RigidBody* c1);
    RigidBody* createBouncer(int x, int y, int width, int height);
    void ApplyAerodynamics(RigidBody* body, float deltaTime);
    void ApplyWindForce(RigidBody* body, float deltaTime);

    void ShowDebug();

    bool IsRectangleIntersectingWithCircle(const SDL_Rect& rect, const SDL_Point& circleCenter, int circleRadius);

    std::list<RigidBody*> bodies;
    std::list<RigidBody*> bombs;
    std::list<Explosion*> explosions;
    RigidBody* corpses[50] = { nullptr };
    Explosion* defused[10] = { nullptr };

    CollisionDetection collisionMethod[3];
    CollisionDetection* currentCollisionMethod;

    Integrators currentIntegrator;

    Timer startCounting;

    int c = 0;
    int in = 0;

private:
    vec2 canon = {10, 150};
    bool debug;

    SDL_Texture* bird;
    SDL_Texture* MarcoPeligro;

    Anim* currentAnim;
    Anim bebeRight;
    Anim bebeLeft;


};