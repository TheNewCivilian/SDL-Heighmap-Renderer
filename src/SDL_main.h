#ifndef SDL_MAIN_H
#define SDL_MAIN_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include "Bmp_loader.h"
#include "./helpers/debug_color.h"
#include "map_main.h"
#include "renderer_main.h"

const int SCREEN_WIDTH = 1924;
const int SCREEN_HEIGHT = 1600;
const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 32;

class SDL_MAIN {
  public:
    SDL_Window* gWindow = NULL;
    SDL_Renderer *render = NULL;

    SDL_Event e;
    Tilemap2 *base_tilemap;

  public:
    bool init();
    bool loadMedia();
    void close();
    SDL_Renderer *getrenderer();
    SDL_Event *getevent();
    Tilemap2 *getbasetilemap();
};

#endif
