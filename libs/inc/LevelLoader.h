#ifndef __JJC__LEVEL__LOADER__H__
#define __JJC__LEVEL__LOADER__H__

#include <vector>
#include <fstream>
#include <string>

#include <SDL/SDL.h>

#include "FloatRect.h"
#include "RGBColor.h"

struct ColoredFloatRect {
    FloatRect rect;
    RGBColor color;
};

class LevelLoader {
public:
    std::vector<ColoredFloatRect> env;
    void loadLevel(std::string filename);
    void draw(SDL_Surface* surface);
};

#endif // __JJC__LEVEL__LOADER__H__