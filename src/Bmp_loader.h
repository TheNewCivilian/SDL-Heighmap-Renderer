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

typedef struct _tile2{
  SDL_Texture *texture;
  int w;
  int h;
}Tile2;

typedef struct _tilemap2{
  Tile2 **tile;
  int am; //Amount of Tiles
}Tilemap2;


SDL_Surface *load_bmp_to_sdl(const char* filepath);
void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
long load_tile_to_sdl_surface(SDL_Surface *surface,const char* filepath,long offset,int num,int *with,int *heigt);
long load_tile_to_sdl_texture(SDL_Texture *texture,SDL_Renderer *renderer,const char* filepath,long offset,int num,int *with,int *heigt);
Tilemap *load_tilemap_2surface(const char* filepath,long offset,int num);
Tilemap2 *load_tilemap_surface2texture(const char* filepath,SDL_Renderer *renderer,long offset,int num);

#endif
