#include <iostream>
#include <SDL/SDL.h>
#include "util.h"

using namespace std;

int main() {

    SDL_Init(SDL_INIT_EVERYTHING);
    auto win = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE);

    while(true) { 
        SDL_FillRect(win, NULL, SDL_MapRGB(win->format, 0, 0, 0));
        
        int x, y;
        SDL_PumpEvents();
        SDL_GetMouseState(&x, &y);
        Util::draw_triangle(
            { 100, 20, 540, 20, x, y }, 
            { 1.0f, 0.0f, 0.0f }, 
            { 0.0f, 1.0f, 0.0f }, 
            { 0.0f, 0.0f, 1.0f }, 
            win);

        SDL_Flip(win);
        //SDL_Delay(15);
    }

    SDL_Quit();
    return 0;
}

