#ifndef __JJC__CARRIER__H__
#define __JJC__CARRIER__H__

#include "BaseShip.h"
#include <initializer_list>

class Carrier : public BaseShip {
public:
    Carrier(std::initializer_list<FloatRect> init, std::initializer_list<RGBColor> color);
    bool collides(float x, float y) override;
};

#endif // __JJC__CARRIER__H__