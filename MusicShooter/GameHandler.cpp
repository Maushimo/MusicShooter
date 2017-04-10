//
//  GameHandler.cpp
//  MusicShooter
//
//  Created by Mohsin Yusuf on 08/04/2017.
//  Copyright © 2017 Mohsin Yusuf. All rights reserved.
//

#include "GameHandler.hpp"

GameHandler::GameHandler()
{
    
}

GameHandler::~GameHandler()
{
    
}

void GameHandler::setScore(int newScore)
{
    score = newScore;
    //std::cout << "SCORE: " << score << std::endl;
}

void GameHandler::reset()
{
    score = 0;
}
