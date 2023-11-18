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
        /*static char title[400];

        sprintf_s(title, 400, "Actual integrator: EULER -- Deltatime: %f, InSpeed: %0.1f, InAngle: %0.1f, CurrSpeed: %0.1f, CurrAcceleration: %0.1f, CurrentPos: %0.1f",
            App->deltaTime.delta, initialSpeed, angle * 180 / 3.1416, bullet->velocity.x, -(float)GRAVITY, bullet->posRect.x);
        App->window->SetTitle(title);*/

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
    LOG("\n\n  %d num bodies \n",bodies.size());
    

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

        for (RigidBody* bullet2 : bodies) {
            IterativeCollisionIntegration(bullet, bullet2);
        }

        SDL_Rect Screen = { 0,0,SCREEN_WIDTH , SCREEN_HEIGHT };
        if (bullet->collider->Intersects(&Screen) == false) {
            for (int i = 0; i < 50; ++i) {
                if (corpses[i] == nullptr) {
                    corpses[i] = bullet;
                }
            }
        }

    }

    //-----------Delete Corpses---------------//
    for (int i = 0; i < 50; ++i) {
        if (corpses[i] == nullptr) { break; }
        else {
            bodies.remove(corpses[i]);
            corpses[i] = nullptr;

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

// This method cosnumes a lot
void ModulePhysics::IterativeCollisionIntegration(RigidBody* c1, RigidBody* c2) {

    const int maxIterations = 3;
    const float epsilon = 0.001f; 

    SDL_Rect prevPos;
    bool willCollide = false;

    for (int i = 0; i < maxIterations; ++i) {
       
        prevPos = c2->collider->data;

        // Call the current integrator method with copy variables that don't actually modify the body
        SDL_Rect fake_posRect = c1->collider->data;
        vec2 fake_velocity = c1->velocity;
        vec2 fake_acceleration = c1->acceleration;

        IntegratorEuler(App->deltaTime.getDeltaTimeInSeconds(), fake_posRect, fake_velocity, fake_acceleration);
       
        
        if (c2->collider->Intersects(&fake_posRect) && c1 != c2) {
            willCollide = true;
            if (c2->collider->type == ColliderType::GROUND && c1->collider->type == ColliderType::BULLET) {
                c1->acceleration.y = 0;
                c1->velocity.y = 0;
            }
            break;
        }

        if (willCollide == true) {
            c1->posRect = prevPos;
        }
    }

        
}






// Called before quitting
bool ModulePhysics::CleanUp()
{
    LOG("Destroying physics world");

    return true;
}