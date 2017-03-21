//
//  Bullet.cpp
//  MusicShooter
//
//  Created by Mohsin Yusuf on 04/03/2017.
//  Copyright © 2017 Mohsin Yusuf. All rights reserved.
//

#include "Bullet.hpp"

Bullet::Bullet(SDL_Renderer* r,LTexture* texture, float x, float y, float mX, float mY, int sWidth, int sHeight, double playerAngle)
{
    gSpriteSheetTexture = texture;
    gRenderer = r;
    
    posX = x;
    posY = y;
    
    angle = playerAngle;
    
    /* abandoned mouse control
    if(mX > x)
    {
        projectedPosX = float( ((mX-x) * sin(angle)) );
        projectedPosY = float( ((mY-y) * cos(angle)) );
    }else if(y > mY)
    {
        projectedPosX = float( ((x-mX) * sin(angle)) );
        projectedPosY = float( ((y-mY) * cos(angle)) );
    }
     */
    
    /* THE ANGLES PASSED THROUGH FROM THE PLAYER MAKE NO SENSE HERE, SO YES THE ANGLES ARE TECHNICALLY WRONG */
    if(angle == 90)
    {
        projectedPosX = posX;
        projectedPosY = posY * -10;
        
        //std::cout << "Firing UP. ANGLE: " << angle << std::endl;
    }
    else if(angle == 180)
    {
        projectedPosX = posX * 10;
        projectedPosY = posY;
        
        //std::cout << "Firing RIGHT. ANGLE: " << angle << std::endl;
    }
    else if(angle == -90)
    {
        projectedPosX = posX;
        projectedPosY = posY * 10;
        
        //std::cout << "Firing DOWN. ANGLE: " << angle << std::endl;
    }
    else if(angle == 0)
    {
        projectedPosX = posX * -10;
        projectedPosY = posY;
        
        //std::cout << "Firing LEFT. ANGLE: " << angle << std::endl;
    }else if(angle == 45)
    {
        projectedPosX = posX * -10;
        projectedPosY = posY * -10;
        
        //std::cout << "Firing UP LEFT. ANGLE: " << angle << std::endl;
    }
    else if(angle == 135)
    {
        projectedPosX = posX * 10;
        projectedPosY = posY * -10;
        
        //std::cout << "Firing UP RIGHT. ANGLE: " << angle << std::endl;
    }
    else if(angle == -45)
    {
        projectedPosX = posX * -10;
        projectedPosY = posY * 10;
        
        //std::cout << "Firing DOWN LEFT. ANGLE: " << angle << std::endl;
    }
    else if(angle == -135)
    {
        projectedPosX = posX * 10;
        projectedPosY = posY * 10;
        
        //std::cout << "Firing DOWN RIGHT. ANGLE: " << angle << std::endl;
    }
    
    SCREEN_WIDTH = sWidth;
    SCREEN_HEIGHT = sHeight;
    
    isOffScreen = false;
}

Bullet::~Bullet()
{
    gSpriteSheetTexture->free();
}

bool Bullet::loadSheet()
{
    bool success = true;
    
    //clip the sprite needed
    gSpriteClip.x = 173;
    gSpriteClip.y = 12;
    gSpriteClip.w = 29;
    gSpriteClip.h = 27;
    
    return success;
}

void Bullet::update()
{
    isOffScreen = this->checkOffScreen();
    
    posX+=vx;
    posY+=vy;
    
    if(this->posX < projectedPosX)
    {
        if(vx < speed)
        {
            vx++;
        }
    }
    else if(this->posX > projectedPosX)
    {
        if(vx > -speed)
        {
            vx--;
        }
    }
    
    if(this->posY < projectedPosY)
    {
        if(vy < speed)
        {
            vy++;
        }
    }
    else if(this->posY > projectedPosY)
    {
        if(vy > -speed)
        {
            vy--;
        }
    }
}

void Bullet::draw()
{
    if(!this->loadSheet())
    {
        printf("Cannot draw bullet sprite");
        return;
    }else
    {
    gSpriteSheetTexture->render(posX, posY, &gSpriteClip, angle, NULL, SDL_FLIP_NONE);
    }
}

bool Bullet::checkOffScreen()
{
    bool success = false;
    
    //check if the bullet goes off screen
    if(this->posX < 0 || this->posX > SCREEN_WIDTH)
    {
        success = true;
    }
    if(this->posY < 0 || this->posY > SCREEN_HEIGHT)
    {
        success = true;
    }
    
    if(posX == projectedPosX)
    {
        if(posY == projectedPosY)
        {
            success = true;
        }
    }
    return success;
}
