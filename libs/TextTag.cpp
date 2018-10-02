#include "TextTag.h"
#include "Map.h"

void TextTag::draw(SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b) {
    SDL_Color color{r, g, b};
    SDL_Surface* _surface = TTF_RenderText_Solid(
            this->font, this->text.c_str(), color);

    SDL_Rect rect;
    rect.x = map(x, 
            FloatRect::screen_width_start, 
            FloatRect::screen_width_start + FloatRect::screen_width, 
            0.0f, (float)surface->w);

    rect.w = map(w, 
            FloatRect::screen_width_start, 
            FloatRect::screen_width_start + FloatRect::screen_width, 
            0.0f, (float)surface->w);

    rect.y = map(y,
            FloatRect::screen_height_start,
            FloatRect::screen_height_start + FloatRect::screen_height,
            0.0f, (float)surface->h);

    rect.h = map(h,
            FloatRect::screen_height_start,
            FloatRect::screen_height_start + FloatRect::screen_height,
            0.0f, (float)surface->h);

    SDL_BlitSurface(_surface, NULL, surface, &rect);
    SDL_FreeSurface(_surface);
}
