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
    posX+=vx;
    posY+=vy;
    
    //handle drag
    if(vx > 0)
    {
        vx-=drag;
    }else if(vx < 0)
    {
        vx+=drag;
    }
    if(vy > 0)
    {
        vy-=drag;
    }else if(vy < 0)
    {
        vy+=drag;
    }
    
    if(vx < drag && vx > -drag)
    {
        vx = 0;
    }
    if(vy < drag && vy > -drag)
    {
        vy = 0;
    }
    
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
        if(vx < speed)
        {
            vx++;
        }
    }
    
    if(this->posX > playerX)
    {
        if(vx > -speed)
        {
            vx--;
        }
    }
    
    if(this->posY < playerY)
    {
        if(vy < speed)
        {
            vy++;
        }
    }
    
    if(this->posY > playerY)
    {
        if(vy > -speed)
        {
            vy--;
        }
    }
}

bool BasicEnemy::bulletCollide(float bulletX, float bulletY, float bulletWidth, float bulletHeight, std::string path)
{
    bool success = false;
    
    if(bulletX > this->posX && bulletX < (this->posX+this->size))
    {
        if(bulletY > this->posY && bulletY < (this->posY+this->size))
        {
            success = true;
        }
    }
    
    return success;
}

int BasicEnemy::getPosX()
{
    return this->posX;
}

int BasicEnemy::getPosY()
{
    return this->posY;
}

int BasicEnemy::getSize()
{
    return this->size;
}
