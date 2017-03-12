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
    
    Mix_Chunk* kickSnare;
    Mix_Chunk* bass;
    Mix_Chunk* chords;
    Mix_Chunk* lead;
    
    bool loadMusic();
    void playMusic();
    
    void muteDrums();
    void muteBass();
    
    bool isPlayed;
};

#endif /* Audio_hpp */
