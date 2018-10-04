#ifndef __JJC__POLYGON__H__
#define __JJC__POLYGON__H__

#include <initializer_list>
#include <SDL/SDL.h>
#include "TwoDimensionalPoint.h"

class Polygon {
public:
    float* x;
    float* y;
    const int n;

    static Sint16* tf_pts_x;
    static Sint16* tf_pts_y;
    static int tf_size;

    Polygon(int num_pts, 
        std::initializer_list<float> xs, 
        std::initializer_list<float> ys);

    Polygon(int num_pts, float* x, float* y);

    // constructor does nothing
    Polygon(void);

    Polygon(const Polygon& pref);

    virtual ~Polygon();

    void draw(SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b);
    void drawOutline(SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b);
    int size(void);

    auto getPoint(int index) -> TwoDimensionalPoint;

    Polygon rotate(float rads);

    void applyRotate(float rads);

    friend Polygon& operator*=(Polygon& p1, TwoDimensionalPoint tdp) {
        for(int i = 0; i < p1.n; i++) {
            p1.x[i] *= tdp.x;
            p1.y[i] *= tdp.y;
        }
        return p1;
    }

    friend Polygon& operator+=(Polygon& p1, TwoDimensionalPoint tdp) {
        for(int i = 0; i < p1.n; i++) {
            p1.x[i] += tdp.x;
            p1.y[i] += tdp.y;
        }
        return p1;
    }

    friend Polygon operator+(Polygon& p, TwoDimensionalPoint tdp) {
        Polygon ret = p;
        ret += tdp;
        return ret;
    }

};

#endif // __JJC__POLYGON__H__