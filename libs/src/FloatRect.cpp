#include "FloatRect.h"
#include "Map.h"
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

float FloatRect::screen_height_start = 0.0f;
float FloatRect::screen_height = 1.0f;

float FloatRect::screen_width_start = 0.0f;
float FloatRect::screen_width  = 1.0f;

FloatRect::FloatRect(void) : x(0.0f), y(0.0f), h(0.0f), w(0.0f) {}

FloatRect::FloatRect(float x, float y, float h, float w) : x(x), y(y), h(h), w(w) { }

bool FloatRect::hasPt(_2DPt tdp) {
    if(tdp.x < x || tdp.y < y)
        return false;
    if(tdp.x > x+w || tdp.y > y+h)
        return false;
    return true;
}

void FloatRect::draw(SDL_Surface* surface, int color) {
    SDL_Rect rect;
    rect.h = map(this->h, 
            this->screen_height_start, 
            this->screen_height_start + this->screen_height, 0.0f, (float)surface->h);

    rect.w = map(this->w, 
            this->screen_width_start, 
            this->screen_width_start + this->screen_width,  0.0f, (float)surface->w);
    
    rect.x = map(this->x, 
            this->screen_width_start, 
            this->screen_width_start + this->screen_width,  0.0f, (float)surface->w);
    
    rect.y = map(this->y, 
            this->screen_height_start, 
            this->screen_height_start + this->screen_height, 0.0f, (float)surface->h);

    SDL_FillRect(surface, &rect, color);
}

void FloatRect::drawOutline(SDL_Surface* surface, uint8_t r, uint8_t g, uint8_t b) {
    SDL_Rect rect;
    rect.h = map(this->h, 
            this->screen_height_start, 
            this->screen_height_start + this->screen_height, 0.0f, (float)surface->h);

    rect.w = map(this->w, 
            this->screen_width_start, 
            this->screen_width_start + this->screen_width,  0.0f, (float)surface->w);
    
    rect.x = map(this->x, 
            this->screen_width_start, 
            this->screen_width_start + this->screen_width,  0.0f, (float)surface->w);
    
    rect.y = map(this->y, 
            this->screen_height_start, 
            this->screen_height_start + this->screen_height, 0.0f, (float)surface->h);

    rectangleRGBA(surface, rect.x, rect.y, rect.x+rect.w, rect.y+rect.h, r, g, b, 255);
}

FloatRect& FloatRect::operator+=(TwoDimensionalPoint& tdp) {
    this->x += tdp.x;
    this->y += tdp.y;
    return *this;
}