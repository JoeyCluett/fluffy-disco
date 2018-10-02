#ifndef __JJC__ANIMATION__H__
#define __JJC__ANIMATION__H__

#include <SDL/SDL.h>

class Animation {
public:
    // these methods implemented by animation classes
    virtual bool update(float delta_time)        = 0;
    virtual bool update(unsigned int delta_time) = 0;
    virtual void draw(SDL_Surface* surface, uint8_t r, uint8_t g, uint8_t b) = 0;
};

#endif // __JJC__ANIMATION__H__