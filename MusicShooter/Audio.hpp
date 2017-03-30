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

class Audio
{
public:
    Audio();
    ~Audio();
    
    //track layers
    Mix_Chunk* kickSnare;
    Mix_Chunk* bass;
    Mix_Chunk* chords;
    Mix_Chunk* lead;
    
    //layer volumes
    int kickSnareVol;
    int bassVol;
    int chordsVol;
    int leadVol;
    
    bool loadMusic();
    void playMusic();
    
    void update(int enemiesKilled1, int enemiesKilled2);
    
    //mute layers
    void muteDrums();
    void muteBass();
    
    bool isPlayed;
    bool isLoaded;
};

#endif /* Audio_hpp */
