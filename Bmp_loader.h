#ifndef BMP_LOADER_H
#define BMP_LOADER_H

#include <SDL2/SDL.h>

typedef struct _tile{
  SDL_Surface *surface;
  int w;
  int h;
}Tile;

typedef struct _tilemap{
  Tile **tile;
  int am; //Amount of Tiles
}Tilemap;


SDL_Surface *load_bmp_to_sdl(const char* filepath);
void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
long load_tile_to_sdl(SDL_Surface *surface,const char* filepath,long offset,int num,int *with,int *heigt);
Tilemap *load_tilemap(const char* filepath,long offset,int num);

#endif
