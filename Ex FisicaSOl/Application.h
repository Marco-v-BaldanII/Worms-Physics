#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H



#include "p2List.h"
#include "Globals.h"
#include "Module.h"
#include "Dummy.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleSceneIntro.h"
#include "ModuleFonts.h"
#include "ModuleWinScreen.h"

const int FPS = 60;
const float FRAME_TARGET_TIME = 1.0f / FPS;

struct Clock
{
    uint64_t last_tick_time = 0;
    float delta = 0;
    bool fixedDeltaTime = false;
    bool semiFixedDeltaTime = false;

    Clock()
    {
        last_tick_time = SDL_GetPerformanceCounter();
    }

    void tick()
    {
        uint64_t current_tick_time = SDL_GetPerformanceCounter();
        delta = (float)(current_tick_time - last_tick_time) / SDL_GetPerformanceFrequency();
        last_tick_time = current_tick_time;

        if (fixedDeltaTime)
        {
            // Fixed delta time
            SDL_Delay(FRAME_TARGET_TIME * 1000.0f);
            delta = FRAME_TARGET_TIME;
        }
        else
        {
            // Semi-fixed delta time
            if (delta < FRAME_TARGET_TIME)
            {
                semiFixedDeltaTime = true;
                SDL_Delay((FRAME_TARGET_TIME - delta) * 1000.0f);
                delta = FRAME_TARGET_TIME;
            }
        }
    }

    float getDeltaTimeInSeconds()
    {
        return delta;
    }

    void toggleDeltaTimeMode()
    {
        fixedDeltaTime = !fixedDeltaTime;
    }
};

class Application
{
public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModulePlayer* player;
	ModuleSceneIntro* scene_intro;
	ModulePhysics* physics;
    ModuleFonts* fonts;
    ModuleWinScreen* winScreen;

private:

	p2List<Module*> list_modules;

public:

    bool debug = false;

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	Clock deltaTime;

private:

	void AddModule(Module* mod);
};
// Your Application class definition

#endif // APPLICATION_H