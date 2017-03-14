//
//  Basic Enemy.cpp
//  MusicShooter
//
//  Created by Mohsin Yusuf on 12/03/2017.
//  Copyright © 2017 Mohsin Yusuf. All rights reserved.
//

#include "Basic Enemy.hpp"

BasicEnemy::BasicEnemy(SDL_Renderer* r, LTexture* spriteSheetTexture, float x, float y)
{
    gRenderer = r;
    gSpriteSheetTexture = spriteSheetTexture;
    
    posX = x;
    posY = y;
    vx = 3;
    vy = 3;
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
    this->followPlayer();
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

void BasicEnemy::followPlayer()
{
    if(this->posX < playerX)
    {
        posX+=vx;
    }
    
    if(this->posX > playerX)
    {
        posX-=vx;
    }
    
    if(this->posY < playerY)
    {
        posY+=vy;
    }
    
    if(this->posY > playerY)
    {
        posY-=vy;
    }
}

/*
void BasicEnemy::death()
{
    this->~BasicEnemy();
}
*/
