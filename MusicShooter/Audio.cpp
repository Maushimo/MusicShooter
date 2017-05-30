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
    //we haven't played or loaded music yet
    isPlayed = false;
    isLoaded = false;
    
    //load music into the vectors
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
    volume[kickSnare] = 100;
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
    slowTracks[kickSnare] = Mix_LoadWAV("data/audio/slow/kickSnare_SLOW.ogg");
    //Load bass
    normalTracks.push_back(new Mix_Chunk());
    normalTracks[bass] = Mix_LoadWAV("data/audio/bass.ogg");
    //slow bass
    slowTracks.push_back(new Mix_Chunk());
    slowTracks[bass] = Mix_LoadWAV("data/audio/slow/bass_SLOW.ogg");
    //Load chords
    normalTracks.push_back(new Mix_Chunk());
    normalTracks[chords] = Mix_LoadWAV("data/audio/chords.ogg");
    //slow chords
    slowTracks.push_back(new Mix_Chunk());
    slowTracks[chords] = Mix_LoadWAV("data/audio/slow/chords_SLOW.ogg");
    //Load acousticDrums
    normalTracks.push_back(new Mix_Chunk());
    normalTracks[acousticDrums] = Mix_LoadWAV("data/audio/acousticDrums.ogg");
    //slow acousticDrums
    slowTracks.push_back(new Mix_Chunk());
    slowTracks[acousticDrums] = Mix_LoadWAV("data/audio/slow/acousticDrums_SLOW.ogg");
    
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
    
    //load in static music
    introTrack = Mix_LoadWAV("data/audio/Static/TrackIntro.ogg");
    mainTrack = Mix_LoadWAV("data/audio/Static/TrackMain.ogg");
    
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
        
        //hopefully this should just iterate through the tracks and not break...
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
    //set drums volume at the start
    volume[kickSnare] = 100;
    
    //flag to check if player has killed enough enemies for the chords track
    bool overEnemyThresholdChords = false;
    
    if(enemiesKilled >= 50)
    {
        overEnemyThresholdChords = true;
    }
    
    //whilst the enemies are under the MAX amount of enemies killed
    if(enemiesKilled < 75)
    {
        //and if we've crossed over the threshold
        if(overEnemyThresholdChords)
        {
            //equate the chord volume to the enemies killed
            volume[chords] = enemiesKilled-50;
        }
        //otherwise equate the chord volume to max volume
    }else {
        if(overEnemyThresholdChords)
        {
            volume[chords] = 75;
        }
    }
    //this sets the volume for "static" tracks
    //setVolume(mEnemiesKilled);
    
    //increment enemiesKilled
    this->mEnemiesKilled = enemiesKilled;
    
    crossFadeSlowNormal();
    
    //std::cout << "Lead Playing: " << Mix_Playing(lead) << std::endl;
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
    //amount of enemies the player has to kill to unlock certain tracks
    int enemyThreshold = 200;
    //volume threshold for each track
    int bassVolumeThreshold = 50;
    int aDrumVolumeThreshold = 100;
    
    //if the player is hit...
    if(playerIsHit)
    {
        //...and if the state of the game is NOT slow...
        if(!slow)
        {
            //...and if the volume of the BASS and the ACOUSTIC DRUMS is less than 50...
            if(this->mEnemiesKilled > enemyThreshold)
            {
                //...decrement the tracks!
                if(volume[bass] <= bassVolumeThreshold)
                {
                    volume[bass]-=50;
                }
                if(volume[acousticDrums] <= aDrumVolumeThreshold)
                {
                    volume[acousticDrums]-=50;
                }
            }
            //else statement for slowVolumes
        }else
        {
            if(this->mEnemiesKilled > enemyThreshold)
            {
                if(slowVolume[bass] <= bassVolumeThreshold)
                {
                    slowVolume[bass]-=25;
                }
                if(slowVolume[acousticDrums] <= aDrumVolumeThreshold)
                {
                    slowVolume[acousticDrums]-=25;
                }
            }
        }
        //if the player is NOT hit...
    }else if(!playerIsHit)
    {
        if(!slow)
        {
            if(this->mEnemiesKilled > enemyThreshold)
            {
                //...we increment the tracks
                if(volume[bass] < bassVolumeThreshold)
                {
                    volume[bass]++;
                }
                if(volume[acousticDrums] < aDrumVolumeThreshold)
                {
                    volume[acousticDrums]++;
                }
            }
        }else
        {
            if(this->mEnemiesKilled > enemyThreshold)
            {
                if(slowVolume[bass] < bassVolumeThreshold)
                {
                    slowVolume[bass]++;
                }
                if(slowVolume[acousticDrums] < aDrumVolumeThreshold)
                {
                    slowVolume[acousticDrums]++;
                }
            }
        }
    }
}

void Audio::playStaticMusic()
{
    Mix_Volume(0, 100);
    Mix_Volume(1, 100);
    
    if(!isPlayed)
    {
        //mainTrack has 20 sec silence for intro and is looped in the file itself
        Mix_PlayChannel(0, introTrack, 0);
        Mix_PlayChannel(1, mainTrack, 0);
        
        isPlayed = true;
    }
}

void Audio::stop()
{
    //set all volumes to 0
    for(int i = 0; i < volume.size(); i++)
    {
        volume[i] = 0;
    }
    
    for(int i = 0; i < slowVolume.size(); i++)
    {
        slowVolume[i] = 0;
    }
    
    //pause all tracks
    Mix_Pause(-1);
}

/*
void Audio::setVolume(int enemiesKilled)
{
    //set fadingChords
    if(enemiesKilled >= 75)
    {
        volume[fadingChords] = 100;
    }
 }
*/
