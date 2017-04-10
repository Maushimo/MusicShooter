//
//  GameHandler.hpp
//  MusicShooter
//
//  Created by Mohsin Yusuf on 08/04/2017.
//  Copyright © 2017 Mohsin Yusuf. All rights reserved.
//

#ifndef GameHandler_hpp
#define GameHandler_hpp

#include <iostream>

class GameHandler {
public:
    GameHandler();
    ~GameHandler();
    
    int score;
    
    void setScore(int newScore);
    void reset();
};

#endif /* GameHandler_hpp */
