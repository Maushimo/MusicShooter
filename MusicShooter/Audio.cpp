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
    
    //set volumes of each layer
    kickSnareVol = 0;
    bassVol = 0;
    chordsVol = 0;
    leadVol = 0;
}

Audio::~Audio()
{
    //Free music
    Mix_FreeChunk(kickSnare);
    Mix_FreeChunk(bass);
    Mix_FreeChunk(chords);
    
    for(int i = 0; i < melodyNotes.size(); i++)
    {
        Mix_FreeChunk(melodyNotes[i]);
        melodyNotes[i] = NULL;
    }
    
    kickSnare = NULL;
    bass = NULL;
    chords = NULL;
}

bool Audio::loadMusic()
{
    bool success = true;
    
    kickSnare = Mix_LoadWAV("data/audio/kickSnare.ogg");
    bass = Mix_LoadWAV("data/audio/bass.ogg");
    chords = Mix_LoadWAV("data/audio/chords.ogg");
    
    melodyNotes.push_back(new Mix_Chunk());
    melodyNotes[0] = Mix_LoadWAV("data/audio/lead1.ogg");
    
    melodyNotes.push_back(new Mix_Chunk());
    melodyNotes[1] = Mix_LoadWAV("data/audio/lead2.ogg");
    
    melodyNotes.push_back(new Mix_Chunk());
    melodyNotes[2] = Mix_LoadWAV("data/audio/lead3.ogg");
    
    if(kickSnare == NULL || bass == NULL || chords == NULL)
    {
        for(int i = 0; i < melodyNotes.size(); i++)
        {
            if(melodyNotes[i] == NULL)
            {
                printf("Failed to load music.mp3! SDL_Mixer Error: %s\n", Mix_GetError());
                success = false;
            }
        }
    }
    
    //Mute tracks on startup
    for(int i = 0; i < 4; i++)
    {
        Mix_Volume(i, 0);
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
        leadVol = 50;
        
        //updates track volumes every frame
        Mix_Volume(0, kickSnareVol);
        Mix_Volume(1, bassVol);
        Mix_Volume(2, chordsVol);
        Mix_Volume(3, leadVol);
    }
}

void Audio::update(int enemiesKilled)
{
    chordsVol = enemiesKilled;
}

void Audio::playNotes()
{
    //generate random number between 0 and 99
    int prob = rand() % 100;
    
    if(prob < 25)
    {
        //play lead1
        Mix_PlayChannel(3, melodyNotes[0], 0);
        return;
    }
    else if(prob > 25 && prob < 50)
    {
        //play lead2
        Mix_PlayChannel(3, melodyNotes[1], 0);
        return;
    }
    else if(prob > 50 && prob < 75)
    {
        //play lead3
        Mix_PlayChannel(3, melodyNotes[2], 0);
        return;
    }
    else if(prob >75 && prob < 100)
    {
        //play lead4
        Mix_PlayChannel(3, melodyNotes[3], 0);
        return;
    }
}
