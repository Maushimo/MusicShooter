#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <iostream>
#include <string>

//================================//
//------ESSENTIAL VARIABLES------//
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
//================================//

//================================//
//-----TEXTURE WRAPPER CLASS-----//
class LTexture
{
public:
    //Initializes variables
    LTexture();
    
    //Deallocates memory
    ~LTexture();
    
    //Loads image at specified path
    bool loadFromFile( std::string path );
    
    //Deallocates texture
    void free();
    
    //Set color modulation
    void setColor( Uint8 red, Uint8 green, Uint8 blue );
    
    //Set blending
    void setBlendMode( SDL_BlendMode blending );
    
    //Set alpha modulation
    void setAlpha( Uint8 alpha );
    
    //Renders texture at given point
    void render( int x, int y, SDL_Rect* clip = NULL );
    
    //Gets image dimensions
    int getWidth();
    int getHeight();
    
private:
    //The actual hardware texture
    SDL_Texture* mTexture;
    
    //Image dimensions
    int mWidth;
    int mHeight;
};


LTexture::LTexture()
{
    //Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture()
{
    //Deallocate
    free();
}

bool LTexture::loadFromFile( std::string path )
{
    //Get rid of preexisting texture
    free();
    
    //The final texture
    SDL_Texture* newTexture = NULL;
    
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
        
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }
        
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    
    //Return success
    mTexture = newTexture;
    return mTexture != NULL;
}

void LTexture::free()
{
    //Free texture if it exists
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
    //Modulate texture rgb
    SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
    //Set blending function
    SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
    //Modulate texture alpha
    SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip )
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };
    
    //Set clip rendering dimensions
    if( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    
    //Render to screen
    SDL_RenderCopy( gRenderer, mTexture, clip, &renderQuad );
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}
//================================//

//================================//
//----------PLAYER CLASS----------//
class Player{
public:
    Player();
    ~Player();
    
    bool loadSheet();
    void draw();
    
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

private:
    float posX, posY;
    float size = 60;
    float speed = 10;
    LTexture gSpriteSheetTexture;
    SDL_Rect gSpriteClip;
};

Player::Player()
{
    posX = (SCREEN_WIDTH/2)-size/2;
    posY = (SCREEN_HEIGHT/2)-size/2;
}

Player::~Player()
{
}

bool Player::loadSheet()
{
    //success flag
    bool success = true;
    
    if(!gSpriteSheetTexture.loadFromFile("data/spritesheet.png"))
    {
        printf("Failed to load spritesheet texture SDL_Image Error: %s\n", IMG_GetError());
        success = false;
    }else
    {
        gSpriteClip.x = 0;
        gSpriteClip.y = 0;
        gSpriteClip.w = size;
        gSpriteClip.h = size;
    }
    
    return success;
}

void Player::draw()
{
    if(!this->loadSheet())
    {
        printf("Can't draw player sprite\n");
        return;
    }
    else
    {
        gSpriteSheetTexture.render(posX, posY, &gSpriteClip);
    }
}

void Player::moveLeft()
{
    posX-=speed;
}

void Player::moveRight()
{
    posX+=speed;
}

void Player::moveUp()
{
    posY-=speed;
}

void Player::moveDown()
{
    posY+=speed;
}
//================================//

//================================//
bool init();
void close();
bool loadMedia();
void movement();

Player player;

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
    return success;
}

bool loadMedia()
{
    bool success = true;
    
    if(!player.loadSheet())
    {
        printf("Couldn't player.loadsheet\n");
        success = false;
    }
    return success;
}

void close()
{
    //gSpriteSheetTexture.free();
    
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
        player.moveUp();
    }
    if( currentKeyStates[ SDL_SCANCODE_S ] )
    {
        player.moveDown();
    }
    if( currentKeyStates[ SDL_SCANCODE_A ] )
    {
        player.moveLeft();
    }
    if( currentKeyStates[ SDL_SCANCODE_D ] )
    {
        player.moveRight();
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
            movement();
            
            //Clear Screen
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);
            
            player.draw();
            
            //Update Screen
            SDL_RenderPresent(gRenderer);
        }
    }
    close();
    return 0;
}
//================================//
