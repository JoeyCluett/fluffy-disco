#ifndef __JJC__HIT__BOX__H__
#define __JJC__HIT__BOX__H__

#include "FloatRect.h"
#include "Polygon.h"
#include <SDL/SDL.h>
#include <cmath>

// to save on memory consumption, all 
// HitBox objects only maintain three variables
class HitBox {
public:
    float x, y, l;

    // initialize all parts with given values
    HitBox(float x, float y, float l);

    // does the hit box collide with 
    // a given FloatRect
    bool collides(FloatRect& fr);

    // does the hitbox collide with a given HitBox
    // should always return true if argument
    // is self
    bool collides(HitBox& hb);

    // uses SDL_gfx color information
    void draw(SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b);

    HitBox rotate(float rads) {
        HitBox hb = *this;
        hb.x = x*cos(rads) - y*sin(rads);
        hb.y = y*cos(rads) + x*sin(rads);
        hb.l = l;
        return hb;
    }

    friend HitBox& operator*=(HitBox& hb, float f) {
        hb.x *= f;
        hb.y *= f;
        hb.l *= f;
        return hb;
    }

    friend HitBox& operator+=(HitBox& hb, TwoDimensionalPoint tdp) {
        hb.x += tdp.x;
        hb.y += tdp.y;
        return hb;
    }

};

#endif // __JJC__HIT__BOX__H__