#include "main.h"

bool init()
{
    bool success = true;
    
    /* A BUNCH OF INITIALISATION AND ERROR CHECKING */
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not init! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!\n");
        }
        
        gWindow = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
        if(gWindow == NULL)
        {
            printf("Window couldn't be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if(gRenderer == NULL)
            {
                printf("Renderer couldn't be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                
                //init png loading
                int imgFlags = IMG_INIT_PNG;
                if(!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not init! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
                
                //init true type fonts
                if(TTF_Init() == -1)
                {
                    printf("SDL_ttf could not init! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }
                
                //init SDL_Mixer
                if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
                {
                    printf("SDL_Mixer could not init! SDL_Mixer Error: %s\n", Mix_GetError());
                    success = false;
                }
            }
        }
    }
    //load spritesheet
    gSpriteSheetTexture = new LTexture(gRenderer);
    gSpriteSheetTexture->loadFromFile("data/spritesheet.png");
    
    //load font textures
    gFontTexture = new LTexture(gRenderer);
    healthFontTexture = new LTexture(gRenderer);
    mainMenuTitleTexture = new LTexture(gRenderer);
    mainMenuMovementInstructionsTexture = new LTexture(gRenderer);
    mainMenuShootInstructionsTexture = new LTexture(gRenderer);
    
    //init entities
    player = new Player(gRenderer, gSpriteSheetTexture, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    /* ENEMY SPAWNER CONSTRUCTION */
    bEnemySpawner.push_back(new BasicEnemySpawnPoint(gRenderer, gSpriteSheetTexture, player, SCREEN_WIDTH/4, 50));
    bEnemySpawner.push_back(new BasicEnemySpawnPoint(gRenderer, gSpriteSheetTexture, player, SCREEN_WIDTH/2, 50));
    bEnemySpawner.push_back(new BasicEnemySpawnPoint(gRenderer, gSpriteSheetTexture, player, 0, 50));
    bEnemySpawner.push_back(new BasicEnemySpawnPoint(gRenderer, gSpriteSheetTexture, player, SCREEN_WIDTH/6, 50));
    bEnemySpawner.push_back(new BasicEnemySpawnPoint(gRenderer, gSpriteSheetTexture, player, SCREEN_WIDTH/8, 50));
    
    //init audio handler
    audio = new Audio();
    //init game handler
    gHandler = new GameHandler();
    
    gTotalEnemiesKilled = 0;
    
    //set initial state
    GameState = GameStates::MAIN_MENU;
    
    /* 'GAME OVER' TEXT SETUP */
    //set the texture
    gFontTexture->loadFont("data/American Captain.ttf", 45);
    
    //set the text to be displayed
    gFontTexture->loadFromRenderedText("GAME OVER (Press 'Q' to quit)", fontColour);
    
    //create text object
    gText = new Text(gRenderer, gFontTexture, 150, SCREEN_HEIGHT/2);
    
    /* 'HEALTH' TEXT SETUP */
    healthFontTexture->loadFont("data/American Captain.ttf", 25);
    
    healthText = new Text(gRenderer, healthFontTexture, 660, 25);
    
    /* MAIN MENU TEXT SETUP */
    //Title
    mainMenuTitleTexture->loadFont("data/American Captain.ttf", 60);
    mainMenuTitleTexture->loadFromRenderedText("Music Shooter", fontColour);
    
    titleText = new Text(gRenderer, mainMenuTitleTexture, 150, SCREEN_HEIGHT/2);
    
    //Movement instructions
    mainMenuMovementInstructionsTexture->loadFont("data/American Captain.ttf", 30);
    mainMenuMovementInstructionsTexture->loadFromRenderedText("'WASD' to move. Press 'Q' to quit", fontColour);
    
    movementText = new Text(gRenderer, mainMenuMovementInstructionsTexture, 150, (SCREEN_HEIGHT/2+100));
    
    //Shooting instructions
    mainMenuShootInstructionsTexture->loadFont("data/American Captain.ttf", 27);
    mainMenuShootInstructionsTexture->loadFromRenderedText("'Arrow Keys' to shoot. Press '1' for game mode 1 and '2' for game mode 2", fontColour);
    
    shootingText = new Text(gRenderer, mainMenuShootInstructionsTexture, 100, (SCREEN_HEIGHT/2+150));
    
    //set start time
    gStartTime = SDL_GetTicks();

    return success;
}

bool loadMedia()
{
    bool success = true;
    
    if(!player->loadSheet())
    {
        printf("Couldn't player.loadsheet\n");
        success = false;
    }
    
    return success;
}

void close()
{
    //first we want to generate the analytics
    generateAnalytics();
    
    //then destroy/free memory
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;
    
    SDL_Quit();
    TTF_Quit();
    IMG_Quit();
    Mix_Quit();
}

void generateAnalytics()
{
    //create a text file
    std::ofstream analytics("data/Analytics.txt");
    
    //check to see if the text file is "open"
    if(analytics.is_open())
    {
        if(CurrentAudioState == AudioStates::ADAPTIVE)
        {
            analytics << "ADAPTIVE" << std::endl;
        }else if(CurrentAudioState == AudioStates::STATIC)
        {
            analytics << "STATIC" << std::endl;
        }
        
        //write the analytics to the file
        analytics << "ANALYTICS" << std::endl;
        analytics << "Total Time Played (Secs): " << gDeltaTime << std::endl;
        analytics << "Total No. of Key Presses: " << player->getNumOfKeyPresses() << std::endl;
        analytics << "Total Enemies Killed: " << gTotalEnemiesKilled << std::endl;
        analytics << "Total No. of bullets fired: " << player->getTotalBulletsFired() << std::endl;
        analytics << "Total Health Lost: " << player->getHealthLost() << std::endl;
        //then close the file once we're done
        analytics.close();
    }
}

int sumOfEnemiesKilled()
{
    int totalEnemiesKilled = 0;
    
    for(int i = 0; i < bEnemySpawner.size(); i++)
    {
        totalEnemiesKilled+= bEnemySpawner[i]->totalEnemiesKilled;
    }
    
    return totalEnemiesKilled;
}

//================================//
//----------MAIN METHOD----------//
int main(int argc, char const *argv[])
{
    if(!init())
    {
        printf("Failed to init!\n");
    }
    else
    {
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        SDL_Event e;
        
        //MAIN_MENU STATE
        while(GameState == GameStates::MAIN_MENU)
        {
            while(SDL_PollEvent(&e) != 0)
            {
                if(e.type == SDL_QUIT)
                {
                    GameState = GameStates::QUIT;
                }
            }
            //Clear Screen (Black Screen for now)
            SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
            SDL_RenderClear(gRenderer);
            
            //Press '1' or '2' to start game
            if( currentKeyStates[ SDL_SCANCODE_1 ] )
            {
                GameState = GameStates::PLAYING;
                CurrentAudioState = AudioStates::ADAPTIVE;
            }
            if( currentKeyStates[ SDL_SCANCODE_2 ] )
            {
                GameState = GameStates::PLAYING;
                CurrentAudioState = AudioStates::STATIC;
            }
            //Press 'Q' to quit
            if( currentKeyStates[ SDL_SCANCODE_Q ])
            {
                GameState = GameStates::QUIT;
            }
            
            titleText->draw();
            movementText->draw();
            shootingText->draw();
            
            //Update Screen
            SDL_RenderPresent(gRenderer);
        }
        
        //MAIN GAME LOOP
        while(GameState == GameStates::PLAYING)
        {
            while(SDL_PollEvent(&e) != 0)
            {
                if(e.type == SDL_QUIT)
                {
                    //quit = true;
                    GameState = GameStates::QUIT;
                }
            }
            
            //Get Mouse Position
            SDL_GetMouseState(&mouseX, &mouseY);
            
            //Clear Screen
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);
            
            //Press 'Q' to quit
            if( currentKeyStates[ SDL_SCANCODE_Q ])
            {
                GameState = GameStates::QUIT;
            }
            
            /*
            //Press 'R' to reset
            if( currentKeyStates[ SDL_SCANCODE_R ] )
            {
                //set to the playing game state
                GameState = GameStates::PLAYING;
                
                //reset player position (amongst other variables)
                player->reset();
                
                for(int i=0;i<bEnemySpawner.size();i++)
                {
                    //reset enemies in spawner
                    bEnemySpawner[i]->reset();
                }
                
                //resets game stuff (descriptive, I know)
                gHandler->reset();
            }
             */
            
            /* PLAYER STUFF */
            player->movementKeys();
            player->shootKeys();
            
            player->update();
            player->draw(mouseX, mouseY);
            
            /* ENEMY SPAWNERS */
            for(int i=0; i<bEnemySpawner.size(); i++)
            {
                bEnemySpawner[i]->setPlayerPosition(player->posX, player->posY);
                
                bEnemySpawner[i]->spawnEnemy();
                
                bEnemySpawner[i]->spawnerUpdate();
                bEnemySpawner[i]->spawnerDraw();
                
                for(int j=0; j<bEnemySpawner[i]->enemies.size(); j++)
                {
                    //Player collision detection
                    player->isHit(bEnemySpawner[i]->enemies[j]->getPosX(), bEnemySpawner[i]->enemies[j]->getPosY(),
                                  bEnemySpawner[i]->enemies[j]->getSize(), bEnemySpawner[i]->enemies[j]->getSize());
                    
                    if(player->health <= 0)
                    {
                        GameState = GameStates::GAME_OVER;
                    }
                }
                
                //update score
                gTotalEnemiesKilled = sumOfEnemiesKilled();
                //set score in the handler
                gHandler->setScore(gTotalEnemiesKilled);
            }
            
            /* AUDIO STUFF */
            if(CurrentAudioState == AudioStates::ADAPTIVE)
            {
                //Update layer volumes
                audio->update(gTotalEnemiesKilled);
                //increment certain tracks constantly
                audio->incrementTracks(player->mIsHit);
                //then play music
                audio->playMusic();
            }else if(CurrentAudioState == AudioStates::STATIC)
            {
                audio->playStaticMusic();
            }
            
            /* HEALTH TEXT */
            //update the "Health" string
            healthStream.str("");
            healthStream << "Health: " << player->health;
            
            healthFontTexture->loadFromRenderedText(healthStream.str().c_str(), healthColour);
            
            healthText->draw();
            
            //Update Screen
            SDL_RenderPresent(gRenderer);
            
            //update the time
            gDeltaTime = (SDL_GetTicks() - gStartTime) / 1000.0f;
            
            //GAMEOVER STATE
            while(GameState == GameStates::GAME_OVER)
            {
                while(SDL_PollEvent(&e) != 0)
                {
                    if(e.type == SDL_QUIT)
                    {
                        //quit = true;
                        GameState = GameStates::QUIT;
                    }
                }
                //Clear Screen (Black Screen for now)
                SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
                SDL_RenderClear(gRenderer);
                
                audio->stop();
                
                /*
                //Press 'R' to reset
                if( currentKeyStates[ SDL_SCANCODE_R ] )
                {
                    //set to the playing game state
                    GameState = GameStates::PLAYING;
                    
                    //reset player position (amongst other variables)
                    player->reset();
                    
                    for(int i=0;i<bEnemySpawner.size();i++)
                    {
                        //reset enemies in spawner
                        bEnemySpawner[i]->reset();
                    }
                    
                    //resets game stuff (descriptive, I know)
                    gHandler->reset();
                }
                 */
                
                //Press 'Q' to quit
                if( currentKeyStates[ SDL_SCANCODE_Q ])
                {
                    GameState = GameStates::QUIT;
                }
                //draw GAMEOVER text
                gText->draw();
                
                //Update Screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }
    
    close();
    return 0;
}
//================================//
