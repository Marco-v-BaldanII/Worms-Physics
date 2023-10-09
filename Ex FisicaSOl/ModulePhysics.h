#pragma once
#include "Module.h"
#include "Globals.h"
#include <list>

struct vec2 {
    float x;
    float y;
};

struct Bullet {
    SDL_Rect rect;
    vec2 velocity;
    vec2 acceleration;
    SDL_Texture* bird;
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
    std::list<Bullet> bullets;

private:
    vec2 canon = {10, 150};
    bool debug;
    SDL_Texture* bird;
};