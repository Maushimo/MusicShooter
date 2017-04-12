#include "main.h"

bool init()
{
    bool success = true;
    
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
    GameState = GameStates::PLAYING;
    
    /* 'GAME OVER' TEXT SETUP */
    //set the texture
    gFontTexture->loadFont("data/American Captain.ttf", 45);
    
    //set the text to be displayed
    gFontTexture->loadFromRenderedText("GAME OVER (Press 'R' to restart)", fontColour);
    
    //create text object
    gText = new Text(gRenderer, gFontTexture, 150, SCREEN_HEIGHT/2);
    
    /* 'HEALTH' TEXT SETUP */
    healthFontTexture->loadFont("data/American Captain.ttf", 25);
    
    healthText = new Text(gRenderer, healthFontTexture, 660, 25);

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
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;
    
    SDL_Quit();
    TTF_Quit();
    IMG_Quit();
    Mix_Quit();
}

int sumOfEnemiesKilled()
{
    int totalEnemiesKilled;
    
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
            
            player->movementKeys();
            player->shootKeys(audio);
            
            player->update();
            player->draw(mouseX, mouseY);
            
            for(int i=0; i<bEnemySpawner.size(); i++)
            {
                bEnemySpawner[i]->setPlayerPosition(player->posX, player->posY);
                
                bEnemySpawner[i]->spawnEnemy();
                
                bEnemySpawner[i]->spawnerUpdate();
                bEnemySpawner[i]->spawnerDraw();
                
                for(int j=0; j<bEnemySpawner[i]->enemies.size(); j++)
                {
                    //Player collision detection
                    player->isHit(bEnemySpawner[i]->enemies[j]->getPosX(), bEnemySpawner[i]->enemies[j]->getPosY(), bEnemySpawner[i]->enemies[j]->getSize(), bEnemySpawner[i]->enemies[j]->getSize());
                    
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
            
            //Update layer volumes
            audio->update(bEnemySpawner[0]->totalEnemiesKilled);
            //then play music
            audio->playMusic();
            
            //update the "Health" string
            healthStream.str("");
            healthStream << "Health: " << player->health;
            
            healthFontTexture->loadFromRenderedText(healthStream.str().c_str(), healthColour);
            
            healthText->draw();
            
            //Update Screen
            SDL_RenderPresent(gRenderer);
            
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
