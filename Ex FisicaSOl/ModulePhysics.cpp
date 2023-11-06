#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"
#include <list>
#include "ModulePlayer.h"

#define GRAVITY 0.981

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
    debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
    LOG("Creating Physics 2D environment");
    bird = App->textures->Load("Assets/images/triangulo.png");
    return true;
}

void ModulePhysics::IntegratorEuler(float deltaTime, SDL_Rect& rect, vec2& velocity, vec2& acceleration)
{
    rect.x += velocity.x * deltaTime;
    rect.y += velocity.y * deltaTime;
    velocity.x += acceleration.x * deltaTime;
    velocity.y += acceleration.y * deltaTime;

}

void ModulePhysics::IntegratorEuler2(float deltaTime, SDL_Rect& rect, vec2& velocity, vec2& acceleration)
{
    /*velocity.x += acceleration.x * deltaTime;
    velocity.y += acceleration.y * deltaTime;*/
    vec2 oldVelocity = velocity;
    velocity.x += acceleration.x * deltaTime;
    velocity.y += acceleration.y * deltaTime;
    rect.x += (oldVelocity.x + velocity.x) / 2 * deltaTime;
    rect.y += (oldVelocity.y + velocity.y) / 2 * deltaTime;
    rect.x += velocity.x * deltaTime;
    rect.y += velocity.y * deltaTime;
}

void ModulePhysics::IntegratorVerlet(float deltaTime, SDL_Rect& rect, vec2& velocity, vec2& acceleration)
{
    vec2 oldVelocity = velocity;
    velocity.x += acceleration.x * deltaTime;
    velocity.y += acceleration.y * deltaTime;
    rect.x += ((oldVelocity.x + velocity.x) / 2) * deltaTime + 1 / 2 * (acceleration.x * deltaTime * deltaTime);
    rect.y += ((oldVelocity.y + velocity.y) / 2) * deltaTime + 1 / 2 * (acceleration.y * deltaTime * deltaTime);

}

update_status ModulePhysics::PreUpdate()
{
    for (RigidBody& bullet : bodies)
    {
        
            if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_REPEAT)
            {
                IntegratorEuler(App->deltaTime.getDeltaTimeInSeconds(), bullet.posRect, bullet.velocity, bullet.acceleration);
            }
       
            else if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_REPEAT)
            {
                IntegratorEuler2(App->deltaTime.getDeltaTimeInSeconds(), bullet.posRect, bullet.velocity, bullet.acceleration);
            }
            else if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_REPEAT)
            {
                IntegratorVerlet(App->deltaTime.getDeltaTimeInSeconds(), bullet.posRect, bullet.velocity, bullet.acceleration);
            }
            else
            {
                IntegratorEuler(App->deltaTime.getDeltaTimeInSeconds(), bullet.posRect, bullet.velocity, bullet.acceleration);
            }
    }



    return UPDATE_CONTINUE;
}
update_status ModulePhysics::PostUpdate()
{
    if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
        debug = !debug;

    if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
    {
        Bullet bullet;
        bullet.posRect.x = App->player->rigid.posRect.x;
        bullet.posRect.y = App->player->rigid.posRect.y;
        bullet.posRect.w = 10;
        bullet.posRect.h = 10;
        float initialSpeed = 400.0f;
        float angle = 45;
        angle = angle * M_PI / 180.0f;
        bullet.velocity.x = initialSpeed * cos(angle);
        bullet.velocity.y = -initialSpeed * sin(angle);
        bullet.acceleration = { 0,981 };
        bodies.push_back(bullet);

        static char title[400];
        sprintf_s(title, 400, "Actual integrator: EULER -- Deltatime: %f, InSpeed: %0.1f, InAngle: %0.1f, CurrSpeed: %0.1f, CurrAcceleration: %0.1f, CurrentPos: %0.1f",
            App->deltaTime.delta, initialSpeed, angle * 180 / 3.1416, bullet.velocity.x, - (float)GRAVITY, bullet.posRect.x);
        App->window->SetTitle(title);
    }

    for (const RigidBody& bullet : bodies)
    {
        App->renderer->Blit(bird, bullet.posRect.x, bullet.posRect.y);

        /*
        SDL_SetRenderDrawColor(App->renderer->renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(App->renderer->renderer, &bullet.rect);*/
    }

    for (RigidBody& bullet : bodies)
    {
        if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
        {
            IntegratorEuler2(App->deltaTime.getDeltaTimeInSeconds(), bullet.posRect, bullet.velocity, bullet.acceleration);
        }
        else if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
        {
            IntegratorEuler(App->deltaTime.getDeltaTimeInSeconds(), bullet.posRect, bullet.velocity, bullet.acceleration);
        }
        else
        {
            IntegratorEuler2(App->deltaTime.getDeltaTimeInSeconds(), bullet.posRect, bullet.velocity, bullet.acceleration);
        }
    }

    if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
    {
        App->deltaTime.toggleDeltaTimeMode();
    }

    if (!debug)
        return UPDATE_CONTINUE;

    return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
    LOG("Destroying physics world");

    return true;
}