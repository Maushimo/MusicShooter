//
//  Basic Enemy.hpp
//  MusicShooter
//
//  Created by Mohsin Yusuf on 12/03/2017.
//  Copyright © 2017 Mohsin Yusuf. All rights reserved.
//

#ifndef Basic_Enemy_hpp
#define Basic_Enemy_hpp

#include <SDL2/SDL.h>
#include "LTexture.hpp"
#include <iostream>

class BasicEnemy
{
public:
    BasicEnemy(SDL_Renderer* r, LTexture* spriteSheetTexture, float x, float y);
    ~BasicEnemy();
    
    //stores player position
    float playerX, playerY;
    
    bool loadSheet();
    void update();
    void draw();
    //void death();
    void followPlayer();
    
private:
    float posX, posY;
    float size = 51;
    float vx, vy;
    //double angle;
    
    LTexture* gSpriteSheetTexture;
    SDL_Rect gSpriteClip;
    
    SDL_Renderer* gRenderer;
};

#endif /* Basic_Enemy_hpp */
