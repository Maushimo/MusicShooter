//
//  Bullet.hpp
//  MusicShooter
//
//  Created by Mohsin Yusuf on 04/03/2017.
//  Copyright © 2017 Mohsin Yusuf. All rights reserved.
//

#ifndef Bullet_hpp
#define Bullet_hpp

#include "LTexture.hpp"
#include <SDL2/SDL.h>
#include <iostream>

class Bullet
{
public:
    Bullet(SDL_Renderer* r, LTexture* texture, float x, float y, float mX, float mY, int sWidth, int sHeight, double playerAngle);
    ~Bullet();
    
    bool loadSheet();
    
    void update();
    void draw();
    
    bool checkOffScreen();
    
    bool isOffScreen;
    
private:
    float posX, posY;
    float width, height;
    float vx;
    float vy;
    float speed = 10;
    float drag = 0.5;
    
    double angle;
    
    //store projected position for bullet to fly towards
    float projectedPosX, projectedPosY;
    
    //GLOBAL VARIABLES
    LTexture* gSpriteSheetTexture;
    SDL_Rect gSpriteClip;

    SDL_Renderer* gRenderer;
    int SCREEN_WIDTH, SCREEN_HEIGHT;
};

#endif /* Bullet_hpp */
