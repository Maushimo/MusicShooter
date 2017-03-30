//
//  BasicEnemySpawnPoint.cpp
//  MusicShooter
//
//  Created by Mohsin Yusuf on 13/03/2017.
//  Copyright © 2017 Mohsin Yusuf. All rights reserved.
//

#include "BasicEnemySpawnPoint.hpp"

BasicEnemySpawnPoint::BasicEnemySpawnPoint(SDL_Renderer* r, LTexture* texture, Player* p, float x, float y)
{
    posX = x;
    posY = y;
    
    enemyCount = 0;
    
    gRenderer = r;
    enemySpriteSheet = texture;
    
    player = p;
    
    //Set the start time here
    startTime = SDL_GetTicks();
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
        //if our timer reaches/goes over 2 seconds...
        if(deltaTime >= 2)
        {
            //spawn an enemy
            enemies.push_back(new BasicEnemy(gRenderer, enemySpriteSheet, posX, posY));
            enemyCount++;
            
            //then reset timer
            startTime = SDL_GetTicks();
            deltaTime = 0;
        }
    }
}

void BasicEnemySpawnPoint::spawnerUpdate()
{
    //get the change in time and convert it to seconds
    deltaTime = (SDL_GetTicks() - startTime) / 1000.0f;
    
    std::cout << "Spawner Delta Time: " << deltaTime << std::endl;
    
    for(int i = 0; i < enemyCount; i++)
    {
        if(enemies[i] != NULL)
        {
            enemies[i]->update();
        
            for(int j = 0; j < player->bulletCount; j++)
            {
                if(enemies[i]->bulletCollide(player->bullets[j]->posX, player->bullets[j]->posY, player->bullets[j]->width, player->bullets[j]->height, player->bullets[j]->tag))
                {
                    //destroy bullet
                    player->killBullet(j);
                    //destroy enemy
                    this->removeEnemy(i);
                    return;
                }
            }
        }
    }
}

void BasicEnemySpawnPoint::spawnerDraw()
{
    for(int i = 0; i < enemyCount; i++)
    {
        if(enemies[i] != NULL)
        {
            enemies[i]->draw();
        }
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

void BasicEnemySpawnPoint::removeEnemy(int i)
{
    enemies.erase(enemies.begin()+i);
    enemyCount--;
    
    if(totalEnemiesKilled < 128)
    {
        totalEnemiesKilled++;
    }
    
    //std::cout << "Enemy is dead. Current enemy count: " << enemyCount << std::endl;
}
