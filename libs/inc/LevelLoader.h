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
} __attribute__((packed)); // should be exactly 16 bytes all the time

class LevelLoader {
private:
    std::vector<ColoredFloatRect> env;
    
public:
    void loadLevel(std::string filename);
    void saveLevel(std::string filename);

    void draw(SDL_Surface* surface);
    void drawOutline(SDL_Surface* surface);
    void addObject(FloatRect fr, 
            uint8_t r, uint8_t g, uint8_t b);
};

#endif // __JJC__LEVEL__LOADER__H__