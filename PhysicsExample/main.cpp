#include <iostream>

#include <Box2D/Box2D.h>

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

#include <inc/FloatRect.h>
#include <inc/Map.h>
#include <inc/RGBColor.h>

using namespace std;

struct RGB_B2Body {
    b2Body* body;
    RGBColor color;
    unsigned int start_time;
};

int main(int argc, char* argv[]) {

    float object_start_height = 30.0f;

    // array of colors
    RGBColor rgb_arr[] = {
        {255, 0, 0, 255},
        {0, 255, 0, 255},
        {0, 0, 255, 255},
        {255, 255, 0, 255},
        {255, 0, 255, 255},
        {255, 255, 255, 255}
    };

    //b2Vec2 gravity(0.0f, 98.1f);
    b2Vec2 gravity(0.0f, 0.0f);
    b2World world(gravity);

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(object_start_height, (2*object_start_height)+10.0f);
    b2Body* ground_body = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(object_start_height, 10.0f);
    ground_body->CreateFixture(&groundBox, 0.0f);    

    // dimensions of the dynamic collision bodies
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    float timestep = 1.0f / 60.0f;
    int32 vel_iters = 12;//6;
    int32 pos_iters = 10;//2;

    // set up our graphical environment
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Rect** modes = SDL_ListModes(NULL, SDL_FULLSCREEN | SDL_HWSURFACE);
    SDL_Surface* surface = SDL_SetVideoMode(modes[0]->h, modes[0]->h, 32, 
            SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);

    // change scaling for our screen
    FloatRect::screen_height = 2*object_start_height;
    FloatRect::screen_width  = 2*object_start_height;

    Polygon draw_polygon(
        4,
        {-1.0f, 1.0f, 1.0f, -1.0f},
        {-1.0f, -1.0f, 1.0f, 1.0f}
    );

    vector<RGB_B2Body> body_ptr_vec;
    vector<RGB_B2Body> body_circle_vec;

    //for(int i = 0; i < 300; i++) {
    bool quit = false;
    bool mouse_used = false;
    float mouse_x, mouse_y;
    while(!quit) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }
            else if(e.type == SDL_MOUSEBUTTONDOWN) {
                mouse_used = true;
                mouse_x = e.button.x;
                mouse_y = e.button.y;
            }
        }

        if(mouse_used) {
            float mapped_x = map(mouse_x, 0.0f, (float)surface->w, 0.0f, FloatRect::screen_width);
            float mapped_y = map(mouse_y, 0.0f, (float)surface->h, 0.0f, FloatRect::screen_height);

            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(mapped_x, mapped_y);

            b2Body* body = world.CreateBody(&bodyDef);

            b2FixtureDef fixtureDef;
            fixtureDef.shape = &dynamicBox;
            fixtureDef.density = 1.0f;
            fixtureDef.friction = 0.3f;

            body->CreateFixture(&fixtureDef);
            //body->SetAngularVelocity(3.0f*M_PI);
            body_ptr_vec.push_back({body, rgb_arr[rand() % 6], SDL_GetTicks()});

            // ================================
            // generate a circle body

            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(mapped_x, mapped_y);

            body = world.CreateBody(&bodyDef);

            b2CircleShape circleshape;
            circleshape.m_p.Set(0.0f, 0.0f);
            circleshape.m_radius = 1.0f;

            fixtureDef.shape = &circleshape;
            
            body->CreateFixture(&fixtureDef);
            body_circle_vec.push_back({body, rgb_arr[rand() % 6], SDL_GetTicks()});

            mouse_used = false;
        }

        world.Step(timestep, vel_iters, pos_iters);

        // render sequence
        SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));
        
        for(auto& ptr : body_ptr_vec) {
            auto position = ptr.body->GetPosition();
            auto angle    = ptr.body->GetAngle();

            auto p = draw_polygon.rotate(angle);
            p += _2DPt{position.x, position.y};

            RGBColor& c = ptr.color;

            p.draw(surface, c.r, c.g, c.b);
        }

        for(auto& ptr : body_circle_vec) {
            auto position = ptr.body->GetPosition();

            float x = map(position.x, 0.0f, FloatRect::screen_width, 0.0f, (float)surface->w);
            float y = map(position.y, 0.0f, FloatRect::screen_height, 0.0f, (float)surface->h);
            float r = map(1.0f, 0.0f, FloatRect::screen_height, 0.0f, (float)surface->h);

            RGBColor& c = ptr.color;
            circleRGBA(surface, x, y, r, c.r, c.g, c.b, 255);
        }

        SDL_Flip(surface);

        // iterate through body array, remove items that are past their lifetime
        auto current_time = SDL_GetTicks();
        for(auto iter = body_ptr_vec.begin(); iter != body_ptr_vec.end();) {
            RGB_B2Body& b = *iter;
            if(current_time - b.start_time > 20000) { // 5 seconds
                world.DestroyBody(b.body);
                iter = body_ptr_vec.erase(iter);
            } else {
                iter++;
            }
        }

        for(auto iter = body_circle_vec.begin(); iter != body_circle_vec.end();) {
            RGB_B2Body& b = *iter;
            if(current_time - b.start_time > 20000) {
                world.DestroyBody(b.body);
                iter = body_circle_vec.erase(iter);
            } else {
                iter++;
            }
        }

        SDL_Delay(14);
    }

    SDL_Quit();
    return 0;
}