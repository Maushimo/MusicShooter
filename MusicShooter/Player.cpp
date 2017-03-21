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
    gRenderer = r;
    gSpriteSheetTexture = texture;
    SCREEN_WIDTH = sWidth;
    SCREEN_HEIGHT = sHeight;
    
    posX = (SCREEN_WIDTH/2)-size/2;
    posY = (SCREEN_HEIGHT/2)-size/2;
    angle = 0.0;
    
    bulletCount = -1;
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
                //delete it from the array (might actually be deleting renderer too!)
                //delete bullets[i];
                //then set the pointer to NULL
                //bullets[i] = NULL;
                bullets.erase(bullets.begin()+i);
                
                //decrease the number of bullets on screen
                bulletCount--;
                
                std::cout << "Bullet Deleted. Current BulletCount: " << bulletCount << std::endl;
            }else{
                //call it's update function!
                bullets[i]->update();
            }
        }
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
        //set mouse position
        this->mouseX = mouseX;
        this->mouseY = mouseY;
        
        //angle = ((atan2((posY+size/2)-mouseY, (posX+size/2)-mouseX)*180)/M_PI);//-90;
        
        this->gSpriteSheetTexture->render(posX, posY, &gSpriteClip, angle, NULL, SDL_FLIP_NONE);
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
         bulletCount++;
         
         bullets.push_back(new Bullet(gRenderer, gSpriteSheetTexture, (this->posX+size/4), this->posY,this->mouseX, this->mouseY, SCREEN_WIDTH, SCREEN_HEIGHT, this->angle));
         
         std::cout << "Player BulletCount: " << bulletCount << std::endl;
     }else
     {
         std::cout << "Cannot fire" << std::endl;
         return;
     }
     
 }

void Player::death(float entityX, float entityY, float entityW, float entityH)
{
    if(posX > entityX)
    {
        if(posX < entityX+entityW)
        {
            if(posY > entityY)
            {
                if(posY < entityY+entityH)
                {
                    this->~Player();
                }
            }
        }
    }
}
