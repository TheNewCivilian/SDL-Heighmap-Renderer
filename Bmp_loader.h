#ifndef BMP_LOADER_H
#define BMP_LOADER_H

#include <SDL2/SDL.h>

SDL_Surface *load_bmp_to_sdl(const char* filepath);
void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
int load_tile_to_sdl(const char* filepath,int offset,SDL_Surface *surface);

#endif
