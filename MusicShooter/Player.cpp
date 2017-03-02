//
//  Player.cpp
//  MusicShooter
//
//  Created by Mohsin Yusuf on 02/03/2017.
//  Copyright © 2017 Mohsin Yusuf. All rights reserved.
//

#include "Player.hpp"

Player::Player(SDL_Renderer* r, int sWidth, int sHeight)
{
    gRenderer = r;
    gSpriteSheetTexture = new LTexture(gRenderer);
    SCREEN_WIDTH = sWidth;
    SCREEN_HEIGHT = sHeight;
    
    posX = (SCREEN_WIDTH/2)-size/2;
    posY = (SCREEN_HEIGHT/2)-size/2;
    angle = 0.0;
}

Player::~Player()
{
    delete gSpriteSheetTexture;
}

bool Player::loadSheet()
{
    //success flag
    bool success = true;
    
    if(!gSpriteSheetTexture->loadFromFile("data/spritesheet.png"))
    {
        printf("Failed to load spritesheet texture SDL_Image Error: %s\n", IMG_GetError());
        success = false;
    }else
    {
        //Crop player sprites
        gSpriteClip.x = 0;
        gSpriteClip.y = 0;
        gSpriteClip.w = size;
        gSpriteClip.h = size;
    }
    
    return success;
}

void Player::draw(int mouseX, int mouseY)
{
    if(!this->loadSheet())
    {
        printf("Can't draw player sprite\n");
        return;
    }
    else
    {
        //really hacky fix of minusing 90 degrees here, MUST FIX PROPERLY
        angle = ((atan2((posY+size/2)-mouseY, (posX+size/2)-mouseX)*180)/M_PI)-90;
        
        this->gSpriteSheetTexture->render(posX, posY, &gSpriteClip, angle, NULL, SDL_FLIP_NONE);
        this->boundaries();
        //std::cout << "Angle: " << angle << std::endl;
    }
}

void Player::boundaries()
{
    if(posX < 0)
    {
        posX = 0;
    }
    if((posX+size) > SCREEN_WIDTH)
    {
        posX = SCREEN_WIDTH-size;
    }
    if(posY < 0)
    {
        posY = 0;
    }
    if((posY+size) > SCREEN_HEIGHT)
    {
        posY = SCREEN_HEIGHT-size;
    }
}

void Player::moveLeft()
{
    posX-=speed;
}

void Player::moveRight()
{
    posX+=speed;
}

void Player::moveUp()
{
    posY-=speed;
}

void Player::moveDown()
{
    posY+=speed;
}
