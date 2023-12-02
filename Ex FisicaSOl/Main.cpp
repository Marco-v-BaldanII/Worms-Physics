#include <stdlib.h>
#include "Application.h"
#include "Globals.h"
#include <time.h>
#include <thread>
#include <chrono>
using namespace std::chrono;

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

enum main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

int main(int argc, char ** argv)
{
	LOG("Starting game '%s'...", TITLE);

	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;
	Application* App = NULL;

	while (state != MAIN_EXIT)
	{
		high_resolution_clock::time_point start = high_resolution_clock::now();

		switch (state)
		{
		case MAIN_CREATION:

			LOG("-------------- Application Creation --------------");
			App = new Application();
			state = MAIN_START;
			break;

		case MAIN_START:

			LOG("-------------- Application Init --------------");
			if (App->Init() == false)
			{
				LOG("Application Init exits with ERROR");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				LOG("-------------- Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				LOG("Application Update exits with ERROR");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:

			LOG("-------------- Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				LOG("Application CleanUp exits with ERROR");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}

		if (App->debug /*&& semifixed delta time*/ )
		{
			high_resolution_clock::time_point endCycle = high_resolution_clock::now();
			App->physics->elapsedCycle = duration_cast<microseconds>(endCycle - start);

			//Time per frame in microseconds acording to taget FPS
			int microSecCheck = (int)((1.0f / (float)App->physics->targetFPS) * 1E5);

			//This is to cap FPS, the diplaying of FPS on screen is calculated underneath
			if (App->physics->elapsedCycle < std::chrono::microseconds(microSecCheck))
			{
				std::this_thread::sleep_for(std::chrono::microseconds(std::chrono::microseconds(microSecCheck) - App->physics->elapsedCycle));
			}

			//Time per cycle + delay
			high_resolution_clock::time_point endFrame = high_resolution_clock::now();
			App->physics->elapsedFrame = duration_cast<microseconds>(endFrame - start);

			//Calculate FPSs
			App->physics->FPS = 1 / ((double)App->physics->elapsedFrame.count() * 10E-6);
		}
	
	}

	delete App;
	LOG("Exiting game '%s'...\n", TITLE);
	return main_return;
}