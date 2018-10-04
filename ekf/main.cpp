#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include <iostream>
#include <vector>
#include <list>
#include <cstdlib>

#include <inc/FloatRect.h>
#include <inc/Polygon.h>
#include <inc/Map.h>
#include <inc/HitBox.h>
#include <inc/Projectile.h>
#include <inc/TextTag.h>

using namespace std;

namespace ekf {
    float player_x = 0.0f;
    float player_y = 50.0f;

    float a = 0.5f; // scale previous state estimation
    float b = 0.5f; // scale control input signal
    float c = 1.0f; // scale current state estimation

    float r = 0.2f; // average noise for environment

    float p_k = 0.0f; // prediction noise
    float x_hat = player_x;

} // end of namespace ekf

// random float [0.0, 1.0)
float randFloat(void) {
    return float(rand()) / (double(RAND_MAX) + 1.0f);
}

float randFloat(float min, float max) {
    float f = randFloat();
    return map(f, 0.0f, 1.0f, min, max);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    //TTF_Init(); // prepare to load ttf files

    SDL_ShowCursor(SDL_DISABLE);

    // find an acceptable size for monitor resolution
    SDL_Rect** modes = SDL_ListModes(NULL, SDL_FULLSCREEN | SDL_HWSURFACE);
    SDL_Surface* surface = SDL_SetVideoMode(modes[0]->h, modes[0]->h, 32, 
            SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN); // suck it OpenGL

    FloatRect::screen_height = 100.0f;
    FloatRect::screen_width  = 100.0f;

    FloatRect background{0.0f, 0.0f, 100.0f, 100.0f};

    Polygon player_model(4, 
            {0.0f, 2.0f,  0.0f, -2.0f}, // x coordinates
            {0.0f, -2.0f, 4.0f, -2.0f}  // y coordinates
    );
    player_model.applyRotate(-M_PI/2.0f);

    float player_x = 0.0f;
    float player_y = 50.0f;
    float player_speed = 20.0f;
    Polygon tfd_player = player_model + _2DPt{player_x, player_y};

    bool player_slow_down = false;
    bool player_speed_up  = false;

    bool quit = false;
    while(!quit) {
        if(player_x >= 100.0f)
            quit = true;

        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_KEYDOWN) {
                switch(event.key.keysym.sym) {
                    case SDLK_UP:
                        player_speed_up = true;
                        player_slow_down = false;
                        break;
                    case SDLK_DOWN:
                        player_speed_up = false;
                        player_slow_down = true;
                        break;
                    default:
                        break;
                }
            } else if(event.type == SDL_KEYUP) {
                switch(event.key.keysym.sym) {
                    case SDLK_UP:
                        player_speed_up = false;
                        break;
                    case SDLK_DOWN:
                        player_slow_down = false;
                        break;
                    default:
                        break;
                }
            }
        }

        float u_k = 0.0f; // control signal

        if(player_speed_up) {
            player_x += (0.025f * player_speed * 2.0f);
            u_k = 2.0f;
        } else if(player_slow_down) {
            player_x += (0.025f * player_speed * 0.5f);
            u_k = 0.5f;
        } else {
            player_x += (0.025f * player_speed); // constant 40 FPS
        }

        // PREDICT
        float x_hat = ekf::a * ekf::x_hat + ekf::b * u_k; // current state estimation
                float v_k = randFloat(-0.2f, 0.2f);
        float z_k   = ekf::c * x_hat + v_k; // observed state

        // UPDATE
        // calculate gain for this cycle
        float g_k = ekf::p_k * ekf::c / (ekf::c * ekf::p_k * ekf::c + ekf::r);
        ekf::x_hat = x_hat + g_k * (z_k - ekf::c * x_hat);
        ekf::p_k   = (1.0f - g_k * ekf::c) * ekf::p_k;

        ekf::player_x = ekf::x_hat;

        // render the player and the calculated position
        background.draw(surface, SDL_MapRGB(surface->format, 0, 0, 0));
        {
            Polygon player_ref = player_model;
            player_ref += {player_x, player_y};
            player_ref.draw(surface, 255, 0, 0);
        }

        {
            Polygon player_ref = player_model;
            player_ref += {ekf::player_x, ekf::player_y};
            player_ref.drawOutline(surface, 0, 0, 255);
        }

        SDL_Flip(surface);
        SDL_Delay(25); // 40 FPS
    }

    tfd_player.draw(surface, 255, 0, 0);
    SDL_Flip(surface);
    SDL_Delay(2000);

    //TTF_Quit();
    SDL_Quit();
}