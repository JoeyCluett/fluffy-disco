#ifndef __JJC__PROJECTILE__H__
#define __JJC__PROJECTILE__H__

#include "HitBox.h"
#include "FloatRect.h"

class Projectile : public HitBox {
public:
    // when Projectile is updated, the position changes
    float dir;   // between 0 -> 2*M_PI

    // speed is implicitly units per second
    float speed; // can be whatever

    // set everything needed for initialization and update
    Projectile(float x, float y, float l, float dir, float speed);

    // default constructor
    Projectile(void);

    // set update parameters
    void setSpeedDirection(float speed, float direction);

    // update the projectile internally
    void update(float delta_time);
};

#endif // __JJC__PROJECTILE__H__