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

// animation shiz
#include <inc/Animation.h>
#include <inc/MineExplosion.h>

using namespace std;

int fontSize(SDL_Surface* surface, float s) {
    return map(s, 0.0f, FloatRect::screen_height, 0.0f, (float)surface->h);
}

float randFloat(void) {
    return float(rand()) / (double(RAND_MAX) + 1.0f);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init(); // prepare to load ttf files

    SDL_ShowCursor(SDL_DISABLE);

    // find an acceptable size for monitor resolution
    SDL_Rect** modes = SDL_ListModes(NULL, SDL_FULLSCREEN | SDL_HWSURFACE);
    SDL_Surface* surface = SDL_SetVideoMode(modes[0]->h, modes[0]->h, 32, 
            SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN); // suck it OpenGL

    auto font = TTF_OpenFont("28dayslater.ttf", 28);
    SDL_Surface* text;
    SDL_Color text_color = {255, 255, 255};
    text = TTF_RenderText_Solid(font, "Fluffy Disco", text_color);    

    // all fill operations use a mapping function
    // convert floats to integer screen coordinates
    FloatRect::screen_height = 1.05f;
    FloatRect::screen_width  = 1.05f;

    //FloatRect::screen_height = 2.0f;
    //FloatRect::screen_width = 2.0f;

    // just a few frequently used colors
    int RED        = SDL_MapRGB(surface->format, 255, 0, 0);
    int GREEN      = SDL_MapRGB(surface->format, 0, 255, 0);
    int BLUE       = SDL_MapRGB(surface->format, 0, 0, 255);
    int DARK_BLUE  = SDL_MapRGB(surface->format, 0, 0, 100);
    int LIGHT_GREY = SDL_MapRGB(surface->format, 100, 100, 100);
    int BLACK      = SDL_MapRGB(surface->format, 0, 0, 0);
    int WHITE      = SDL_MapRGB(surface->format, 255, 255, 255);
    int YELLOW     = SDL_MapRGB(surface->format, 255, 255, 0);

    // effectively blanks the video buffer
    FloatRect background{0.0f, 0.0f, 1.0f, 1.0f};
    FloatRect black_background{0.0f, 0.0f, 1.2f, 1.2f};

    FloatRect status_bars_background{0.0f, 1.0f, 0.3f, 1.5f};
    FloatRect right_side_background{1.0f, -0.5f, 2.0f, 0.5f};

    // playfield plus some decorative tiles
    FloatRect r1[] = {
        // playfield used for collision detection
        {0.125f, 0.125f, 0.25f, 0.25f}, // x, y, h, w
        {0.625f, 0.125f, 0.25f, 0.25f},
        {0.125f, 0.625f, 0.25f, 0.25f},
        {0.625f, 0.625f, 0.25f, 0.25f},
        
        // decorative tiles
        {0.125f + 0.025f, 0.125f + 0.025f, 0.2f, 0.2f},
        {0.625f + 0.025f, 0.125f + 0.025f, 0.2f, 0.2f},
        {0.125f + 0.025f, 0.625f + 0.025f, 0.2f, 0.2f},
        {0.625f + 0.025f, 0.625f + 0.025f, 0.2f, 0.2f},

        // perimeter used for collision detection
        {-1.0f, -1.0f, 3.0f, 1.0f}, // left
        {1.0f, -1.0f,  3.0f, 1.0f}, // right
        {-1.0f, -1.0f, 1.0f, 3.0f}, // top
        {-1.0f, 1.0f,  1.0f, 3.0f}  // bottom
    };

    const float player_scale = 0.01f;

    TwoDimensionalPoint tdp_scale{player_scale, player_scale};
    TwoDimensionalPoint tdp_translate{0.5f, 0.5f};

    FloatRect projectile{0.0f, 0.0, 0.005, 0.005};
    projectile += tdp_translate;

    // player (fighter ship)
    Polygon poly(4, 
            {0.0f, 2.0f,  0.0f, -2.0f},   // x coordinates
            {0.0f, -2.0f, 4.0f, -2.0f}); // y coordinates
    poly *= tdp_scale;

    vector<HitBox> hbs = {
        {0, 1, 2},
        {-1.5f, -1, 0.7},
        {1.5f, -1, 0.7},
        {0, 2.5f, 1},
        {-1, -0.25f, 1},
        {1, -0.25, 1},
        {0, 3.25f, 0.5f}
    };

    // scale the hitbox list appropriately
    for(auto& hb : hbs)
        hb *= player_scale;

    // a few runtime parameters
    bool turn_left   = false;
    bool turn_right  = false;
    bool go_forward  = false;
    bool go_backward = false;

    // projectile storage
    list<Projectile> fr_list;
    bool fire = false;
    long int last_fire_frame = -100L; // should be able to fire on first frame

    bool drop_mine = false;

    const int WEAPON_mg     = 0;
    const int WEAPON_shotty = 1;
    const int WEAPON_sniper = 2;
    const int WEAPON_mine   = 3;
    const int WEAPON_remote = 4;
    int current_weapon_state = WEAPON_mg;

    const char* weapon_name_mg   = "Machine Gun";
    const char* weapon_name_shot = "Shotgun    ";

    float fire_meter = 1.0f;
    FloatRect fire_indicator{0.0f, 1.0f, 0.025f, 1.0f};
    FloatRect shield_indicator{0.0f, 1.025f, 0.025f, 1.0f};
    bool shield = false;
    float shield_meter_amount = 1.0f;

    TextTag fire_label(0.01f, 1.0f, 0.012f, 0.175f);
    fire_label.font = font;

    TextTag shield_label(0.01f, 1.025f, 0.025f, 1.0f);
    shield_label.font = font;
    shield_label.text = "Shield";

    list<Animation*> animation_list;

    // more of a debugging thing
    bool show_hitboxes = false;

    // player position
    float rads = 0.0;
    float player_x_pos = 0.5f;
    float player_y_pos = 0.5f;

    // for framerate regulation
    auto frame_start_time = SDL_GetTicks();
    long int frame_count = 0L;

    // wait for user to press something
    bool quit = false;
    do {

        float tmp_rads = rads;
        float tmp_player_x_pos = player_x_pos;
        float tmp_player_y_pos = player_y_pos;

        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_KEYDOWN) {
                switch(e.key.keysym.sym) {
                    case SDLK_LEFT:
                        turn_left = true; break;
                    case SDLK_RIGHT:
                        turn_right = true; break;
                    case SDLK_ESCAPE:
                        quit = true; break;
                    case SDLK_h:
                        show_hitboxes = !show_hitboxes; break;
                    case SDLK_UP:
                        go_forward = true; break;
                    case SDLK_DOWN:
                        go_backward = true; break;
                    case SDLK_g:
                        switch(current_weapon_state) {
                            case WEAPON_mg:
                                current_weapon_state = WEAPON_shotty;
                                break;
                            case WEAPON_shotty:
                                current_weapon_state = WEAPON_mg;
                                break;
                            default:
                                break;
                        }
                        break;

                    case SDLK_s:
                        shield = !shield;
                        break;

                    case SDLK_d:
                        drop_mine = true; break;

                    case SDLK_SPACE:
                    case SDLK_f:
                        fire = true; break;
                    default:
                        break;
                }
            } else if(e.type == SDL_KEYUP) {
                switch(e.key.keysym.sym) {
                    case SDLK_LEFT:
                        turn_left = false; break;
                    case SDLK_RIGHT:
                        turn_right = false; break;
                    case SDLK_UP:
                        go_forward  = false; break;
                    case SDLK_DOWN:
                        go_backward = false; break;
                    default:
                        break;
                }
            }            
        }

        // apply rotation according to flags
        if(turn_left) {
            tmp_rads -= (M_PI*2.0)/150.0f;
        }
        if(turn_right) {
            tmp_rads += (M_PI*2.0)/150.0f;
        }

        // clamp rotation
        if(tmp_rads > (M_PI*2.0))
            tmp_rads -= (M_PI*2.0);
        else if(tmp_rads < 0.0)
            tmp_rads = (M_PI*2.0) + tmp_rads;

        if(go_forward) {
            tmp_player_x_pos += 0.003 * cos(rads);
            tmp_player_y_pos += 0.003 * sin(rads);
        } else if(go_backward) {
            tmp_player_x_pos -= 0.003 * cos(rads);
            tmp_player_y_pos -= 0.003 * sin(rads);
        }

        black_background.draw(surface, BLACK); // blank draw buffer
        background.draw(surface, LIGHT_GREY); 
        
        // draw projectiles
        for(auto& f : fr_list) {
            f.update(0.0125f); // in staying with the 40FPS mark
            f.draw(surface, 255, 255, 255);
        }
        
        for(int i = 0; i < 4; i++)
            r1[i].draw(surface, DARK_BLUE);
        for(int i = 4; i < 8; i++) // decoration
            r1[i].draw(surface, BLACK);
    
        std::vector<HitBox> tf_hitboxes;

        for(auto& hb : hbs) {
            HitBox tmp = hb.rotate(tmp_rads - (M_PI/2.0));
            tmp += {tmp_player_x_pos, tmp_player_y_pos};
            
            // add tf'd hitbox to detection queue
            tf_hitboxes.push_back(tmp);
        }

        bool should_update_persistent = true;
        for(auto& hb : tf_hitboxes) {
            for(int i = 0; i < 4 && should_update_persistent; i++) {
                if(hb.collides(r1[i])) {
                    should_update_persistent = false; // dont update player position
                    //r1[i].draw(surface, RED);
                }
            }

            for(int i = 8; i < 12 && should_update_persistent; i++) {
                if(hb.collides(r1[i])) {
                    should_update_persistent = false;
                    //r1[i].draw(surface, RED);
                }
            }
        }

        if(should_update_persistent) {
            player_x_pos = tmp_player_x_pos;
            player_y_pos = tmp_player_y_pos;
            rads = tmp_rads;
        }

        Polygon pref = poly.rotate(rads - (M_PI/2));
        pref += {player_x_pos, player_y_pos};
        pref.draw(surface, 255, 0, 0);
        
        // shields
        if(shield && shield_meter_amount > 0.01f) {
            shield_meter_amount -= 0.005f;
            pref.drawOutline(surface, 255, 255, 255);
            if(shield_meter_amount < 0.01f)
                shield = false;
        }

        // conditionally show hitboxes
        if(show_hitboxes) {
            for(auto& tmp : tf_hitboxes)
                tmp.draw(surface, 0, 255, 255);
        }

        if(fire) {

            switch(current_weapon_state) {
                case WEAPON_mg:
                    // 4 projectiles / second
                    if(fire_meter > 0.15f && (frame_count - last_fire_frame) > 10L) {
                        float rand_rads = rads + ((randFloat()-0.5f) * 0.15f); // random shoot off-center
                        Projectile fr(player_x_pos, player_y_pos, 0.005f, rand_rads, 1.0f);
                        fr_list.push_back(fr);
                        last_fire_frame = frame_count;

                        fire_meter -= 0.15;
                    }
                    break;

                case WEAPON_shotty:
                    // 1 projectile every 1.5 seconds
                    if(fire_meter > 0.5f) {
                        for(int i = 0; i < 10; i++) { // for every pellet
                            float rand_rads = rads + ((randFloat() - 0.5f) * 0.35f); // more spread
                            Projectile pr(player_x_pos, player_y_pos, 0.005f, rand_rads, 1.0f);
                            fr_list.push_back(pr);
                        }
                        fire_meter -= 0.5f;
                    }
                    break;

                default:
                    SDL_Quit();
                    TTF_Quit();
                    throw runtime_error("UNKNOWN WEAPON TYPE");
                    break;
            }

            fire = false;
        }

        // add mine to animation list and let go
        if(drop_mine) {
            if(fire_meter > 0.6f) {
                animation_list.push_back(new MineExplosion(player_x_pos, player_y_pos));
                fire_meter -= 0.6f;
            }
            drop_mine = false;
        }

        fire_meter += 0.005f;
        if(fire_meter > 1.0f)
            fire_meter = 1.0f;
        fire_indicator.w = fire_meter;

        shield_meter_amount += 0.002;
        if(shield_meter_amount > 1.0f)
            shield_meter_amount = 1.0f;
        shield_indicator.w = shield_meter_amount;

        // update (and remove) projectiles
        // ignore on-field barriers, only test out-of-bounds
        if((frame_count & 0L)) {
            for(auto iter = fr_list.begin(); iter != fr_list.end(); iter++) {
                auto& e = *iter;
                for(int i = 8; i < 12; i++) {
                    if(e.collides(r1[i]))
                        iter = fr_list.erase(iter);
                }
            }
        }

        // every 2 frames, test barrier collision
        if(!(frame_count & 1L)) {
            for(auto iter = fr_list.begin(); iter != fr_list.end(); iter++) {
                auto& e = *iter;
                for(int i = 0; i < 4; i++) {
                    if(e.collides(r1[i])) {
                        iter = fr_list.erase(iter);
                        break;
                    }
                }
            }
        }

        // update and draw animations
        for(auto iter = animation_list.begin(); iter != animation_list.end(); iter++) {
            bool result = (*iter)->update(0.025f); // 40 FPS
            (*iter)->draw(surface, 0, 255, 255);
            if(result) {
                delete (*iter); // remove associated animation object
                iter = animation_list.erase(iter);
            }
        }

        status_bars_background.draw(surface, BLACK);
        right_side_background.draw(surface, BLACK);

        fire_indicator.draw(surface, YELLOW);
        shield_indicator.draw(surface, GREEN);

        // use the proper label on the weapon
        switch(current_weapon_state) {
            case WEAPON_mg:
                fire_label.text = "Rifle  "; break;
            case WEAPON_shotty:
                fire_label.text = "Shotgun"; break;
            default:
                fire_label.text = "UNKNOWN";
                break;
        }

        //fire_label.text = "Hello World";
        fire_label.draw(surface, 50, 50, 50);
        shield_label.draw(surface, 50, 50, 50);

        // show some text
        SDL_BlitSurface(text, NULL, surface, NULL);

        SDL_Flip(surface);

        auto frame_end_time = SDL_GetTicks();
        auto frame_delta = frame_end_time - frame_start_time;
        if(frame_delta < 25)
            SDL_Delay(25 - frame_delta); // ~40 FPS
        frame_start_time = frame_end_time;

        // sometimes, its nice to know which frame we are on
        frame_count++;

    } while(!quit);

    TTF_Quit(); // free all of the font resources
    SDL_Quit(); // ... quit SDL
    return 0;
}