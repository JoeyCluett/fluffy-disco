#ifndef __JJC__TEXT__TAG__H__
#define __JJC__TEXT__TAG__H__

#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "FloatRect.h"

class TextTag : public FloatRect {
public:
    std::string text;
    TTF_Font* font;

    TextTag(float x, float y, float h, float w) : FloatRect(x, y, h, w) { }
    void draw(SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b);
};

#endif // __JJC__TEXT__TAG__H__