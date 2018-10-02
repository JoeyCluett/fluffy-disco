#include "MineExplosion.h"
#include <inc/Animation.h>
#include <cmath>
#include <inc/FloatRect.h>
#include <stdexcept>

MineExplosion::MineExplosion(float x, float y) {
    this->total_animation_time = 0.0f;
    this->x = x;
    this->y = y;
}

bool MineExplosion::update(float delta_time) {
    this->total_animation_time += delta_time;

    if(total_animation_time > 3.0f) {
        if(total_animation_time > 4.5f) {
            return true;
        } else {
            // 3.0 < t < 4.5
            this->distance = (total_animation_time - 3.0) / 10.0f; 
            return false;
        }
    } else {
        return false;
    }

    throw std::runtime_error("MineExplosion::update() -> wtf happened!");
}

bool MineExplosion::update(unsigned int delta_time) {
    return this->update(float(delta_time) / 1000.0f); // use the float instead
}

void MineExplosion::draw(SDL_Surface* surface, uint8_t r, uint8_t g, uint8_t b) {
    const float partial = (M_PI * 2.0f) / 20.0f;
    int color = SDL_MapRGB(surface->format, r, g, b);

    for(int i = 0; i < 20; i++) {
        float rot = i * partial;
        FloatRect fr(x + distance*cosf(rot), y + distance*sinf(rot), 0.005, 0.005);
        fr.draw(surface, color);
    }

}
