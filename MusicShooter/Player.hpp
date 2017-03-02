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
    void draw(int mouseX, int mouseY);
    void boundaries();
    
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    
private:
    float posX, posY;
    float size = 60;
    float speed = 10;
    double angle;
    LTexture* gSpriteSheetTexture;
    SDL_Rect gSpriteClip;
    
    SDL_Renderer* gRenderer;
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
};

#endif /* Player_hpp */
