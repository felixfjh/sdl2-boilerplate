#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>

#define COLOR_GRAY (SDL_Color) { .r = 25, .g = 25, .b = 25, .a = 255 }
#define COLOR_RED (SDL_Color) { .r = 255, .g = 0, .b = 0, .a = 255 }

void set_color(SDL_Renderer *renderer, const SDL_Color *color);

#endif /* RENDER_H */
