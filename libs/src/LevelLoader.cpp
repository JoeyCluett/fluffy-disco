#include <LevelLoader.h>

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <string>
#include <fstream>

/*
    Level file format:
    
        i32 n -> number of tiles needed for level (read(#bytes) as needed)
                ~2,000,000,000 MAX
        for(i = [0, n])
            ColoredFloatRect : place in env

        ... should be end of file (error if not)
*/

void LevelLoader::loadLevel(std::string filename) {
    std::ifstream file(filename, std::ios::binary);
    
    int n;
    file.read((char*)&n, 4);

    ColoredFloatRect cfr;
    for(int i = 0; i < n; i++) {
        float attr[4];
        uint8_t c[4];

        file.read((char*)attr, 16);
        file.read((char*)c, 4);

        cfr.rect.x = attr[0];
        cfr.rect.y = attr[1];
        cfr.rect.h = attr[2];
        cfr.rect.w = attr[3];

        cfr.color.r = c[0];
        cfr.color.g = c[1];
        cfr.color.b = c[2];
        cfr.color.a = c[3];

        this->env.push_back(cfr);
    }

    file.close();
}

void LevelLoader::saveLevel(std::string filename) {
    std::ofstream file(filename, std::ios::binary | std::ios::trunc);
    
    int n = this->env.size();
    file.write((char*)&n, 4);
    
    for(auto& fr : this->env) {
        float attr[4];
        uint8_t c[4];

        attr[0] = fr.rect.x;
        attr[1] = fr.rect.y;
        attr[2] = fr.rect.h;
        attr[3] = fr.rect.w;

        c[0] = fr.color.r;
        c[1] = fr.color.g;
        c[2] = fr.color.b;
        c[3] = fr.color.a;

        file.write((char*)attr, 16);
        file.write((char*)c, 4);
    }

    file.close();
}

void LevelLoader::addObject(FloatRect fr, uint8_t r, uint8_t g, uint8_t b) {
    ColoredFloatRect cfr;
    cfr.rect = fr;
    cfr.color = {r, g, b, 0xFF};

    this->env.push_back(cfr);
}

void LevelLoader::draw(SDL_Surface* surface) {
    for(auto& fr : this->env) {
        auto& c = fr.color;
        fr.rect.draw(surface, SDL_MapRGB(surface->format, c.r, c.g, c.b));
    }
}

void LevelLoader::drawOutline(SDL_Surface* surface) {
    for(auto& fr : this->env) {
        fr.rect.drawOutline(surface, fr.color.r, fr.color.g, fr.color.b);
    }
}
