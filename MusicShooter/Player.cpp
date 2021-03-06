//
//  Player.cpp
//  MusicShooter
//
//  Created by Mohsin Yusuf on 02/03/2017.
//  Copyright © 2017 Mohsin Yusuf. All rights reserved.
//

#include "Player.hpp"

Player::Player(SDL_Renderer* r, LTexture* texture, int sWidth, int sHeight)
{
    //set spritesheet and global renderers
    gRenderer = r;
    gSpriteSheetTexture = texture;
    SCREEN_WIDTH = sWidth;
    SCREEN_HEIGHT = sHeight;
    
    //default position, angle and health
    posX = (SCREEN_WIDTH/2)-size/2;
    posY = (SCREEN_HEIGHT/2)-size/2;
    angle = 0.0;
    health = 100;
    
    bulletCount = -1;
    
    //set start time for keysPerMin timer
    timerStartTime = SDL_GetTicks();
    
    //set keystroke tracking
    numOfKeyPresses = 0;
    keysPerMin = 0;
    totalMins = 0;
    
    totalBulletsFired = 0;
    
    mIsHit = false;
}

Player::~Player()
{
    gSpriteSheetTexture->free();
}

bool Player::loadSheet()
{
    //success flag
    bool success = true;
    
    //Crop player sprites
    gSpriteClip.x = 0;
    gSpriteClip.y = 0;
    gSpriteClip.w = (size-7);
    gSpriteClip.h = (size-8);
    
    return success;
}

void Player::update()
{
    //get the change in time and convert it to seconds
    timerDeltaTime = (SDL_GetTicks() - timerStartTime) / 1000;
    
    //if the timer hits a time that is a multiple of 60...
    if(timerDeltaTime % 60 == 0 && timerDeltaTime != 0)
    {
        //...increase total minutes elapsed!
        totalMins++;
        timerStartTime = SDL_GetTicks();
    }
    
    //update "keysPerMin"
    this->checkKeysPerMin();
    
    //std::cout << "Mins: " << totalMins << " Sec: " << timerDeltaTime << std::endl;
    //std::cout << "Keys Per Min: " << keysPerMin << std::endl;
    
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
    
    //iterate through bullets array
    for(int i = 0; i < bulletCount; i++)
    {
        //if it exists
        if(bullets[i] != NULL)
        {
            //if any bullet is offScreen
            if(bullets[i]->isOffScreen)
            {
                //delete it from the array
                bullets.erase(bullets.begin()+i);
                
                //decrease the number of bullets on screen
                bulletCount--;
                
                //std::cout << "Bullet Deleted. Current BulletCount: " << bulletCount << std::endl;
                return;
            }else{
                //call it's update function!
                bullets[i]->update();
            }
        }
    }
    
    //std::cout << "Player Health: " << this->health << std::endl;
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
        //set mouse position
        this->mouseX = mouseX;
        this->mouseY = mouseY;
        
        //render the cropped sprite
        this->gSpriteSheetTexture->render(posX, posY, &gSpriteClip, angle, NULL, SDL_FLIP_NONE);
        //keep the sprite within game area
        this->boundaries();
        
        //iterate through every bullet in the array
        for(int i = 0; i < bulletCount; i++)
        {
            //if it exists
            if(bullets[i+1] != NULL)
            {
                //call it's draw function!
                bullets[i+1]->draw();
            }
        }
        
        //std::cout << "Player Angle: " << this->angle << std::endl;
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

void Player::checkKeysPerMin()
{
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    
    //Everytime one of these keys is pressed, the numOfKeyPresses increments
    if( currentKeyStates[ SDL_SCANCODE_W ] )
    {
        this->numOfKeyPresses++;
    }
    if( currentKeyStates[ SDL_SCANCODE_S ] )
    {
        this->numOfKeyPresses++;
    }
    if( currentKeyStates[ SDL_SCANCODE_A ] )
    {
        this->numOfKeyPresses++;
    }
    if( currentKeyStates[ SDL_SCANCODE_D ] )
    {
        this->numOfKeyPresses++;
    }
    
    //this->keysPerMin = this->numOfKeyPresses;
}

void Player::shootKeys()
{
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    
    /* U D L R */
    if( currentKeyStates[ SDL_SCANCODE_UP ] )
    {
        this->angle = 90;
        this->shoot();
    }
    if( currentKeyStates[ SDL_SCANCODE_DOWN ] )
    {
        this->angle = -90;
        this->shoot();
    }
    if( currentKeyStates[ SDL_SCANCODE_LEFT ] )
    {
        this->angle = 0;
        this->shoot();
    }
    if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
    {
        this->angle = 180;
        this->shoot();
    }
    
    /* DIAGONAL */
    if( currentKeyStates[ SDL_SCANCODE_UP ] && currentKeyStates[ SDL_SCANCODE_LEFT ] )
    {
        this->angle = 45;
        this->shoot();
    }
    if( currentKeyStates[ SDL_SCANCODE_UP ] && currentKeyStates[ SDL_SCANCODE_RIGHT ] )
    {
        this->angle = 135;
        this->shoot();
    }
    if( currentKeyStates[ SDL_SCANCODE_DOWN ] && currentKeyStates[ SDL_SCANCODE_LEFT ] )
    {
        this->angle = -45;
        this->shoot();
    }
    if( currentKeyStates[ SDL_SCANCODE_DOWN ] && currentKeyStates[ SDL_SCANCODE_RIGHT ] )
    {
        this->angle = -135;
        this->shoot();
    }
}

//Every time these functions are invoked, velocity is added
void Player::moveLeft()
{
    if(vx > -speed)
    {
        vx--;
    }
}

void Player::moveRight()
{
    if(vx < speed)
    {
        vx++;
    }
}

void Player::moveUp()
{
    if(vy > -speed)
    {
        vy--;
    }
}

void Player::moveDown()
{
    if(vy < speed)
    {
        vy++;
    }
}

 void Player::shoot()
 {
     if(bulletCount < 10)
     {
         //increment on screen bullet count
         bulletCount++;
         
         //create a new bullet in the bullets vector
         bullets.push_back(new Bullet(gRenderer, gSpriteSheetTexture, (this->posX+size/4), this->posY, SCREEN_WIDTH, SCREEN_HEIGHT, this->angle));
         
         totalBulletsFired++;
         
         //std::cout << "Player BulletCount: " << bulletCount << std::endl;
     }else
     {
         //std::cout << "Cannot fire" << std::endl;
         return;
     }
     
 }

void Player::isHit(float entityX, float entityY, float entityW, float entityH)
{
    if(posX > entityX && posX < entityX+entityW && posY > entityY && posY < entityY+entityH)
    {
        health-=0.1;
        mIsHit = true;
    }else {
        mIsHit = false;
    }
}

void Player::killBullet(int bulletIndex)
{
    //remove from vector
    bullets.erase(bullets.begin()+bulletIndex);
    //decrease count of onscreen bullets
    bulletCount--;
}

void Player::reset()
{
    //basically copy the constructors code
    posX = (SCREEN_WIDTH/2)-size/2;
    posY = (SCREEN_HEIGHT/2)-size/2;
    
    angle = 0.0;
    health = 100;
    
    bulletCount = -1;
}

int Player::getNumOfKeyPresses()
{
    return numOfKeyPresses;
}

float Player::getHealthLost()
{
    return 100-this->health;
}

int Player::getTotalBulletsFired()
{
    return totalBulletsFired;
}
