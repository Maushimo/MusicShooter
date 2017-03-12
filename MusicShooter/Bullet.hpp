//
//  Bullet.hpp
//  MusicShooter
//
//  Created by Mohsin Yusuf on 04/03/2017.
//  Copyright © 2017 Mohsin Yusuf. All rights reserved.
//

#ifndef Bullet_hpp
#define Bullet_hpp

#include <SDL2/SDL.h>
#include <iostream>

class Bullet
{
public:
    Bullet(SDL_Renderer* r, int sWidth, int sHeight);
    ~Bullet();
    
    void update();
    void draw();
    
    void setPosition(float x, float y);
    
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    
private:
    float posX, posY;
    float width, height;
    float vx = 2;
    float vy = 2;
    float speed = 10;
    
    SDL_Rect drawnBullet;
    
    SDL_Renderer* gRenderer;
    int SCREEN_WIDTH, SCREEN_HEIGHT;
};

#endif /* Bullet_hpp */
