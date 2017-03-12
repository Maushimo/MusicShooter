//
//  Bullet.cpp
//  MusicShooter
//
//  Created by Mohsin Yusuf on 04/03/2017.
//  Copyright © 2017 Mohsin Yusuf. All rights reserved.
//

#include "Bullet.hpp"

Bullet::Bullet(SDL_Renderer* r, int sWidth, int sHeight)
{
    gRenderer = r;
    
    SCREEN_WIDTH = sWidth;
    SCREEN_HEIGHT = sHeight;
}

Bullet::~Bullet()
{
}

void Bullet::setPosition(float x, float y)
{
    posX = x;
    posY = y;
}

void Bullet::update()
{
    //constantly adding velocity
    this->posX+=this->vx;
    this->posY+=this->vy;
}

void Bullet::draw()
{
    SDL_SetRenderDrawColor(gRenderer, 128, 128, 128, 255);
    SDL_RenderDrawRect(gRenderer, &drawnBullet);
}

void Bullet::moveUp()
{
    if(vy > -speed){
        vy++;
    }
}

void Bullet::moveDown()
{
    if(vy < speed)
    {
        vy--;
    }
}

void Bullet::moveLeft()
{
    if(vx < speed)
    {
        vx--;
    }
}

void Bullet::moveRight()
{
    if(vx > -speed)
    {
        vx++;
    }
}
