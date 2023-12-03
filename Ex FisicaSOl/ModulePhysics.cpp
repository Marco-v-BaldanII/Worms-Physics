#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"
#include <list>
#include "ModulePlayer.h"
#include "Timer.h"
#include "ModuleFonts.h"
#include "Anim.h"




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
    bird = App->textures->Load("Assets/images/BebeMarco-Sheet.png");
    MarcoPeligro = App->textures->Load("Assets/images/MarcoEnPeligro.png");

    //Pushbacks bebé

    bebeRight.PushBack({ 0,0,32,32 });
    /*bebeRight.PushBack({ 32,0,32,32 });
    bebeRight.PushBack({ 64,0,32,32 });
    bebeRight.PushBack({ 96,0,32,32 });
    bebeRight.PushBack({ 128,0,32,32 });*/
    bebeRight.loop = true;
    bebeRight.speed = 0.05f;

    bebeLeft.PushBack({ 0,32,32,32 });
    /*bebeLeft.PushBack({ 32,32,32,32 });
    bebeLeft.PushBack({ 64,32,32,32 });
    bebeLeft.PushBack({ 96,32,32,32 });
    bebeLeft.PushBack({ 128,32,32,32 });*/
    bebeLeft.loop = true;
    bebeLeft.speed = 0.05f;
 

    collisionMethod[0] = CollisionDetection::ITERATIVE;
    collisionMethod[1] = CollisionDetection::TELEPORT;
    collisionMethod[2] = CollisionDetection::RAYCAST;
    currentCollisionMethod = &collisionMethod[0];

    currentIntegrator = EULER;

    startCounting.Start();

    return true;
}

void ModulePhysics::IntegratorEuler(float deltaTime, SDL_Rect& rect, vec2& velocity, vec2& acceleration)
{
    if (deltaTime != 0) {
        
        LOG("\n position X = %d + %lf * %lf", rect.x, velocity.x, deltaTime);
        rect.x += METERS_TO_PIXELS(velocity.x) * deltaTime;
       
        rect.y += METERS_TO_PIXELS(velocity.y) * deltaTime;
        velocity.x += acceleration.x * deltaTime;
        velocity.y += acceleration.y * deltaTime;
    }

}

void ModulePhysics::IntegratorEuler2(float deltaTime, SDL_Rect& rect, vec2& velocity, vec2& acceleration)
{
    vec2 oldVelocity = velocity;
    velocity.x += acceleration.x * deltaTime;
    velocity.y += acceleration.y * deltaTime;
    rect.x += METERS_TO_PIXELS((oldVelocity.x + velocity.x) / 2) * deltaTime;
    rect.y += METERS_TO_PIXELS((oldVelocity.y + velocity.y) / 2) * deltaTime;
}

void ModulePhysics::IntegratorVerlet(float deltaTime, SDL_Rect& rect, vec2& velocity, vec2& acceleration)
{
    vec2 oldVelocity = velocity;
    velocity.x += acceleration.x * deltaTime;
    velocity.y += acceleration.y * deltaTime;
    rect.x += ((METERS_TO_PIXELS(oldVelocity.x + velocity.x) / 2)) * deltaTime + 1 / 2 * (METERS_TO_PIXELS(acceleration.x) * deltaTime * deltaTime);
    rect.y += ((METERS_TO_PIXELS(oldVelocity.y + velocity.y) / 2)) * deltaTime + 1 / 2 * (METERS_TO_PIXELS(acceleration.y) * deltaTime * deltaTime);
}


void ModulePhysics::AnimationLogic()
{
    for (const RigidBody* bullet : bodies)
    {

        if (bullet->velocity.x >= 0)
        {
            currentAnim = &bebeRight;
        }
        else
        {
            currentAnim = &bebeLeft;
        }

        currentAnim->Update();
        if (bullet->collider->type == ColliderType::BULLET)
        {
            App->renderer->Blit(bird, bullet->posRect.x, bullet->posRect.y, &(currentAnim->GetCurrentFrame()));
        }
        
    }
}

update_status ModulePhysics::PreUpdate()
{

    // ---------C HotKey Changes the current Collision method-----------//
    if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN) {
        c++;
        int index = c % 3;
        switch (index) {
        case 0:
            *currentCollisionMethod = CollisionDetection::ITERATIVE;
            break;
        case 1:
            *currentCollisionMethod = CollisionDetection::RAYCAST;
            break;
        case 2:
            *currentCollisionMethod = CollisionDetection::TELEPORT;
            break;
        }
    }

    // F2 HotKey to change Integrator
    if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
        in++;
        int index = in % 3;
        switch (index) {
        case 0:
            currentIntegrator = EULER;
            break;
        case 1:
            currentIntegrator = SIMPLETIC_EULER;
            break;
        case 2:
            currentIntegrator = VERLET;
            break;
        }
    }

    for (RigidBody* bomb : bombs)
    {
        if (bomb->collider->type == ColliderType::BULLET)
        {
            ApplyAerodynamics(bomb, App->deltaTime.getDeltaTimeInSeconds());
            ApplyWindForce(bomb, App->deltaTime.getDeltaTimeInSeconds());
        }
        
        if (bomb->posRect.y >= 150 && bomb->isMoving)
        {
            bomb->velocity.y = 2;
        }
        else if(bomb->isMoving)
        {
            bomb->velocity.y = 8;
        }

        if (currentIntegrator == EULER)
        {
            IntegratorEuler(App->deltaTime.getDeltaTimeInSeconds(), bomb->posRect, bomb->velocity, bomb->acceleration);
        }

        else if (currentIntegrator == SIMPLETIC_EULER)
        {
            IntegratorEuler2(App->deltaTime.getDeltaTimeInSeconds(), bomb->posRect, bomb->velocity, bomb->acceleration);
        }
        else if (currentIntegrator == VERLET)
        {
            IntegratorVerlet(App->deltaTime.getDeltaTimeInSeconds(), bomb->posRect, bomb->velocity, bomb->acceleration);
        }
        else
        {
            if (App->deltaTime.getDeltaTimeInSeconds() == 0) {
                LOG("This should never happen");
            }
            IntegratorEuler(App->deltaTime.getDeltaTimeInSeconds(), bomb->posRect, bomb->velocity, bomb->acceleration);
        }
    }

    for (RigidBody* bullet : bodies)
    {

        if (bullet->collider->type == ColliderType::BULLET)
        {
            ApplyAerodynamics(bullet, App->deltaTime.getDeltaTimeInSeconds());
            //ApplyWindForce(bullet, App->deltaTime.getDeltaTimeInSeconds());
        }

        for (RigidBody* bullet2 : bodies) {

            if (bullet != bullet2) {

                if (bullet->collider->Intersects(&bullet2->collider->data)) {

                    LOG("\n \nColllision\n");
                    
                    bullet2->collider->listener->OnCollision(bullet, bullet2);
                }
            }
        }
        for (Explosion* _explosion : explosions) {
            SDL_Point p = { _explosion->shape.x, _explosion->shape.y };
            if (IsRectangleIntersectingWithCircle(bullet->collider->data, p, _explosion->shape.r) == true && _explosion->done == false) {
                LOG("someone has touched an explosion");
                bullet->collider->listener->OnExplosion(bullet);
                if (bullet->collider->type == ColliderType::PLAYER) {
                    _explosion->done = true;
                }
            };
        }

        if (currentIntegrator == EULER)
        {
            IntegratorEuler(App->deltaTime.getDeltaTimeInSeconds(), bullet->posRect, bullet->velocity, bullet->acceleration);
        }

        else if (currentIntegrator == SIMPLETIC_EULER)
        {
            IntegratorEuler2(App->deltaTime.getDeltaTimeInSeconds(), bullet->posRect, bullet->velocity, bullet->acceleration);
        }
        else if (currentIntegrator == VERLET)
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
        if (startCounting.ReadMSec() > 500) {
            for (int i = 0; i < NUM_PLAYERS; ++i) {
                if (App->player->myPlayers[i]->posDif ==-1571 ) {
                    App->player->myPlayers[i]->posDif = 0;
                    App->player->myPlayers[i]->movement = 700;
                }
                App->player->myPlayers[i]->posDif = abs(App->player->myPlayers[i]->oldPosX) - abs(App->player->myPlayers[i]->rigid->posRect.x);
                App->player->myPlayers[i]->movement -= abs(App->player->myPlayers[i]->posDif);
            }
            
        }


    }
    // Add finished explosions to the "defused" list of explosions to be deleted
    for (Explosion* _explosion : explosions) {

        if (_explosion->myTimer.ReadMSec() > 500) {
            for (int i = 0; i < 10; ++i) {
                if (defused[i] == nullptr) {
                    defused[i] = _explosion;
                    
                    break;
                }
            }
        }
        
    }

    return UPDATE_CONTINUE;
}
update_status ModulePhysics::PostUpdate()
{
    LOG("\n\n  %d num bodies \n",bodies.size());

    
        for (const RigidBody* bullet : bodies)
        {
            
            
                if (bullet->collider != nullptr) {
                    bullet->collider->data.x = bullet->posRect.x;
                    bullet->collider->data.y = bullet->posRect.y;
                    AnimationLogic();
                    if (App->debug) {
                        switch (bullet->collider->type) {

                        case ColliderType::PLAYER:
                            App->renderer->DrawQuad(bullet->collider->data, 0, 0, 255, OPACITY);
                            break;
                        case ColliderType::GROUND:
                            App->renderer->DrawQuad(bullet->collider->data, 0, 255, 10, OPACITY);
                            break;
                        case ColliderType::BULLET:
                            App->renderer->DrawQuad(bullet->collider->data, 255, 80, 70, OPACITY);
                            break;
                        case ColliderType::BOUNCER:
                            App->renderer->DrawQuad(bullet->collider->data, 80, 0, 100, OPACITY);
                            break;
                        case ColliderType::BREAKABLE:
                            App->renderer->DrawQuad(bullet->collider->data, 255, 140, 0, OPACITY);

                        }
                    }
                }
            
        }
        if (App->debug) {
            for (Explosion* explode : explosions) {
                App->renderer->DrawCircle(explode->shape.x, explode->shape.y, explode->shape.r, 80, 80, 0, OPACITY);
            }

          
        }
    
        for (RigidBody* bomb : bombs)
        {
            App->renderer->Blit(MarcoPeligro, bomb->posRect.x, bomb->posRect.y); 
            if (debug) {
                if (bomb->collider != nullptr) {

                    bomb->collider->data.x = bomb->posRect.x;
                    bomb->collider->data.y = bomb->posRect.y;
                    if (App->debug) {

                        switch (bomb->collider->type) {

                        case ColliderType::PLAYER:
                            App->renderer->DrawQuad(bomb->collider->data, 0, 0, 255, OPACITY);
                            break;
                        case ColliderType::GROUND:
                            App->renderer->DrawQuad(bomb->collider->data, 0, 255, 10, OPACITY);
                            break;
                        case ColliderType::BULLET:
                            App->renderer->DrawQuad(bomb->collider->data, 255, 80, 70, OPACITY);
                            break;
                        case ColliderType::BOUNCER:
                            App->renderer->DrawQuad(bomb->collider->data, 80, 0, 100, OPACITY);
                            break;
                        case ColliderType::BREAKABLE:
                            App->renderer->DrawQuad(bomb->collider->data, 255, 140, 0, OPACITY);
                            break;
                        
                        case ColliderType::AID:
                            App->renderer->DrawQuad(bomb->collider->data, 255, 140, 0, OPACITY);

                        }
                    }
                }
            }
            for (RigidBody* bullet : bodies) {

                

                    if (bullet->collider->Intersects(&bomb->collider->data)) {

                        LOG("\n \nColllision\n");

                        bomb->collider->listener->OnCollision(bomb, bullet);
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
            bombs.remove(corpses[i]);
            //App->textures->Unload(corpses[i]->bird);
            corpses[i] = nullptr;
        }

    }
   
    for (int i = 0; i < 10; ++i) {
        if (defused[i] == nullptr) { break; }
        else {
            for (int j = 0; j < 50; ++j) {
                if (corpses[j] == nullptr) {
                    corpses[j] = defused[i]->mybody;
                    break;
                }
                
            }
            explosions.remove(defused[i]);
            defused[i] = nullptr;
        }
    }

    if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
    {
        App->deltaTime.toggleDeltaTimeMode();
    }

    if (App->debug) {
        ShowDebug();
    }

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
                
                App->player->OnCollision(c1, c2);
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

bool ModulePhysics::IsRectangleIntersectingWithCircle(const SDL_Rect& rect, const SDL_Point& circleCenter, int circleRadius) {
    // Find the closest point on the rectangle to the circle
    int closestX, closestY;

    if (circleCenter.x < rect.x)
        closestX = rect.x;
    else if (circleCenter.x > rect.x + rect.w)
        closestX = rect.x + rect.w;
    else
        closestX = circleCenter.x;

    if (circleCenter.y < rect.y)
        closestY = rect.y;
    else if (circleCenter.y > rect.y + rect.h)
        closestY = rect.y + rect.h;
    else
        closestY = circleCenter.y;

    // Calculate the distance between the closest point and the circle's center
    int distance = std::sqrt(std::pow(circleCenter.x - closestX, 2) + std::pow(circleCenter.y - closestY, 2));

    // If the distance is less than the circle's radius, they intersect
    return distance < circleRadius;
}

RigidBody* ModulePhysics::createBouncer(int x, int y, int width, int height) 
{
    RigidBody* bouncer = new RigidBody();

    bouncer->posRect.x = x; 
    bouncer->posRect.y = y; 

    bouncer->CreateCollider(SDL_Rect{ x,y,width,height }, ColliderType::BOUNCER, this);

    return bouncer;
}

void ModulePhysics::ApplyAerodynamics(RigidBody* body, float deltaTime)
{
    //The formula for aerodynamic drag is: Drag = 0.5 * airDensity * velocity^2 * dragCoefficient * area
    //airDensity = 1.225 (at sea level and at 15 °C), and area = 1 for simplicity
    body->dragCoefficient = 0.1f;
    float airDensity = 1.225f;
    float area = 1.0f;

    float dragForceX = 0.5f * airDensity * body->velocity.x * body->velocity.x * body->dragCoefficient * area;
    float dragForceY = 0.5f * airDensity * body->velocity.y * body->velocity.y * body->dragCoefficient * area;

    vec2 dragDirection = body->velocity.Normalize();
    dragDirection.x *= -1;
    dragDirection.y *= -1;

    if (body->velocity.x != 0) {
        body->acceleration.x += dragForceX * dragDirection.x * deltaTime;
    }

    body->acceleration.y += dragForceY * dragDirection.y * deltaTime;
}

void ModulePhysics::ApplyWindForce(RigidBody* body, float deltaTime)
{
    body->windForce.x = App->player->windForceX;
    body->windForce.y = App->player->windForceY;

    body->posRect.x += body->windForce.x;
    body->velocity.y += body->windForce.y;
}

// Called before quitting
bool ModulePhysics::CleanUp()
{
    LOG("Destroying physics world");
    return true;
}

void ModulePhysics::ShowDebug() {

    switch (*currentCollisionMethod) {


    case CollisionDetection::ITERATIVE:
        App->fonts->BlitText(0, 30, 0, "iterative collision detection");
        break;
    case CollisionDetection::RAYCAST:
        App->fonts->BlitText(0, 30, 0, "raycast collision detection");
        break;
    case CollisionDetection::TELEPORT:
        App->fonts->BlitText(0, 30, 0, "teleporting collision detection");
        break;
    }
    switch (currentIntegrator) {
    case EULER:
        App->fonts->BlitText(0, 60, 0, "euler integrator");
        break;
    case SIMPLETIC_EULER:
        App->fonts->BlitText(0, 60, 0, "simpletic euler integrator");
        break;
    case VERLET:
        App->fonts->BlitText(0, 60, 0, "verlet integrator");
        break;
    }


}