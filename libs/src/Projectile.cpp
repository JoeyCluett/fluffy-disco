#include "Projectile.h"

#include <cmath>

Projectile::Projectile(void)
        : HitBox(0.0f, 0.0f, 0.0f) { ; }

Projectile::Projectile(float x, float y, float l, float dir, float speed) 
        : HitBox(x, y, l) {
            
    this->dir = dir;
    this->speed = speed;
}

void Projectile::setSpeedDirection(float speed, float direction) {
    this->dir = direction;
    this->speed = speed;
}

void Projectile::update(float delta_time) {
    this->x += (this->speed * delta_time) * cosf(this->dir);
    this->y += (this->speed * delta_time) * sinf(this->dir);
}