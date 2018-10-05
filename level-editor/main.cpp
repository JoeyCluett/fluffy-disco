#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
#include <cmath>

#include <SDL/SDL.h>
#include <inc/LevelLoader.h>
#include <inc/Map.h>
#include <inc/TextTag.h>

using namespace std;

int main(int argc, char* argv[]) {
    if(argc != 3) {
        cout << "Usage: " << argv[0] << " <filename> <-new|-edit>\n";
        return 1;
    }
    
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Rect** modes = SDL_ListModes(NULL, SDL_FULLSCREEN | SDL_HWSURFACE);
    SDL_Surface* surface = SDL_SetVideoMode(modes[0]->h, modes[0]->h, 32, 
            SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);

    TTF_Init();
    auto font = TTF_OpenFont("28dayslater.ttf", 28);
    TextTag state_tag(0.0f, 0.0f, 0.1f, 1.0f);
    state_tag.font = font;

    auto start_time = SDL_GetTicks();

    LevelLoader level_loader;
    if(string("-edit") == argv[2]) {
        level_loader.loadLevel(argv[1]);
    }

    FloatRect::screen_height = 1.2f;
    FloatRect::screen_width  = 1.2f;

    int background_color = SDL_MapRGB(surface->format, 255, 255, 255);

    int BLACK   = SDL_MapRGB(surface->format, 0, 0, 0);
    int WHITE   = SDL_MapRGB(surface->format, 255, 255, 255);
    int RED     = SDL_MapRGB(surface->format, 255, 0, 0);
    int GREEN   = SDL_MapRGB(surface->format, 0, 255, 0);
    int BLUE    = SDL_MapRGB(surface->format, 0, 0, 255);
    int YELLOW  = SDL_MapRGB(surface->format, 255, 255, 0);
    int CYAN    = SDL_MapRGB(surface->format, 0, 255, 255);
    int MAGENTA = SDL_MapRGB(surface->format, 255, 0, 255);

    int color_array[8] = {
        BLACK, WHITE, RED, GREEN,
        BLUE, YELLOW, CYAN, MAGENTA
    };

    RGBColor rgb_triplet_array[8] = {
        {0, 0, 0, 255}, {255, 255, 255, 255}, {255, 0, 0, 255}, {0, 255, 0, 255},
        {0, 0, 255, 255}, {255, 255, 0, 255}, {0, 255, 255, 255}, {255, 0, 255, 255}
    };

    int selected_color = BLACK;

    vector<FloatRect> color_rect_vec;
    for(int i = 0; i < 8; i++) {
        FloatRect fr{0.125f*i, 1.0f, 0.125f, 0.125};
        color_rect_vec.push_back(fr);
    }

    FloatRect background{0.0f, 0.0f, 1.0f, 1.0f};
    FloatRect right_side{1.0f, 0.0f, 1.0f, 0.2f};

    _2DPt pt1{0.0f, 0.0f};
    _2DPt pt2{0.0f, 0.0f};

    vector<SDLKey> input_chars;

    const int STATE_default = 0;
    const int STATE_has_first_corner = 1; // ignores further color changes and key presses
    const int STATE_polygon_finished = 2; 
    int current_state = STATE_default;

    bool draw_outlines_only = false;

    bool quit = false;
    while(!quit) {
        typedef bool flag_t;

        // will need to get tf's into [0,1)
        flag_t mouse_moved = false;
        int mouse_x = -1, mouse_y = -1;

        flag_t mouse_button = false;
        int button_x = -1, button_y = -1;

        flag_t has_chars = false;
        input_chars.clear();

        // should both corners be rendered?
        flag_t render_full_input_polygon = false;

        // calculate everything that needs to happen this frame
        // state machine uses active flags to determine what should
        // happen in current state
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_MOUSEBUTTONDOWN) {
                mouse_button = true;
                button_x = e.button.x;
                button_y = e.button.y;
            }
            else if(e.type == SDL_MOUSEMOTION) {
                mouse_moved = true;
                mouse_x = e.motion.x;
                mouse_y = e.motion.y;
            }
            else if(e.type == SDL_KEYDOWN) {
                has_chars = true;
                input_chars.push_back(e.key.keysym.sym);
            }
        }

        // all flags have been set appropriately
        switch(current_state) {
            case STATE_default:
                state_tag.text = "STATE_default";
                if(mouse_button) {
                    float mapped_x = map((float)button_x, 0.0f, (float)surface->w, 0.0f, 1.2f);
                    float mapped_y = map((float)button_y, 0.0f, (float)surface->h, 0.0f, 1.2f);

                    if(mapped_x > 1.0f)
                        break; // beyond the scope of any onscreen elements

                    if(mapped_y > 1.0f) {
                        // in the color area
                        for(int i = 0; i < 8; i++) {
                            if(color_rect_vec[i].hasPt({mapped_x, mapped_y})) {
                                selected_color = color_array[i];
                                break;
                            }
                        }
                    } else {
                        pt1 = {mapped_x, mapped_y};
                        pt2 = {mapped_x, mapped_y};
                        current_state = STATE_has_first_corner;
                        render_full_input_polygon = true;
                    }
                }
                break;
            case STATE_has_first_corner:
                state_tag.text = "STATE_has_first_corner";

                if(mouse_moved) { // waiting for second corner to be laid down
                    float mapped_x = map((float)mouse_x, 0.0f, (float)surface->w, 0.0f, 1.2f);
                    float mapped_y = map((float)mouse_y, 0.0f, (float)surface->h, 0.0f, 1.2f);

                    if(mapped_x > 1.0f || mapped_y > 1.0f)
                        break; // second point is not onscreen

                    pt2 = {mapped_x, mapped_y};
                    render_full_input_polygon = true;
                }

                if(mouse_button) {
                    float mapped_x = map((float)button_x, 0.0f, (float)surface->w, 0.0f, 1.2f);
                    float mapped_y = map((float)button_y, 0.0f, (float)surface->h, 0.0f, 1.2f);

                    if(mapped_x < 1.0f && mapped_y < 1.0f)
                        current_state = STATE_polygon_finished;
                }

                break;
            case STATE_polygon_finished:
                state_tag.text = "STATE_polygon_finished";
                {
                    float x = pt1.x < pt2.x ? pt1.x : pt2.x;
                    float y = pt1.y < pt2.y ? pt1.y : pt2.y;
                    float h = fabsf(pt1.y - pt2.y);
                    float w = fabsf(pt1.x - pt2.x);
                    FloatRect fr{x, y, h, w};

                    for(int i = 0; i < 8; i++) {
                        if(selected_color == color_array[i]) {
                            auto& c = rgb_triplet_array[i];
                            level_loader.addObject(fr, c.r, c.g, c.b);
                            break;
                        }
                    }
                }
                pt1 = {0.0f, 0.0f};
                pt2 = {0.0f, 0.0f};
                current_state = STATE_default;
                break;
            default:
                throw runtime_error("UNKNOWN STATE: " + to_string(current_state));
        }

        background.draw(surface, background_color);
        right_side.draw(surface, selected_color);

        // draw color picking polys
        for(int i = 0; i < 8; i++) {
            color_rect_vec[i].draw(surface, color_array[i]);
        }

        // draw existing environment
        if(draw_outlines_only)
            level_loader.drawOutline(surface);
        else
            level_loader.draw(surface);

        {
            float x = pt1.x < pt2.x ? pt1.x : pt2.x;
            float y = pt1.y < pt2.y ? pt1.y : pt2.y;
            float h = fabsf(pt1.y - pt2.y);
            float w = fabsf(pt1.x - pt2.x);
            FloatRect fr{x, y, h, w};
            fr.draw(surface, selected_color);
        }

        flag_t save_file = false;

        // see if we need to exit real quick
        if(has_chars) {
            for(auto& i : input_chars) {
                if(i == SDLK_ESCAPE)
                    quit = true;

                else if(i == SDLK_s) {
                    string save = "Saving file to: ";
                    save += argv[1];
                    state_tag.text = save;
                    save_file = true;
                }

                else if(i == SDLK_d)
                    draw_outlines_only = !draw_outlines_only;
            }
        }

        state_tag.draw(surface, 128, 128, 128);

        SDL_Flip(surface);

        if(save_file) {
            level_loader.saveLevel(argv[1]);
            SDL_Delay(1000);
        }

        // framerate regulation
        auto current_time = SDL_GetTicks();
        auto delta = current_time - start_time;
        if(delta < 25) // 40 FPS
            SDL_Delay(25 - delta);
        start_time = SDL_GetTicks();
    }

    TTF_Quit();
    SDL_Quit();
    return 0;
}