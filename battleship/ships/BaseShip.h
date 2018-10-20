#ifndef __JJC__BASE__SHIP__H__
#define __JJC__BASE__SHIP__H__

/*
    Other ships inherit this class
*/

#include <inc/FloatRect.h>
#include <inc/RGBColor.h>
#include <inc/TwoDimensionalPoint.h>
#include <vector>

class BaseShip {
private:
    // store the shapes used for rendering
    std::vector<FloatRect> __fr_vec;
    std::vector<int> __rgb_vec; // float vecs use an SDL formatted color
    std::vector<_2DPt> __hit_list;

public:
    // does a given point collide with this 
    // ship (this is battleship, after all)?
    virtual bool pointCollides(float x, float y) = 0;

    void draw(SDL_Surface* surface) {
        int s = __fr_vec.size();
        for(int i = 0; i < s; i++)
            __fr_vec[i].draw(surface, __rgb_vec[i]);
    }

    // need this or tends to crash (memory 
    // removed twice is generally frowned upon)
    virtual ~BaseShip();
};

#endif // __JJC__BASE__SHIP__H__