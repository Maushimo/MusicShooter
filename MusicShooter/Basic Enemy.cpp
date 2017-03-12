//
//  Basic Enemy.cpp
//  MusicShooter
//
//  Created by Mohsin Yusuf on 12/03/2017.
//  Copyright © 2017 Mohsin Yusuf. All rights reserved.
//

#include "Basic Enemy.hpp"

BasicEnemy::BasicEnemy(SDL_Renderer* r, float x, float y, LTexture* spriteSheetTexture)
{
    gRenderer = r;
    gSpriteSheetTexture = spriteSheetTexture;
    
    posX = x;
    posY = y;
    vy = 5;
}

BasicEnemy::~BasicEnemy()
{
    gSpriteSheetTexture->free();
}

bool BasicEnemy::loadSheet()
{
    bool success = true;
    
    gSpriteClip.x = 61;
    gSpriteClip.y = 0;
    gSpriteClip.w = size;
    gSpriteClip.h = size;
    
    return success;
}

void BasicEnemy::update()
{
    posY+=vy;
}

void BasicEnemy::draw()
{
    if(!this->loadSheet())
    {
        printf("Can't load enemy sprite.");
        return;
    }else
    {
        gSpriteSheetTexture->render(posX, posY, &gSpriteClip, 0, NULL, SDL_FLIP_NONE);
    }
}
