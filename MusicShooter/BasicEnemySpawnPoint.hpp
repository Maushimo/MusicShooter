//
//  BasicEnemySpawnPoint.hpp
//  MusicShooter
//
//  Created by Mohsin Yusuf on 13/03/2017.
//  Copyright © 2017 Mohsin Yusuf. All rights reserved.
//

#ifndef BasicEnemySpawnPoint_hpp
#define BasicEnemySpawnPoint_hpp

#include <SDL2/SDL.h>
#include "Basic Enemy.hpp"
#include "LTexture.hpp"
#include <iostream>
#include <chrono>

class BasicEnemySpawnPoint
{
public:
    BasicEnemySpawnPoint(SDL_Renderer* r, LTexture* texture, float x, float y);
    ~BasicEnemySpawnPoint();
    
    bool underEnemyCount();
    void spawnEnemy();
    
    void spawnerUpdate();
    void spawnerDraw();
    
    void setPlayerPosition(float playerX, float playerY);
    
private:
    //count of enemies currently on screen
    int enemyCount;
    //max amount of enemies that can be spawned at one time
    const static int enemyLimit = 10;
    
    BasicEnemy* enemies[enemyLimit];
    
    //position of spawnpoint
    float posX, posY;
    
    //global stuff to be passed in
    LTexture* enemySpriteSheet;
    SDL_Renderer* gRenderer;
};

#endif /* BasicEnemySpawnPoint_hpp */
