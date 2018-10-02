#include "HitBox.h"
#include "FloatRect.h"
#include "Map.h"
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <cmath>
#include <iostream>

HitBox::HitBox(float x, float y, float l) {
    this->x = x;
    this->y = y;
    this->l = l;
}

bool HitBox::collides(FloatRect& fr) {
    float fr_middle_x = fr.x + (fr.w / 2.0f);
    float fr_middle_y = fr.y + (fr.h / 2.0f);

    float min_x = (fr.w / 2.0f) + (l/2.0f);
    float min_y = (fr.h / 2.0f) + (l/2.0f);

    if(fabsf(fr_middle_x - x) < min_x && fabs(fr_middle_y - y) < min_y) {
        return true;
    }

    return false;
}

bool HitBox::collides(HitBox& hb) {
    float min_len = (this->l + hb.l) / 2.0f;
    float len_x = fabsf(this->x + hb.x) / 2.0f;
    float len_y = fabsf(this->y + hb.y) / 2.0f;
    
    if(len_x < min_len && len_y < min_len)
        return true;
    return false;
}

void HitBox::draw(SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b) {
    Sint16 xs[2];
    Sint16 ys[2];

    float half_len = this->l / 2.0f;

    // upper left corner
    xs[0] = map(this->x - half_len, 
            FloatRect::screen_width_start, 
            FloatRect::screen_width_start + FloatRect::screen_width,  
            0.0f, (float)surface->w);

    ys[0] = map(this->y - half_len, 
            FloatRect::screen_height_start, 
            FloatRect::screen_height_start + FloatRect::screen_height, 
            0.0f, (float)surface->h);

    // lower right corner
    xs[1] = map(this->x + half_len, 
            FloatRect::screen_width_start, 
            FloatRect::screen_width_start + FloatRect::screen_width,  
            0.0f, (float)surface->w);

    ys[1] = map(this->y + half_len,
            FloatRect::screen_height_start, 
            FloatRect::screen_height_start + FloatRect::screen_height, 
            0.0f, (float)surface->h);
    
    rectangleRGBA(surface, xs[0], ys[0], xs[1], ys[1], r, g, b, 255);
}
