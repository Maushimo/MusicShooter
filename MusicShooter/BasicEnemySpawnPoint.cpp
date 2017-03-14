//
//  BasicEnemySpawnPoint.cpp
//  MusicShooter
//
//  Created by Mohsin Yusuf on 13/03/2017.
//  Copyright © 2017 Mohsin Yusuf. All rights reserved.
//

#include "BasicEnemySpawnPoint.hpp"

BasicEnemySpawnPoint::BasicEnemySpawnPoint(SDL_Renderer* r, LTexture* texture, float x, float y)
{
    posX = x;
    posY = y;
    
    enemyCount = 0;
    
    gRenderer = r;
    enemySpriteSheet = texture;
}

BasicEnemySpawnPoint::~BasicEnemySpawnPoint()
{
    
}

bool BasicEnemySpawnPoint::underEnemyCount()
{
    bool success = true;
    
    if(enemyCount >= enemyLimit)
    {
        //printf("Enemy limit has been exceeded");
        success = false;
    }
    
    return success;
}

void BasicEnemySpawnPoint::spawnEnemy()
{
    if(!underEnemyCount())
    {
        //printf("Can't spawn more enemies, too many enemies on screen.");
        return;
    }else
    {
        enemies[enemyCount] = new BasicEnemy(gRenderer, enemySpriteSheet, posX, posY);
        enemyCount++;
    }
}

void BasicEnemySpawnPoint::spawnerUpdate()
{
    for(int i = 0; i < enemyCount; i++)
    {
        enemies[i]->update();
    }
}

void BasicEnemySpawnPoint::spawnerDraw()
{
    for(int i = 0; i < enemyCount; i++)
    {
        enemies[i]->draw();
    }
}

void BasicEnemySpawnPoint::setPlayerPosition(float playerX, float playerY)
{
    for(int i = 0; i < enemyCount; i++)
    {
        enemies[i]->playerX = playerX;
        enemies[i]->playerY = playerY;
    }
}
