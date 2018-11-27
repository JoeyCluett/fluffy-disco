#ifndef __JJC__PLAYER__MODEL__H__
#define __JJC__PLAYER__MODEL__H__

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <inc/Polygon.h>

class PlayerModel {
public:
    float x, y, theta;
    Polygon p;

    PlayerModel(void);
    void draw(SDL_Surface* screen, u_int8_t r, u_int8_t g, u_int8_t b, bool show_debug);

};

PlayerModel::PlayerModel(void) {
    
}

#endif // __JJC__PLAYER__MODEL__H__