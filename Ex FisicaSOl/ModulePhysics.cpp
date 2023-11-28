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
    //aqui esta los pajaros de todos los colliders change//
    bird = App->textures->Load("Assets/images/triangulo.png");

    collisionMethod[0] = CollisionDetection::ITERATIVE;
    collisionMethod[1] = CollisionDetection::TELEPORT;
    collisionMethod[2] = CollisionDetection::RAYCAST;
    currentCollisionMethod = &collisionMethod[1];

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
    // ---------C HotKey Changes the current Collision method-----------//
    if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN) {
        c++;
        int index = c % 3;
        *currentCollisionMethod = collisionMethod[index ];
    }

    for (RigidBody* bullet : bodies)
    {
        /*static char title[400];

        sprintf_s(title, 400, "Actual integrator: EULER -- Deltatime: %f, InSpeed: %0.1f, InAngle: %0.1f, CurrSpeed: %0.1f, CurrAcceleration: %0.1f, CurrentPos: %0.1f",
            App->deltaTime.delta, initialSpeed, angle * 180 / 3.1416, bullet->velocity.x, -(float)GRAVITY, bullet->posRect.x);
        App->window->SetTitle(title);*/

        //ApplyAerodynamics(bullet, App->deltaTime.getDeltaTimeInSeconds());
        //ApplyWindForce(bullet, App->deltaTime.getDeltaTimeInSeconds());

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
            if (debug) {
                if (bullet->collider != nullptr) {

                    bullet->collider->data.x = bullet->posRect.x;
                    bullet->collider->data.y = bullet->posRect.y;

                    switch (bullet->collider->type) {

                    case ColliderType::PLAYER:
                        App->renderer->DrawQuad(bullet->collider->data, 0, 0, 255, OPACITY);
                        break;
                    case ColliderType::GROUND :
                        App->renderer->DrawQuad(bullet->collider->data, 0, 255, 10, OPACITY);
                        break;
                    case ColliderType::BULLET:
                        App->renderer->DrawQuad(bullet->collider->data, 255, 80, 70, OPACITY);
                        break;
                    case ColliderType::BOUNCER:
                        App->renderer->DrawQuad(bullet->collider->data, 80, 0, 100, OPACITY);
                        break;
                    case ColliderType::TESTGROUND:
                        App->renderer->DrawQuad(bullet->collider->data, 0, 215, 60, OPACITY);

                    }
                }
            }
        }
    
    float gravity = GRAVITY;
    for (RigidBody* bullet : bodies)
    {
        if (*currentCollisionMethod == CollisionDetection::ITERATIVE && bullet->isMoving) {
            for (RigidBody* bullet2 : bodies) {
                IterativeCollisionIntegration(bullet, bullet2);
            }
        }
        if (*currentCollisionMethod == CollisionDetection::RAYCAST && bullet->isMoving) {
            RayCast(bullet);
        }
        // The teleport method is the OnCollision of the shooter (player)

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

    const int maxIterations = 15;
    const float epsilon = 0.001f; 

    SDL_Rect prevPos;
    bool willCollide = false;

    for (int i = 0; i < maxIterations; ++i) {
       
        prevPos = c1->collider->data;

        // Call the current integrator method with copy variables that don't actually modify the body
        SDL_Rect fake_posRect = c1->collider->data;
        vec2 fake_velocity = c1->velocity;
        vec2 fake_acceleration = c1->acceleration;

        IntegratorEuler(App->deltaTime.getDeltaTimeInSeconds(), fake_posRect, fake_velocity, fake_acceleration);
       
     
        if (c2->collider->Intersects(&fake_posRect) && c1 != c2) {
            
            willCollide = true;
            if (c2->collider->type != ColliderType::PLAYER && c1->collider->type == ColliderType::BULLET) {
                App->renderer->DrawQuad(fake_posRect, 80, 80, 80, 255, true);
                c1->isMoving = false;
                //SDL_Delay(3000);
                c1->acceleration.y = 0;
                c1->velocity.y = 0;
                c1->velocity.x = 0;
            }
            break;
        }

        if (willCollide == true) {
            c1->collider->data = prevPos;
        }
    }
}

void ModulePhysics::RayCast(RigidBody* c1) {

    if (c1->collider->type == ColliderType::BULLET) {
        int rayLength = 3;

        int x1 = c1->collider->data.x + c1->collider->data.w / 2;  // Start point of the ray (center of the bullet)
        int y1 = c1->collider->data.y + c1->collider->data.h / 2;
        int x2 = x1 + (c1->velocity.x * App->deltaTime.getDeltaTimeInSeconds() * rayLength);  // End point of the ray
        int y2 = y1 + (c1->velocity.y * App->deltaTime.getDeltaTimeInSeconds() * rayLength);

        App->renderer->DrawLine(x1, y1, x2, y2, 100, 100, 100, 255);

        for (RigidBody* bullet : bodies) {
            if (bullet->collider->type == ColliderType::GROUND && SDL_IntersectRectAndLine( &bullet->collider->data, &x1, &y1, &x2, &y2 )) {
                c1->StopAllMotion();
                c1->collider->data.x = x2;
                c1->collider->data.y = y2;
            }
        }
    }
}

RigidBody* ModulePhysics::createBouncer(int x, int y, int width, int height) 
{
    RigidBody* bouncer = new RigidBody();

    bouncer->posRect.x = x; 
    bouncer->posRect.y = y; 

    bouncer->CreateCollider(SDL_Rect{ x,y,width,height }, ColliderType::BOUNCER, this);

    return bouncer;
}

//void ModulePhysics::ApplyAerodynamics(RigidBody* body, float deltaTime)
//{
//    float speed = body->LengthSquared();
//    float dragMagnitude = body->dragCoefficient * speed * speed;
//
//    if (speed != 0) {
//        vec2 dragForce = body->Normalize() * -dragMagnitude;
//        body->velocity += dragForce * deltaTime;
//    }
//}
//
//void ModulePhysics::ApplyWindForce(RigidBody* body, float deltaTime)
//{
//    body->velocity += body->windForce * deltaTime;
//}

// Called before quitting
bool ModulePhysics::CleanUp()
{
    LOG("Destroying physics world");
    return true;
}