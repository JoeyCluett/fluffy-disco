#include <iostream>

#include <Box2D/Box2D.h>

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

#include <inc/FloatRect.h>
#include <inc/Map.h>

using namespace std;

int main(int argc, char* argv[]) {

    b2Vec2 gravity(0.0f, -0.981f);
    b2World world(gravity);

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);

    b2Body* ground_body = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(20.0f, 10.0f);
    ground_body->CreateFixture(&groundBox, 0.0f);

    float object_start_height = 10.0f;

    // define a dynamic body
    b2BodyDef bodyDef; // collision body
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(object_start_height, object_start_height);
    b2Body* body = world.CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    // define dynamic body fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);

    float timestep = 1.0f / 60.0f;
    int32 vel_iters = 6;
    int32 pos_iters = 2;

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

    std::vector<float> y_pos_record;

    for(int i = 0; i < 300; i++) {
        world.Step(timestep, vel_iters, pos_iters);
        auto position = body->GetPosition();
        auto angle    = body->GetAngle();

        auto p = draw_polygon + _2DPt{position.x, position.y};

        // render sequence
        SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));
        p.draw(surface, 255, 0, 0);
        SDL_Flip(surface);
        SDL_Delay(25);

        cout << "x: " << position.x << ", y: " << position.y << ", angle: " << angle << endl;
        y_pos_record.push_back(position.y);
    }

    bool quit = false;
    while(!quit) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }
        }

        SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));

        // render a line indicating what the record was
        for(int i = 0; i < surface->w -1; i++) {
            float first_x  = map((float)i, 0.0f, (float)surface->w, 0.0f, 300.0f);
            float second_x = map((float)(i+1), 0.0f, (float)surface->w, 0.0f, 300.0f);
            int first_y  = map(y_pos_record[first_x], 0.0f, object_start_height, (float)surface->h, 0.0f);
            int second_y = map(y_pos_record[second_x], 0.0f, object_start_height, (float)surface->h, 0.0f);

            lineRGBA(surface, i, first_y, i+1, second_y, 0, 255, 0, 255);
        }
        SDL_Flip(surface);
        SDL_Delay(40);
    }

    SDL_Quit();
    return 0;
}