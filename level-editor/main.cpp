#include <iostream>

#include <SDL/SDL.h>

using namespace std;

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Rect** modes = SDL_ListModes(NULL, SDL_FULLSCREEN | SDL_HWSURFACE);
    SDL_Surface* surface = SDL_SetVideoMode(modes[0]->h, modes[0]->h, 32, 
            SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN); // suck it OpenGL

    

    SDL_Quit();
    return 0;
}