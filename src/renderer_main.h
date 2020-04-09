#ifndef RENDERER_MAIN_H
#define RENDERER_MAIN_H

#include "game_sdl.h"

class RENDERER{
  public:
    int GAME_X = 0;
    int GAME_Y = 0;
    int GAME_ZOOM = 0;


  protected:
    SDL_Renderer *render;
    Tilemap2 *base_tilemap;
    MAP_GEN *map_gen;

  public:
    bool moveMap(int x,int y);
    bool renderMap(int , int);
    int setbase_tilemap(Tilemap2 *);
    int setrenderer(SDL_Renderer *);
    int setmapgen(MAP_GEN *);
    int *getGAME_X();
    int *getGAME_Y();
    int *getGAME_ZOOM();
};

#endif
