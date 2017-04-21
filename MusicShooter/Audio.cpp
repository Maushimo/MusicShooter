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
    
    //don't start the game in "slow" mode
    slow = false;
    
    //set volumes of each layer
    for(int i = 0; i < normalTracks.size(); i++)
    {
        volume.push_back(0);
    }
    
    for(int i = 0; i < slowTracks.size(); i++)
    {
        slowVolume.push_back(0);
    }
    
    //set drums volume at the start
    volume[kickSnare] = 127;
    volume[acousticDrums] = 127;
}

Audio::~Audio()
{
    //Free music
    for(int i = 0; i < normalTracks.size(); i++)
    {
        Mix_FreeChunk(normalTracks[i]);
        normalTracks[i] = NULL;
    }
    
    //Free slow tracks
    for(int i = 0; i < slowTracks.size(); i++)
    {
        Mix_FreeChunk(slowTracks[i]);
        slowTracks[i] = NULL;
    }
}

bool Audio::loadMusic()
{
    bool success = true;

    /* LOAD ALL TRACKS */
    //Load kickSnare
    normalTracks.push_back(new Mix_Chunk());
    normalTracks[kickSnare] = Mix_LoadWAV("data/audio/kickSnare.ogg");
    //slow kickSnare
    slowTracks.push_back(new Mix_Chunk());
    slowTracks[kickSnare] = Mix_LoadWAV("data/audio/slowCore/kickSnare_SLOW.ogg");
    //Load bass
    normalTracks.push_back(new Mix_Chunk());
    normalTracks[bass] = Mix_LoadWAV("data/audio/bass.ogg");
    //slow bass
    slowTracks.push_back(new Mix_Chunk());
    slowTracks[bass] = Mix_LoadWAV("data/audio/slowCore/bass_SLOW.ogg");
    //Load chords
    normalTracks.push_back(new Mix_Chunk());
    normalTracks[chords] = Mix_LoadWAV("data/audio/chords.ogg");
    //slow chords
    slowTracks.push_back(new Mix_Chunk());
    slowTracks[chords] = Mix_LoadWAV("data/audio/slowCore/chords_SLOW.ogg");
    //Load acousticDrums
    normalTracks.push_back(new Mix_Chunk());
    normalTracks[acousticDrums] = Mix_LoadWAV("data/audio/acousticDrums.ogg");
    //slow acousticDrums
    slowTracks.push_back(new Mix_Chunk());
    slowTracks[acousticDrums] = Mix_LoadWAV("data/audio/slowCore/acousticDrums_SLOW.ogg");
    
    
    for(int i=0; i<normalTracks.size();i++)
    {
        for(int j=0; j<slowTracks.size();j++)
        {
            if(normalTracks[i] == NULL || slowTracks[j] == NULL)
            {
                printf("Failed to load music! SDL_Mixer Error: %s\n", Mix_GetError());
                success = false;
            }
        }
    }
    
    //Mute tracks on startup
    for(int i = 0; i < normalTracks.size(); i++)
    {
        Mix_Volume(i, 0);
    }
    
    //the music has been loaded
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
            
            for(int j = 0; j < slowTracks.size(); j++)
            {
                //the slow tracks will be 4 channels on from their normal counterparts
                Mix_PlayChannel(j+4, slowTracks[j], -1);
            }
            
            isPlayed = true;
        }
        
        //hopefully this should just iterate through the 4 tracks and not break...
        for(int i=0; i<volume.size(); i++)
        {
            Mix_Volume(i, volume[i]);
        }
        
        //set the slow tracks volume
        for(int i=0; i<slowVolume.size(); i++)
        {
            Mix_Volume(i+4, slowVolume[i]);
        }
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
    
    crossFadeSlowNormal();
    //std::cout << "Slow: " << slow << std::endl;
    //std::cout << "Normal Drum Volume: " << volume[kickSnare] << " Slow Drum Volume: " << slowVolume[kickSnare] << std::endl;
}

void Audio::crossFadeSlowNormal()
{
    //flag to check whether the operation is complete or not
    bool crossfadeDone = false;
    //new vector to store new volume
    std::vector<int> newVolume;
    
    //if the player is currently in the "slow" state...
    if(slow)
    {
        //flage for loading volume
        bool volumeIsLoaded = false;
        
        //flags for checking when the volumes have been switched
        bool normalVolumeDecreased = false;
        bool slowVolumeIncreased = false;
        
        //if the volume hasn't been loaded yet...
        if(!volumeIsLoaded)
        {
            //...load it!
            newVolume = volume;
            //we can set this flag to "true"
            volumeIsLoaded = true;
        }
        
        //if the normal volume has yet to be decreased AND the slow volume has yet to be increased...
        if(!normalVolumeDecreased && !slowVolumeIncreased)
        {
            //iterate through current normal volume
            for(int i=0; i<volume.size(); i++)
            {
                //whilst the volumes are greater than 0...
                while(volume[i] != 0)
                {
                    //...decrease the volume!
                    volume[i]--;
                }
                
                //once the volume has reached 0...
                if(volume[i] == 0)
                {
                    //...we can set this flag to "true"
                    normalVolumeDecreased = true;
                }
            }
        
            //iterate through the current slow volume
            for(int i=0; i<slowVolume.size(); i++)
            {
                //whilst the slow volume is less than the new volume we just set...
                while(slowVolume[i] < newVolume[i])
                {
                    //...increase the slow volume!
                    slowVolume[i]++;
                }
                
                //once we've reached the point we want to get to...
                if(slowVolume[i] == newVolume[i]-1)
                {
                    //...we can set this flag to "true"
                    slowVolumeIncreased = true;
                }
            }
        }
        
        //crossfading has finished
        crossfadeDone = true;
    //if the current state is not slow...
    }else if(!slow)
    {
        bool volumeIsLoaded = false;
        
        //different flags here but serves the same purpose
        bool slowVolumeDecreased = false;
        bool normalVolumeIncreased = false;
        
        if(!volumeIsLoaded)
        {
            newVolume = slowVolume;
            volumeIsLoaded = true;
        }
        
        //if the slow volumes has yet to be decreased AND the normal volume has yet to increase...
        if(!slowVolumeDecreased && !normalVolumeIncreased)
        {
            //same thing happens except the opposite... slow volumes goes up, normal volume goes down
            for(int i=0; i<slowVolume.size(); i++)
            {
                while(slowVolume[i] != 0)
                {
                    slowVolume[i]--;
                }
                
                if(slowVolume[i] == 0)
                {
                    slowVolumeDecreased = true;
                }
            }
            
            for(int i=0; i<volume.size(); i++)
            {
                while(volume[i] < newVolume[i])
                {
                    volume[i]++;
                }
                
                if(volume[i] == newVolume[i]-1)
                {
                    normalVolumeIncreased = true;
                }
            }
        }
        //cross fade is finished
        crossfadeDone = true;
    }
}

void Audio::incrementTracks(bool playerIsHit)
{
    //if the player is hit...
    if(playerIsHit)
    {
        std::cout << "playerIsHit " << playerIsHit << std::endl;
        //...and if the state of the game is NOT slow...
        if(!slow)
        {
            //...and if the volume of the BASS and the ACOUSTIC DRUMS is less than 50...
            if(volume[bass] <= 50 && volume[acousticDrums] <= 50)
            {
                //...decrement the tracks!
                volume[bass]-=50;
                volume[acousticDrums]-=50;
            }
            //else statement for slowVolumes
        }else
        {
            if(slowVolume[bass] <= 50)
            {
                slowVolume[bass]-=50;
                slowVolume[acousticDrums]-=50;
            }
        }
        //if the player is NOT hit...
    }else if(!playerIsHit)
    {
        std::cout << "playerIsHit: " << playerIsHit << std::endl;
        if(!slow)
        {
            if(volume[bass] < 50)
            {
                //...we increment the tracks
                volume[bass]++;
                volume[acousticDrums]++;
            }
        }else
        {
            if(slowVolume[bass] < 50)
            {
                slowVolume[bass]++;
                slowVolume[acousticDrums]++;
            }
        }
    }
}
