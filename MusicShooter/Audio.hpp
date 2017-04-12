//
//  Audio.hpp
//  MusicShooter
//
//  Created by Mohsin Yusuf on 08/03/2017.
//  Copyright © 2017 Mohsin Yusuf. All rights reserved.
//

#ifndef Audio_hpp
#define Audio_hpp

#include <SDL2_mixer/SDL_mixer.h>
#include <iostream>
#include <vector>

class Audio
{
    enum Tracks { kickSnare, bass, chords, lead };
    
public:
    Audio();
    ~Audio();
    
    //track layers
    std::vector<Mix_Chunk*> normalTracks;
    //std::vector<Mix_Chunk*> addPerc;
    
    //layer volumes
    std::vector<int> volume;
    
    bool loadMusic();
    void playMusic();
    
    //MIGHT USE THIS FUNCTION FOR BULLET SOUNDS
    //void playPerc();
    
    void update(int enemiesKilled);
    
    bool isPlayed;
    bool isLoaded;
};

#endif /* Audio_hpp */
