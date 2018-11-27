#pragma once

#include <SDL/SDL.h>
#include "Polygon.h"

class FloatLine {
public:
    float x1, x2, y1, y2;

    FloatLine(void);
    FloatLine(float x1, float x2, float y1, float y2);

    void draw(SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b);
};