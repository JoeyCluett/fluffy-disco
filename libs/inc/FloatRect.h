#ifndef __JJC__FLOAT__SQUARE__H__
#define __JJC__FLOAT__SQUARE__H__

#include <SDL/SDL.h>
#include "Polygon.h"

class FloatRect {
public:
    float x, y, h, w; // 16 bytes

    static float screen_height;
    static float screen_width;
    static float screen_height_start;
    static float screen_width_start;

    // initialize a sqaure however you want
    FloatRect(float x, float y, float h, float w);

    // initializes everything to zero
    FloatRect(void);

    // set the screen size in terms of 
    // whatever scale you want
    static void setScreenSize(float h, float w);

    // see if the given point resides in this FloatRect
    bool hasPt(_2DPt tdp);

    // render the square onto the given surface
    virtual void draw(SDL_Surface* surface, int color);
    virtual void drawOutline(SDL_Surface* surface, uint8_t r, uint8_t g, uint8_t b);

    // translate
    FloatRect& operator+=(TwoDimensionalPoint tdp);

    // scale
    FloatRect& operator*=(TwoDimensionalPoint tdp);

    FloatRect translate(float x, float y) const {
        return FloatRect(this->x + x, this->y + y, this->h, this->w);
    }
} __attribute__((packed));

#endif // __JJC__FLOAT__SQUARE__H__