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
#include <SDL2_image/SDL_image.h>
#include "LTexture.hpp"
#include <iostream>

//----------PLAYER CLASS----------//
class Player{
public:
    Player(SDL_Renderer* r, int sWidth, int sHeight);
    ~Player();
    
    bool loadSheet();
    void update();
    void draw(int mouseX, int mouseY);
    void boundaries();
    
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    
    //void shoot();
    
private:
    float posX, posY;
    float size = 60;
    
    //Velocity
    float vx;
    float vy;
    
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
