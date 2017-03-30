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
#include <iostream>
#include <vector>

//----------PLAYER CLASS----------//
class Player
{
public:
    Player(SDL_Renderer* r, LTexture* texture, int sWidth, int sHeight);
    ~Player();
    
    float posX, posY;
    
    //amount of bullets on screen
    int bulletCount;
    std::vector<Bullet*> bullets;
    
    bool loadSheet();
    void update();
    void draw(int mouseX, int mouseY);
    void boundaries();
    
    void movementKeys();
    void shootKeys();
    
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    
    void shoot();
    void death(float entityX, float entityY, float entityW, float entityH);
    
    //function to 'kill' bullet when it collides with enemies
    void killBullet(int bulletIndex);
    
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
    
    LTexture* gSpriteSheetTexture;
    SDL_Rect gSpriteClip;
    
    SDL_Renderer* gRenderer;
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
};

#endif /* Player_hpp */
