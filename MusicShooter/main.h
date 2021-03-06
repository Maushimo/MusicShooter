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
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_mixer/SDL_mixer.h>

#include "LTexture.hpp"
#include "Player.hpp"
#include "Basic Enemy.hpp"
#include "BasicEnemySpawnPoint.hpp"
#include "Audio.hpp"
#include "Text.hpp"
#include "GameHandler.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

//Different game states
enum class GameStates { MAIN_MENU, PLAYING, QUIT, GAME_OVER };

enum class AudioStates { ADAPTIVE, STATIC };

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

//global spritesheet for entities
LTexture* gSpriteSheetTexture;
//gameover font texture;
LTexture* gFontTexture;

std::stringstream healthStream;
//health font texture;
LTexture* healthFontTexture;

//Main menu textures
LTexture* mainMenuTitleTexture;
LTexture* mainMenuMovementInstructionsTexture;
LTexture* mainMenuShootInstructionsTexture;

//runs at the start of the program
bool init();
//runs when the program is closed
void close();
//mainly runs checks on the LTextures
bool loadMedia();

//method to generate text file containing various in game stats
void generateAnalytics();

//method to return sum of enemies killed across all spawners
int sumOfEnemiesKilled();

Player* player;

//vector containing spawners
std::vector<BasicEnemySpawnPoint*> bEnemySpawner;

//audio handler
Audio* audio;

//the GLOBAL amount of enemies killed
int gTotalEnemiesKilled;

//handles stats
GameHandler* gHandler;

GameStates GameState;

AudioStates CurrentAudioState;

//set the colour
SDL_Color fontColour = { 255,255,255,255 };
SDL_Color healthColour = { 0,0,0,255 };

//GAME OVER text
Text* gText;
//health text
Text* healthText;

//main menu text
Text* titleText;
Text* movementText;
Text* shootingText;

//store mouse position
int mouseX, mouseY;

//global timer stuff
float gStartTime;
float gDeltaTime;

#endif /* main_h */
