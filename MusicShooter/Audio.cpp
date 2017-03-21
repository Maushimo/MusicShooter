//
//  Audio.cpp
//  MusicShooter
//
//  Created by Mohsin Yusuf on 08/03/2017.
//  Copyright © 2017 Mohsin Yusuf. All rights reserved.
//

#include "Audio.hpp"

Audio::Audio()
{
    isPlayed = false;
    isLoaded = false;
    this->loadMusic();
}

Audio::~Audio()
{
    //Free music
    Mix_FreeChunk(kickSnare);
    Mix_FreeChunk(bass);
    Mix_FreeChunk(chords);
    Mix_FreeChunk(lead);
    
    kickSnare = NULL;
    bass = NULL;
    chords = NULL;
    lead = NULL;
}

bool Audio::loadMusic()
{
    bool success = true;
    
    kickSnare = Mix_LoadWAV("data/audio/kickSnare.ogg");
    bass = Mix_LoadWAV("data/audio/bass.ogg");
    chords = Mix_LoadWAV("data/audio/chords.ogg");
    lead = Mix_LoadWAV("data/audio/lead.ogg");
    
    if(kickSnare == NULL || bass == NULL || chords == NULL || lead == NULL)
    {
        printf("Failed to load music.mp3! SDL_Mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    
    isLoaded = success;
    
    return success;
}

void Audio::playMusic()
{
    if(!isLoaded)
    {
        printf("Can't play music.");
        return;
    }else
    {
        if(!isPlayed)
        {
            Mix_PlayChannel(0, kickSnare, -1);
            Mix_PlayChannel(1, bass, -1);
            Mix_PlayChannel(2, chords, -1);
            
            isPlayed = true;
        }
    }
}

void Audio::muteDrums()
{
    Mix_Volume(0, 0);
}

void Audio::muteBass()
{
    Mix_Volume(1, 0);
}

void Audio::playDrums()
{
    Mix_Volume(0, 128);
}

void Audio::playBass()
{
    Mix_Volume(1, 128);
}
