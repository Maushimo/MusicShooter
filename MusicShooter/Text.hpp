//
//  Text.hpp
//  MusicShooter
//
//  Created by Mohsin Yusuf on 10/04/2017.
//  Copyright © 2017 Mohsin Yusuf. All rights reserved.
//

#ifndef Text_hpp
#define Text_hpp

#include "LTexture.hpp"

#include <iostream>

class Text
{
public:
    Text(SDL_Renderer* r, LTexture* fontTexture, float x, float y);
    ~Text();
    
    //draw the text texture
    void draw();
    
private:
    //position
    float posX, posY;
    
    //global renderer
    SDL_Renderer* gRenderer;
    //global font
    LTexture* gFontTexture;
};

#endif /* Text_hpp */
