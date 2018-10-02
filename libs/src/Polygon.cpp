#include "Polygon.h"
#include "FloatRect.h"
#include "Map.h"
#include <string>
#include <stdexcept>
#include <initializer_list>

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

// some static initializer stuff needs to happen before
Sint16* Polygon::tf_pts_x = new Sint16[8];
Sint16* Polygon::tf_pts_y = new Sint16[8];
int Polygon::tf_size = 8;

Polygon::Polygon(
        int num_pts, 
        std::initializer_list<float> xs, 
        std::initializer_list<float> ys) : n(num_pts) {

    if(xs.size() != ys.size())
        throw std::runtime_error("Polygon::Polygon -> initializer list sizes need to be the same");

    this->x = new float[num_pts];
    this->y = new float[num_pts];

    int index = 0;
    for(auto f : xs) {
        this->x[index] = f;
        index++;
    }

    index = 0;
    for(auto f : ys) {
        this->y[index] = f;
        index++;
    }

}

Polygon::Polygon(int num_pts, float* x, float* y) : n(num_pts) {
    this->x = new float[num_pts];
    this->y = new float[num_pts];

    for(int i = 0; i < num_pts; i++) {
        this->x[i] = x[i];
        this->y[i] = y[i];
    }
}

Polygon::Polygon(void) : n(0) {  }

// copy constructor
Polygon::Polygon(const Polygon& pref) : n(pref.n) {
    this->x = new float[pref.n];
    this->y = new float[pref.n];

    for(int i = 0; i < pref.n; i++) {
        this->x[i] = pref.x[i];
        this->y[i] = pref.y[i];
    }
}

// cant have those pesky memory leaks
Polygon::~Polygon(void) {
    delete[] this->x;
    delete[] this->y;
}

void Polygon::draw(SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b) {
    for(int i = 0; i < this->n; i++) {
        this->tf_pts_x[i] = map(this->x[i], 
                FloatRect::screen_width_start, 
                FloatRect::screen_width_start + FloatRect::screen_width, 
                0.0f, (float)surface->w);

        this->tf_pts_y[i] = map(this->y[i], 
                FloatRect::screen_height_start, 
                FloatRect::screen_height_start + FloatRect::screen_height, 
                0.0f, (float)surface->h);
    }

    filledPolygonRGBA(surface, this->tf_pts_x, this->tf_pts_y, this->n, r, g, b, 255);
}

void Polygon::drawOutline(SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b) {
    for(int i = 0; i < this->n; i++) {
        this->tf_pts_x[i] = map(this->x[i], 
                FloatRect::screen_width_start, 
                FloatRect::screen_width_start + FloatRect::screen_width, 
                0.0f, (float)surface->w);

        this->tf_pts_y[i] = map(this->y[i], 
                FloatRect::screen_height_start, 
                FloatRect::screen_height_start + FloatRect::screen_height, 
                0.0f, (float)surface->h);
    }

    polygonRGBA(surface, this->tf_pts_x, this->tf_pts_y, this->n, r, g, b, 255);
}

auto Polygon::getPoint(int index) -> TwoDimensionalPoint {
    TwoDimensionalPoint tdp;
    tdp.x = this->x[index];
    tdp.y = this->y[index];
    return tdp;
}

int Polygon::size(void) {
    return this->n;
}

Polygon Polygon::rotate(float rads) {
    Polygon p = *this;

    for(int i = 0; i < p.n; i++) {
        p.x[i] = x[i]*cos(rads) - y[i]*sin(rads);
        p.y[i] = y[i]*cos(rads) + x[i]*sin(rads);
    }

    return p;
}

void Polygon::applyRotate(float rads) {
    float tx, ty;

    for(int i = 0; i < this->n; i++) {
        tx = x[i];
        ty = y[i];

        x[i] = tx*cos(rads) - ty*sin(rads);
        y[i] = ty*cos(rads) + tx*sin(rads);
    }
}
