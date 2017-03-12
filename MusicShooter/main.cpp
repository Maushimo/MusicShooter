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
    
    player = new Player(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT, gSpriteSheetTexture);
    audio = new Audio();
    
    for(int i = 0; i < 10; i ++){
        bEnemy[i] = new BasicEnemy(gRenderer, i*10, i*10, gSpriteSheetTexture);
    }
    
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
    
    for(int i = 0; i < 10; i ++)
    {
        if(!bEnemy[i]->loadSheet())
        {
            printf("Couldn't BasicEnemy.loadsheet\n");
            success = false;
        }
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
    IMG_Quit();
    Mix_Quit();
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
        bool quit = false;
        SDL_Event e;
        
        while(!quit)
        {
            while(SDL_PollEvent(&e) != 0)
            {
                if(e.type == SDL_QUIT)
                {
                    quit = true;
                }
            }
            
            //Get Mouse Position
            SDL_GetMouseState(&mouseX, &mouseY);
            
            //Clear Screen
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);
            
            player->movementKeys();
            
            const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
            
            if( currentKeyStates[ SDL_SCANCODE_1 ] )
            {
                audio->muteDrums();
            }
            
            if( currentKeyStates[ SDL_SCANCODE_2 ])
            {
                audio->muteBass();
            }
            
            player->update();
            player->draw(mouseX, mouseY);
            
            for(int i=0;i<10;i++)
            {
                bEnemy[i]->update();
                bEnemy[i]->draw();
            }
            
            //Update Screen
            SDL_RenderPresent(gRenderer);
            
            //audio->playMusic();
        }
    }
    close();
    return 0;
}
//================================//
