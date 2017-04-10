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

#include "Player.hpp"
#include "Basic Enemy.hpp"
#include "LTexture.hpp"

#include <iostream>
#include <vector>
#include <string>

class BasicEnemySpawnPoint
{
public:
    BasicEnemySpawnPoint(SDL_Renderer* r, LTexture* texture, Player* p, float x, float y);
    ~BasicEnemySpawnPoint();
    
    //sum of enemies killed on current instance of spawner
    int totalEnemiesKilled;
    
    //get it? "PUBLIC ENEMIES"?
    std::vector<BasicEnemy*> enemies;
    
    bool underEnemyCount();
    void spawnEnemy();
    
    void spawnerUpdate();
    void spawnerDraw();
    
    void setPlayerPosition(float playerX, float playerY);
    void removeEnemy(int i);
    
    //function to reset the spawner to it's original state
    void reset();
    
private:
    //max amount of enemies that can be spawned at one time
    const int enemyLimit = 10;
    //count of enemies currently on screen
    int enemyCount;
    
    //position of spawnpoint
    float posX, posY;
    
    //spawner timer stuff
    float startTime;
    float deltaTime;
    
    int timeLimit;
    
    //global stuff to be passed in
    LTexture* enemySpriteSheet;
    SDL_Renderer* gRenderer;
    Player* player;
};

#endif /* BasicEnemySpawnPoint_hpp */
