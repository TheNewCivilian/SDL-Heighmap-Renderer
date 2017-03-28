#ifndef MAIN_SDL_H
#define MAIN_SDL_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include "bitmap.h"
#include "bittypes.h"
#include "Bmp_loader.h"

#define KNORMAL  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGREEN  "\x1B[32m"
#define KYELLOW  "\x1B[33m"
#define KBLUE  "\x1B[34m"
#define KMAGENTA  "\x1B[35m"
#define KCYAN  "\x1B[36m"
#define KWHITE  "\x1B[37m"

typedef struct _map{
  int *index;
}Map;

typedef struct _heighmap{
  int *index;
  int *height;
  int *hdraw;
  int mapsize;
  int width;
}Heightmap;

//Starts up SDL and creates window
bool init();
//Loads media
bool loadMedia();
 //Frees media and shuts down SDL
void close();

//Screen dimension constants
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 1200;

const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 32;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

SDL_Renderer *render = NULL;
//Event handler
SDL_Event e;

Tilemap *tilemap;
Tilemap2 *tilemap2;

Map *GameMap;
Heightmap *	GameMap2;
//Gameposition
int GAME_X = 0;
int GAME_Y = 0;
int GAME_ZOOM = 0;

int *w = (int *) malloc(sizeof(int));
int *mapsize = (int *) malloc(sizeof(int));

#endif
