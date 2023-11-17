#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"
#include <list>
#include "ModulePlayer.h"


#define OPACITY 80

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
    if (deltaTime != 0) {
        
        LOG("\n position X = %d + %lf * %lf", rect.x, velocity.x, deltaTime);
        rect.x += velocity.x * deltaTime;
       
        rect.y += velocity.y * deltaTime;
        velocity.x += acceleration.x * deltaTime;
        velocity.y += acceleration.y * deltaTime;
    }

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
    for (RigidBody* bullet : bodies)
    {

        for (RigidBody* bullet2 : bodies) {

            if (bullet != bullet2) {

                if (bullet->collider->Intersects(&bullet2->collider->data)) {

                    LOG("\n \nColllision\n");
                    
                    bullet2->collider->listener->OnCollision(bullet, bullet2);

                }

            }


        }


        
        
        
            if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_REPEAT)
            {
                IntegratorEuler(App->deltaTime.getDeltaTimeInSeconds(), bullet->posRect, bullet->velocity, bullet->acceleration);
            }

            else if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_REPEAT)
            {
                IntegratorEuler2(App->deltaTime.getDeltaTimeInSeconds(), bullet->posRect, bullet->velocity, bullet->acceleration);
            }
            else if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_REPEAT)
            {
                IntegratorVerlet(App->deltaTime.getDeltaTimeInSeconds(), bullet->posRect, bullet->velocity, bullet->acceleration);
            }
            else
            {
                if (App->deltaTime.getDeltaTimeInSeconds() == 0) {
                    LOG("This should never happen");
                }
                IntegratorEuler(App->deltaTime.getDeltaTimeInSeconds(), bullet->posRect, bullet->velocity, bullet->acceleration);
            }
        
    }



    return UPDATE_CONTINUE;
}
update_status ModulePhysics::PostUpdate()
{
    

    if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
        debug = !debug;

 

    for (const RigidBody* bullet : bodies)
    {
        App->renderer->Blit(bird, bullet->posRect.x, bullet->posRect.y);
        
            if (bullet->collider != nullptr) {

                bullet->collider->data.x = bullet->posRect.x;
                bullet->collider->data.y = bullet->posRect.y;
                
                App->renderer->DrawQuad(bullet->collider->data, 255, 80, 70,OPACITY);
            }


    }

    float gravity = GRAVITY;
    for (RigidBody* bullet : bodies)
    {
        if (bullet->isMoving) {
            if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
            {
                IntegratorEuler2(App->deltaTime.getDeltaTimeInSeconds(), bullet->posRect, bullet->velocity, bullet->acceleration);
            }
            else if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
            {
                IntegratorEuler(App->deltaTime.getDeltaTimeInSeconds(), bullet->posRect, bullet->velocity, bullet->acceleration);
            }
            else
            {
                IntegratorEuler2(App->deltaTime.getDeltaTimeInSeconds(), bullet->posRect, bullet->velocity, bullet->acceleration);
            }
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