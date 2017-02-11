#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>
#include <cmath>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

bool init();
void close();

bool init()
{
	bool success = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL could not init! SDL Error: %s\n", SDL_GetError());
		success = false;
	}else{
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")){
			printf("Warning: Linear texture filtering not enabled!\n");
		}

		gWindow = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
		if(gWindow == NULL){
			printf("Window couldn't be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}else{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if(gRenderer == NULL){
				printf("Renderer couldn't be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}else{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			}
		}
	}
	return success;
}

void close()
{
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	SDL_Quit();
}

int main(int argc, char const *argv[])
{
	if(!init()){
		printf("Failed to init!\n");
	}else{
		bool quit = false;
		SDL_Event e;

		while(!quit){
			while(SDL_PollEvent(&e) != 0){
				if(e.type == SDL_QUIT){
					quit = true;
				}
			}
			//Clear Screen
			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
			SDL_RenderClear(gRenderer);

			SDL_Rect eRect = { 300, 300, 50, 100 };
			SDL_RenderDrawRect(gRenderer, &eRect);

			int foo = filledEllipseRGBA(gRenderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 100, 100, 255, 0, 0, 255);
			printf("Circle = %d\n", foo);

			//Update Screen
			SDL_RenderPresent(gRenderer);
		}
	}
	close();
	return 0;
}