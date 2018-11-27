#include "FloatLine.h"
#include "FloatRect.h"
#include "Map.h"
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

FloatLine::FloatLine(void) : x1(0.0f), x2(0.0f), y1(0.0f), y2(0.0f) {}

FloatLine::FloatLine(float x1, float x2, float y1, float y2) : 
        x1(x1), x2(x2), y1(y1), y2(y2) {}

void FloatLine::draw(SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b) {
    float x1tf = map(this->x1, 
            FloatRect::screen_width_start, 
            FloatRect::screen_width_start + FloatRect::screen_width,  0.0f, (float)surface->w);
    
    float y1tf = map(this->y1, 
            FloatRect::screen_height_start, 
            FloatRect::screen_height_start + FloatRect::screen_height, 0.0f, (float)surface->h);

    float x2tf = map(this->x2, 
            FloatRect::screen_width_start, 
            FloatRect::screen_width_start + FloatRect::screen_width,  0.0f, (float)surface->w);
    
    float y2tf = map(this->y2, 
            FloatRect::screen_height_start, 
            FloatRect::screen_height_start + FloatRect::screen_height, 0.0f, (float)surface->h);

    lineRGBA(surface, x1tf, y1tf, x2tf, y2tf, r, g, b, 255);
}
