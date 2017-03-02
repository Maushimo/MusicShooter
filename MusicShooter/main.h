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
#include "LTexture.hpp"
#include "Player.hpp"
#include <iostream>
#include <string>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

bool init();
void close();
bool loadMedia();
void movement();

Player* player;

int mouseX, mouseY;

#endif /* main_h */
