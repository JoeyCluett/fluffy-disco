#pragma once

#include <cmath>
#include <initializer_list>
#include <SDL/SDL.h>

struct Triangle { float x0, y0, x1, y1, x2, y2; };
struct Triplet { float a, b, c; };
struct BoundingBox { int x_min, x_max, y_min, y_max; };

Triplet operator*(Triplet t, float f) {
    return {t.a*f, t.b*f, t.c*f};
}

Triplet operator+(Triplet t, Triplet s) {
    return {t.a+s.a, t.b+s.b, t.c+s.c};
}

namespace Util {

    static inline float area_of(Triangle t) {
        float tmp = t.x0*(t.y1 - t.y2) + t.x1*(t.y2 - t.y0) + t.x2*(t.y0 - t.y1);
        tmp /= 2.0f;
        return (tmp < 0.0f ? -tmp : tmp);
    }

    static inline bool in_triangle(Triangle& t, float x, float y) {
        float a = ((t.y1 - t.y2)*(x - t.x2) + (t.x2 - t.x1)*(y - t.y2)) / ((t.y1 - t.y2)*(t.x0 - t.x2) + (t.x2 - t.x1)*(t.y0 - t.y2));
        float b = ((t.y2 - t.y0)*(x - t.x2) + (t.x0 - t.x2)*(y - t.y2)) / ((t.y1 - t.y2)*(t.x0 - t.x2) + (t.x2 - t.x1)*(t.y0 - t.y2));
        float c = 1.0f - a - b;
        return (a >= 0.0f && b >= 0.0f && c >= 0.0f && a <= 1.0f && b <= 1.0f && c <= 1.0f);
    }

    static inline bool is_between(float in, float min, float max) {
        return (in >= min && in <= max);
    }

    static BoundingBox get_bounding_box_of(Triangle& t) {
        BoundingBox bb{(int)t.x0, (int)t.x0, (int)t.y0, (int)t.y0};
        for(float f : {t.x0, t.x1, t.x2}) {
            if(f < bb.x_min) bb.x_min = f;
            if(f > bb.x_max) bb.x_max = f;
        }
        for(float f : {t.y0, t.y1, t.y2}) {
            if(f < bb.y_min) bb.y_min = f;
            if(f > bb.y_max) bb.y_max = f;
        }
        return bb;
    }

    static Triplet normalize(Triplet& t) {
        float n_vec = std::sqrt(t.a*t.a + t.b*t.b + t.c*t.c);
        return {t.a/n_vec, t.b/n_vec, t.c/n_vec};
    }

    static Triplet get_color_of(Triangle& t, float x, float y, float tri_area) {
        float l0 = area_of({x, y, t.x1, t.y1, t.x2, t.y2});
        float l1 = area_of({x, y, t.x0, t.y0, t.x2, t.y2});
        float l2 = area_of({x, y, t.x1, t.y1, t.x0, t.y0});

        return {l0/tri_area, l1/tri_area, l2/tri_area};
    }

    static float length_between(float x0, float y0, float x1, float y1) {
        float tmp = (x0-x1)*(x0-x1) + (y0-y1)*(y0-y1);
        return std::sqrt(tmp);
    }

    void draw_triangle(Triangle t, Triplet c0, Triplet c1, Triplet c2, SDL_Surface* win) {
        auto bb = get_bounding_box_of(t);
        auto tri_area = area_of(t);

        for(int y = bb.y_min; y <= bb.y_max; y++) {
        for(int x = bb.x_min; x <= bb.x_max; x++) {
            if(in_triangle(t, x, y)) {
                // point is in triangle
                auto tri = get_color_of(t, x, y, tri_area);
                SDL_Rect r{(Sint16)x, (Sint16)y, 1, 1};

                auto c = c0*tri.a + c1*tri.b + c2*tri.c;

                SDL_FillRect(win, &r, SDL_MapRGB(win->format, 255*c.a, 255*c.b, 255*c.c));
            }
        }
        }
    }

} // end of namespace util


