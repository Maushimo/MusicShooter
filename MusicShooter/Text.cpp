//
//  Text.cpp
//  MusicShooter
//
//  Created by Mohsin Yusuf on 10/04/2017.
//  Copyright © 2017 Mohsin Yusuf. All rights reserved.
//

#include "Text.hpp"

Text::Text(SDL_Renderer* r, LTexture* fontTexture, float x, float y)
{
    gRenderer = r;
    gFontTexture = fontTexture;
    
    posX = x;
    posY = y;
}

Text::~Text()
{
    //Deallocate
    gFontTexture->free();
}

void Text::draw()
{
    gFontTexture->render(posX, posY);
}
