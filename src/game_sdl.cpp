#include "game_sdl.h"

bool GAME_SDL::init() {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return false;
  }

  // Create window
  this->sdl_window = SDL_CreateWindow(
    "TileMapRenderer",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
  );

  if (this->sdl_window == NULL) {
    printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
    return false;
  }

  // Create Renderer Context
  this->render = SDL_CreateRenderer(
    this->sdl_window,
    -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  );
  if (this->render == NULL) {
    printf( "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
    return false;
  }
  return true;
}

bool GAME_SDL::loadMedia() {
  this->base_tilemap = load_tilemap_surface2texture("../assets/BaseMaptiles.bmp",this->render,0,1);
  printf("loadMedia successfull!\n");
  return true;
}

void GAME_SDL::close() {
  SDL_DestroyWindow(this->sdl_window);
  this->sdl_window = NULL;
  SDL_DestroyRenderer(this->render);
  this->render = NULL;
  SDL_Quit();
}

SDL_Renderer* GAME_SDL::getrenderer(){
  return this->render;
}

SDL_Event* GAME_SDL::getevent(){
  return &(this->sdl_event);
}

Tilemap2* GAME_SDL::getbasetilemap(){
  return this->base_tilemap;
}