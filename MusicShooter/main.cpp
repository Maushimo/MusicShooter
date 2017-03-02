#include "main.h"

bool init()
{
    bool success = true;
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
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
            }
        }
    }
    player = new Player(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);

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
    IMG_Quit();
}

void movement()
{
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    
    if( currentKeyStates[ SDL_SCANCODE_W ] )
    {
        player->moveUp();
    }
    if( currentKeyStates[ SDL_SCANCODE_S ] )
    {
        player->moveDown();
    }
    if( currentKeyStates[ SDL_SCANCODE_A ] )
    {
        player->moveLeft();
    }
    if( currentKeyStates[ SDL_SCANCODE_D ] )
    {
        player->moveRight();
    }
}
//================================//

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
            //std::cout << "Mouse X: " << mouseX << " Mouse Y: " << mouseY << std::endl;
            
            //Movement keys
            movement();
            
            //Clear Screen
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);
            
            player->update();
            player->draw(mouseX, mouseY);
            
            //Update Screen
            SDL_RenderPresent(gRenderer);
        }
    }
    close();
    return 0;
}
//================================//
