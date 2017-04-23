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
    
    //there aren't enemies on the screen just yet
    enemyCount = 0;
    //set initial enemy limit
    enemyLimit = 1;
    
    gRenderer = r;
    enemySpriteSheet = texture;
    
    player = p;
    
    totalEnemiesKilled = 0;
    
    //Set the start time here
    spawnStartTime = SDL_GetTicks();
    enemyLimitStartTime = SDL_GetTicks();
    //generate a random spawn time under 4 seconds
    timeLimit = rand() % 4;
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
        //if our timer reaches/goes over set time...
        if(spawnDeltaTime >= timeLimit)
        {
            //spawn an enemy
            enemies.push_back(new BasicEnemy(gRenderer, enemySpriteSheet, posX, posY));
            enemyCount++;
            
            //then reset timer
            spawnStartTime = SDL_GetTicks();
            spawnDeltaTime = 0;
        }
    }
}

void BasicEnemySpawnPoint::spawnerUpdate()
{
    //get the change in time and convert it to seconds
    spawnDeltaTime = (SDL_GetTicks() - spawnStartTime) / 1000.0f;
    enemyLimitDeltaTime = (SDL_GetTicks() - enemyLimitStartTime) / 1000;
    
    //every time the ENEMYLIMIT timers hit a multiple of 30 (excluding 0)
    if((enemyLimitDeltaTime % 30) == 0 && enemyLimitDeltaTime != 0)
    {
        //and if the enemy limit is UNDER 20
        if(enemyLimit < 20)
        {
            //we increment the limit to the spawner
            enemyLimit++;
            
            //then reset the timer
            enemyLimitStartTime = SDL_GetTicks();
            enemyLimitDeltaTime = 0;
        }else
        {
            //if we are over 20 then just set the limit to 20
            enemyLimit = 20;
        }
    }
    
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
    
    totalEnemiesKilled++;
    
    //std::cout << "Enemy is dead. Current enemy count: " << enemyCount << std::endl;
}

void BasicEnemySpawnPoint::reset()
{
    for(int i=0;i<enemies.size();i++)
    {
        enemies.erase(enemies.begin()+i);
    }
    
    enemyCount = 0;
    totalEnemiesKilled = 0;
}
