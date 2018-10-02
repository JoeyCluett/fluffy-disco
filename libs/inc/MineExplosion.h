#include "Animation.h"
#include <SDL/SDL.h>
#include <vector>
#include "TwoDimensionalPoint.h"

/*
    MineExplosion waits a few seconds then detonates
    sending probes for collision detection
*/

class MineExplosion : public Animation {
private:
    float total_animation_time = 0.0f;
    float distance = 0.0f;
    float x, y;

public:
    MineExplosion(float x, float y);

    bool update(float delta_time) override;
    bool update(unsigned int delta_time) override;
    void draw(SDL_Surface* surface, uint8_t r, uint8_t g, uint8_t b) override;
};
