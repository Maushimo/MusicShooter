//
//  LTexture.hpp
//  MusicShooter
//
//  Created by Mohsin Yusuf on 02/03/2017.
//  Copyright © 2017 Mohsin Yusuf. All rights reserved.
//

#ifndef LTexture_hpp
#define LTexture_hpp

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <iostream>

//-----TEXTURE WRAPPER CLASS-----//
class LTexture
{
public:
    //Initializes variables
    LTexture(SDL_Renderer* r);
    
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
    void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
    
    //Gets image dimensions
    int getWidth();
    int getHeight();
    
private:
    //The actual hardware texture
    SDL_Texture* mTexture;
    
    SDL_Renderer* gRenderer;
    
    //Image dimensions
    int mWidth;
    int mHeight;
};


#endif /* LTexture_hpp */
