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
    for(int i = 0; i < normalTracks.size(); i++)
    {
        volume.push_back(0);
    }
}

Audio::~Audio()
{
    //Free music
    for(int i = 0; i < normalTracks.size(); i++)
    {
        Mix_FreeChunk(normalTracks[i]);
        normalTracks[i] = NULL;
    }
    
    /*
    for(int i = 0; i < addPerc.size(); i++)
    {
        Mix_FreeChunk(addPerc[i]);
        addPerc[i] = NULL;
    }
     */
}

bool Audio::loadMusic()
{
    bool success = true;

    //Load kickSnare
    normalTracks.push_back(new Mix_Chunk());
    normalTracks[kickSnare] = Mix_LoadWAV("data/audio/kickSnare.ogg");
    //Load bass
    normalTracks.push_back(new Mix_Chunk());
    normalTracks[bass] = Mix_LoadWAV("data/audio/bass.ogg");
    //Load chords
    normalTracks.push_back(new Mix_Chunk());
    normalTracks[chords] = Mix_LoadWAV("data/audio/chords.ogg");
    
    
    /*
    addPerc.push_back(new Mix_Chunk());
    addPerc[0] = Mix_LoadWAV("data/audio/randomDrums/hat.ogg");
    
    addPerc.push_back(new Mix_Chunk());
    addPerc[1] = Mix_LoadWAV("data/audio/randomDrums/shaker.ogg");
    
    addPerc.push_back(new Mix_Chunk());
    addPerc[2] = Mix_LoadWAV("data/audio/randomDrums/snare.ogg");
    
    addPerc.push_back(new Mix_Chunk());
    addPerc[3] = Mix_LoadWAV("data/audio/randomDrums/conga.ogg");
     */
    
    for(int i=0; i<normalTracks.size();i++)
    {
        if(normalTracks[i] == NULL)
        {
            printf("Failed to load music! SDL_Mixer Error: %s\n", Mix_GetError());
            success = false;
        }
    }
    
    //Mute tracks on startup
    for(int i = 0; i < normalTracks.size(); i++)
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
            for(int i = 0; i < normalTracks.size(); i++)
            {
                Mix_PlayChannel(i, normalTracks[i], -1);
            }
            
            isPlayed = true;
        }
        
        //updates track volumes every frame
        Mix_Volume(kickSnare, volume[kickSnare]);
        Mix_Volume(bass, volume[bass]);
        Mix_Volume(chords, volume[chords]);
        Mix_Volume(lead, volume[lead]);
    }
}

void Audio::update(int enemiesKilled)
{
    if(enemiesKilled < 128)
    {
        volume[chords] = enemiesKilled;
    }else {
        volume[chords] = 128;
    }
}

/*
void Audio::playPerc()
{
    //generate random number between 0 and 99
    int prob = rand() % 100;
    
    if(prob < 25)
    {
        //play lead1
        Mix_PlayChannel(3, addPerc[0], 0);
        std::cout << "PERCPLAYED" << std::endl;
        return;
    }
    else if(prob > 25 && prob < 50)
    {
        //play lead2
        Mix_PlayChannel(3, addPerc[1], 0);
        std::cout << "PERCPLAYED" << std::endl;
        return;
    }
    else if(prob > 50 && prob < 75)
    {
        //play lead3
        Mix_PlayChannel(3, addPerc[2], 0);
        std::cout << "PERCPLAYED" << std::endl;
        return;
    }
    else if(prob >75 && prob < 100)
    {
        //play lead4
        Mix_PlayChannel(3, addPerc[3], 0);
        std::cout << "PERCPLAYED" << std::endl;
        return;
    }
}
 */
