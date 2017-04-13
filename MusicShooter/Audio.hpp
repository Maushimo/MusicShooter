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
    //enums to help alleviate confusion
    enum Tracks { kickSnare, bass, chords, lead };
    
public:
    Audio();
    ~Audio();
    
    //track layers
    std::vector<Mix_Chunk*> normalTracks;
    std::vector<Mix_Chunk*> slowTracks;
    //std::vector<Mix_Chunk*> addPerc;
    
    //layer volumes
    //normal track volume
    std::vector<int> volume;
    std::vector<int> slowVolume;
    
    bool loadMusic();
    void playMusic();
    
    //method to crossfade between slow and 
    void crossFadeSlowNormal();
    
    //MIGHT USE THIS FUNCTION FOR BULLET SOUNDS
    //void playPerc();
    
    void update(int enemiesKilled);
    
    bool isPlayed;
    bool isLoaded;
    
    //checks if the game's music is in "slow" mode
    bool slow;
};

#endif /* Audio_hpp */
