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
    SDL_RenderDrawRect(gRenderer, &drawnBullet);
}
