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
    gSpriteSheetTexture->free();
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
        gSpriteClip.w = (size-7);
        gSpriteClip.h = (size-8);
    }
    
    return success;
}

void Player::update()
{
    //constantly add velocity
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
    
    //eliminate judder
    if(vx < drag && vx > -drag)
    {
        vx = 0;
    }
    if(vy < drag && vy > -drag)
    {
        vy = 0;
    }
    
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
        angle = ((atan2((posY+size/2)-mouseY, (posX+size/2)-mouseX)*180)/M_PI)-90;
        
        this->gSpriteSheetTexture->render(posX, posY, &gSpriteClip, angle, NULL, SDL_FLIP_NONE);
        this->boundaries();
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

//functions controlling movement keys
void Player::movementKeys()
{
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    
    if( currentKeyStates[ SDL_SCANCODE_W ] )
    {
        this->moveUp();
    }
    if( currentKeyStates[ SDL_SCANCODE_S ] )
    {
        this->moveDown();
    }
    if( currentKeyStates[ SDL_SCANCODE_A ] )
    {
        this->moveLeft();
    }
    if( currentKeyStates[ SDL_SCANCODE_D ] )
    {
        this->moveRight();
    }
}

//Every time these functions are invoked, velocity is added
void Player::moveLeft()
{
    //posX-=speed;
    if(vx > -speed)
    {
        vx--;
    }
}

void Player::moveRight()
{
    //posX+=speed;
    if(vx < speed)
    {
        vx++;
    }
}

void Player::moveUp()
{
    //posY-=speed;
    if(vy > -speed)
    {
        vy--;
    }
}

void Player::moveDown()
{
    //posY+=speed;
    if(vy < speed)
    {
        vy++;
    }
}

 void Player::shoot()
 {
     
 }
