#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include <iostream>
#include <vector>
#include <list>
#include <cstdlib>

// standard render routines
#include <inc/FloatRect.h>
#include <inc/Polygon.h>
#include <inc/Map.h>
#include <inc/HitBox.h>
#include <inc/Projectile.h>
#include <inc/TextTag.h>
#include <inc/RGBColor.h>

// animation shiz
#include <inc/Animation.h>
#include <inc/MineExplosion.h>

using namespace std;

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_ShowCursor(SDL_DISABLE);

    // find an acceptable size for monitor resolution
    SDL_Rect** modes = SDL_ListModes(NULL, SDL_FULLSCREEN | SDL_HWSURFACE);
    SDL_Surface* surface = SDL_SetVideoMode(modes[0]->h, modes[0]->h, 32, 
            SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN); // suck it OpenGL

    // all fill operations use a mapping function
    // convert floats to integer screen coordinates
    FloatRect::screen_height = 1.00f;
    FloatRect::screen_width  = 1.00f;

    const int large_ship_size = 10;
    FloatRect large_ship[large_ship_size] = {
        {0.0f, 0.0f, 4.0f, 1.0f},

        // white line in the middle
        {0.48f, 0.2f, 3.6f, 0.04f}, // x, y, h, w

        // dashes next to middle line
        {0.6f, 0.2f, 0.3f, 0.04f},
        {0.6f, 0.8f, 0.3, 0.04f},
        {0.6f, 1.4f, 0.3f, 0.04f},
        {0.6f, 2.0f, 0.3, 0.04f},
        {0.6f, 2.6f, 0.3f, 0.04f},
        {0.6f, 3.2f, 0.3, 0.04f},

        // coning tower
        {-0.2f, 1.75f, 0.5f, 0.5f},
        {-0.1f, 1.85f, 0.3f, 0.3f}
    };
    RGBColor large_ship_color[large_ship_size] = {
        {100, 100, 100, 255},
        {255, 255, 255, 255},
        
        {255, 255, 255, 255},
        {255, 255, 255, 255},
        {255, 255, 255, 255},
        {255, 255, 255, 255},
        {255, 255, 255, 255},
        {255, 255, 255, 255},

        {175, 175, 175, 255},
        {75, 75, 75, 255},
    };

    // scale down the float rect so it fits in the 1.0 x 1.0 screen space
    for(int i = 0; i < large_ship_size; i++) {
        large_ship[i] *= _2DPt{0.25f, 0.25f};
        large_ship[i] +=  _2DPt{-0.125f, -0.5f};

        // shift to center of screen
        large_ship[i] += _2DPt{0.5f, 0.5f};
    }

    bool quit = false;
    while(!quit) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_KEYDOWN) {
                switch(e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    default:
                        break;
                }
            }
        }

        for(int i = 0; i < large_ship_size; i++) {
            auto& c = large_ship_color[i];
            int color = SDL_MapRGB(surface->format, c.r, c.g, c.b);
            large_ship[i].draw(surface, color);
        }

        SDL_Flip(surface);
        SDL_Delay(100);
    }

    SDL_Quit();
    return 0;
}