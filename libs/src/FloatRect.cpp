#include "FloatRect.h"
#include "Map.h"

float FloatRect::screen_height_start = 0.0f;
float FloatRect::screen_height = 1.0f;

float FloatRect::screen_width_start = 0.0f;
float FloatRect::screen_width  = 1.0f;

FloatRect::FloatRect(void) : x(0.0f), y(0.0f), h(0.0f), w(0.0f) {}

FloatRect::FloatRect(float x, float y, float h, float w) : x(x), y(y), h(h), w(w) { }

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

FloatRect& FloatRect::operator+=(TwoDimensionalPoint& tdp) {
    this->x += tdp.x;
    this->y += tdp.y;
    return *this;
}