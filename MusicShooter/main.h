//
//  main.h
//  MusicShooter
//
//  Created by Mohsin Yusuf on 02/03/2017.
//  Copyright © 2017 Mohsin Yusuf. All rights reserved.
//

#ifndef main_h
#define main_h

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include "Player.hpp"
#include "Basic Enemy.hpp"
#include "Audio.hpp"
#include <iostream>
#include <thread>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

//runs at the start of the program
bool init();
//runs when the program is closed
void close();
bool loadMedia();

Player* player;
BasicEnemy* bEnemy[10];
Audio* audio;

int mouseX, mouseY;

#endif /* main_h */
