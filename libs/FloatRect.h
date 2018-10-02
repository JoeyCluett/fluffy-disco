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
    void setScreenSize(float h, float w);

    // render the square onto the given surface
    virtual void draw(SDL_Surface* surface, int color);

    FloatRect& operator+=(TwoDimensionalPoint& tdp);

    FloatRect translate(float x, float y) const {
        return FloatRect(this->x + x, this->y + y, this->h, this->w);
    }
};

#endif // __JJC__FLOAT__SQUARE__H__