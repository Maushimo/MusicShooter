//
//  Player.hpp
//  MusicShooter
//
//  Created by Mohsin Yusuf on 02/03/2017.
//  Copyright © 2017 Mohsin Yusuf. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <SDL2/SDL.h>

#include "LTexture.hpp"
#include "Bullet.hpp"
#include "Audio.hpp"

#include <iostream>
#include <vector>

//----------PLAYER CLASS----------//
class Player
{
public:
    Player(SDL_Renderer* r, LTexture* texture, int sWidth, int sHeight);
    ~Player();
    
    //player position
    float posX, posY;
    
    //player health
    float health;
    
    //amount of bullets on screen
    int bulletCount;
    std::vector<Bullet*> bullets;
    
    //a member variable communicating to other classes when the player is hit
    bool mIsHit;
    
    bool loadSheet();
    void update();
    void draw(int mouseX, int mouseY);
    void boundaries();
    
    //methods to handle key presses
    void movementKeys();
    void shootKeys(Audio* a);
    
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    
    void shoot();
    void isHit(float entityX, float entityY, float entityW, float entityH);
    
    void checkKeysPerMin();
    
    //function to 'kill' bullet when it collides with enemies
    void killBullet(int bulletIndex);
    
    //function to revert player to it's original state
    void reset();
    
    /* GETTERS */
    int getNumOfKeyPresses();
    float getHealthLost();
    
private:
    //float posX, posY;
    float size = 60;
    
    //Velocity
    float vx;
    float vy;
    
    //mouse position
    float mouseX;
    float mouseY;
    
    float speed = 10;
    float drag = 0.5;
    double angle;
    
    //total number of keystrokes
    int numOfKeyPresses;
    int keysPerMin;
    //Timer stuff for measuring keystrokes per min
    int timerStartTime;
    int timerDeltaTime;
    //total time elapsed since start of program in minutes
    int totalMins;
    
    LTexture* gSpriteSheetTexture;
    SDL_Rect gSpriteClip;
    
    SDL_Renderer* gRenderer;
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
};

#endif /* Player_hpp */
