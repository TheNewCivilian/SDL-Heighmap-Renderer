#include "main.h"

int main( int argc, char* args[] )
{
  GAME_SDL sdl_main;
  MAP_GEN map_gen;
  RENDERER renderer;
  bool exit_loop = false;

  //Initialize SDL
  if( !sdl_main.init() )
  {
    printf("Failed to initialize!\n");
    return 1;
  }

  //Load media
  if( !sdl_main.loadMedia() )
  {
    printf("Failed to load media!\n");
    return 1;
  }

  renderer.setrenderer(sdl_main.getrenderer());

  map_gen.load_highMap("../assets/Megatest.bmp");

  renderer.setmapgen(&map_gen);
  renderer.setbase_tilemap(sdl_main.getbasetilemap());
  SDL_RenderClear(sdl_main.getrenderer());
  renderer.renderMap(0,0);
  SDL_RenderPresent(sdl_main.getrenderer());
  while(!exit_loop) {
    while( SDL_PollEvent( sdl_main.getevent() ) != 0 ) {
      //User requests quit
      if ( sdl_main.getevent()->type == SDL_QUIT ) {
        exit_loop = true;
      } else if( sdl_main.getevent()->type == SDL_KEYDOWN ) {
        switch( sdl_main.getevent()->key.keysym.sym ) {
            case SDLK_UP:
              renderer.moveMap(0,25);
              break;
            case SDLK_DOWN:
              renderer.moveMap(0,-25);
              break;
            case SDLK_LEFT:
              renderer.moveMap(25,0);
              break;
            case SDLK_RIGHT:
              renderer.moveMap(-25,0);
              break;
            case SDLK_ESCAPE:
              exit_loop = true;
              break;
            case SDLK_PLUS:
              if (*renderer.getGAME_ZOOM() > -32){
                *renderer.getGAME_ZOOM() = *renderer.getGAME_ZOOM()-1;
              }
              renderer.moveMap((TILE_WIDTH-*renderer.getGAME_ZOOM()*4)/2,-(TILE_HEIGHT-*renderer.getGAME_ZOOM()*2)/2);
              break;
            case SDLK_MINUS:
              if (*renderer.getGAME_ZOOM() < 10){
                *renderer.getGAME_ZOOM() = *renderer.getGAME_ZOOM()+1;
              }
              renderer.moveMap(-(TILE_WIDTH-*renderer.getGAME_ZOOM()*4)/2,(TILE_HEIGHT-*renderer.getGAME_ZOOM()*2)/2);
              break;
            default:
              break;
        }
        SDL_FlushEvents(sdl_main.getevent()->type,sdl_main.getevent()->type);
      }
    }
  }
  free(map_gen.getGameMap()->index);
  free(map_gen.getGameMap()->hdraw);
  free(map_gen.getGameMap()->height);
  free(map_gen.getGameMap());
  sdl_main.close();
  return 0;
}