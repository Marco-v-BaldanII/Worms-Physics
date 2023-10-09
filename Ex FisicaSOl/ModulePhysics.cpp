#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"
#include <list>

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
}

update_status ModulePhysics::PreUpdate()
{
    for (Bullet& bullet : bullets)
    {
        if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
        {
            IntegratorEuler(App->deltaTime.getDeltaTimeInSeconds(), bullet.rect, bullet.velocity, bullet.acceleration);
        }
        else if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
        {
            IntegratorEuler2(App->deltaTime.getDeltaTimeInSeconds(), bullet.rect, bullet.velocity, bullet.acceleration);
        }
        else
        {
            IntegratorEuler(App->deltaTime.getDeltaTimeInSeconds(), bullet.rect, bullet.velocity, bullet.acceleration);
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
        bullet.rect.x = 70;
        bullet.rect.y = 190;
        bullet.rect.w = 10;
        bullet.rect.h = 10;
        float initialSpeed = 400.0f;
        float angle = 45;
        angle = angle * M_PI / 180.0f;
        bullet.velocity.x = initialSpeed * cos(angle);
        bullet.velocity.y = -initialSpeed * sin(angle);
        bullet.acceleration = { 0,981 };
        bullets.push_back(bullet);

        static char title[400];
        sprintf_s(title, 400, "Actual integrator: EULER -- Deltatime: %f, InSpeed: %0.1f, InAngle: %0.1f, CurrSpeed: %0.1f, CurrAcceleration: %0.1f, CurrentPos: %0.1f", App->deltaTime.delta, initialSpeed, angle, bullet.velocity, bullet.acceleration, bullet.rect.x);
        App->window->SetTitle(title);
    }
    
    for (const Bullet& bullet : bullets)
    {
        App->renderer->Blit(bird,bullet.rect.x,bullet.rect.y);
        
        /*
        SDL_SetRenderDrawColor(App->renderer->renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(App->renderer->renderer, &bullet.rect);*/
    }

    for (Bullet& bullet : bullets)
    {
        if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
        {
            IntegratorEuler2(App->deltaTime.getDeltaTimeInSeconds(), bullet.rect, bullet.velocity, bullet.acceleration);
        }
        else if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
        {
            IntegratorEuler(App->deltaTime.getDeltaTimeInSeconds(), bullet.rect, bullet.velocity, bullet.acceleration);
        }
        else
        {
            IntegratorEuler2(App->deltaTime.getDeltaTimeInSeconds(), bullet.rect, bullet.velocity, bullet.acceleration);
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