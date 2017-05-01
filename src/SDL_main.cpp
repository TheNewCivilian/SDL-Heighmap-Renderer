#include "SDL_main.h"



bool SDL_MAIN::init() {
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return false;
	} else {
		//Create window
		gWindow = SDL_CreateWindow( "TileMapRenderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL ) {
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			return false;
		} else {
			render = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	    if (render == NULL) {
	        printf( "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
	        return false;
	    }
		}
	}
	return true;
}

bool SDL_MAIN::loadMedia() {
	base_tilemap = load_tilemap_surface2texture("../assets/BaseMaptiles.bmp",render,0,1);
	printf("loadMedia successfull!\n");
	return true;
}

void SDL_MAIN::close() {
	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	SDL_DestroyRenderer(render);
	render = NULL;
	//Quit SDL subsystems
	SDL_Quit();
}

SDL_Renderer *SDL_MAIN::getrenderer(){
	return render;
}

SDL_Event *SDL_MAIN::getevent(){
	return &e;
}

Tilemap2 *SDL_MAIN::getbasetilemap(){
	return base_tilemap;
}


int main( int argc, char* args[] )
{
	SDL_MAIN sdl_main;
	MAP_GEN map_gen;
	RENDERER renderer;
	bool Lexit = false;
	//Initialize SDL
	if( !sdl_main.init() )
	{
		printf( "Failed to initialize!\n" );
		return 1;
	}
	else
	{
		//Load media
		if( !sdl_main.loadMedia() )
		{
			printf( "Failed to load media!\n" );
			return 1;
	 	} else {


			//loadMap("map.bmp",mapsize,w);
			renderer.setrenderer(sdl_main.getrenderer());
			map_gen.load_highMap("../assets/Megatest.bmp");
			renderer.setmapgen(&map_gen);
			renderer.setbase_tilemap(sdl_main.getbasetilemap());
			SDL_RenderClear(sdl_main.getrenderer());
			renderer.renderMap(0,0);
			SDL_RenderPresent(sdl_main.getrenderer());
			while(!Lexit){
				while( SDL_PollEvent( sdl_main.getevent() ) != 0 ) {
					//User requests quit
					if( sdl_main.getevent()->type == SDL_QUIT ) {
						Lexit = true;
					}
					 //User presses a key
					else if( sdl_main.getevent()->type == SDL_KEYDOWN ) {
							switch( sdl_main.getevent()->key.keysym.sym ) {
								 	case SDLK_UP:
									  renderer.moveMap(0,25);
										SDL_FlushEvents(sdl_main.getevent()->type,sdl_main.getevent()->type);
										//printf("UP pressed \n");
										break;
									case SDLK_DOWN:
										//printf("DOWN pressed \n");
										renderer.moveMap(0,-25);
										SDL_FlushEvents(sdl_main.getevent()->type,sdl_main.getevent()->type);
										break;
									case SDLK_LEFT:
										//printf("LEFT pressed \n");
									 	renderer.moveMap(25,0);
										SDL_FlushEvents(sdl_main.getevent()->type,sdl_main.getevent()->type);
										break;
									case SDLK_RIGHT:
										//printf("RIGHT pressed \n");
									 	renderer.moveMap(-25,0);
										SDL_FlushEvents(sdl_main.getevent()->type,sdl_main.getevent()->type);
										break;
									case SDLK_ESCAPE:
										//printf("ESC pressed \n");
										Lexit = true;
										break;
									case SDLK_PLUS:
										//printf("RIGHT pressed \n");
										if (*renderer.getGAME_ZOOM() > -32){
											*renderer.getGAME_ZOOM() = *renderer.getGAME_ZOOM()-1;
										}
										renderer.moveMap((TILE_WIDTH-*renderer.getGAME_ZOOM()*4)/2,-(TILE_HEIGHT-*renderer.getGAME_ZOOM()*2)/2);
										SDL_FlushEvents(sdl_main.getevent()->type,sdl_main.getevent()->type);
										break;
									case SDLK_MINUS:
										//printf("RIGHT pressed \n");
										if (*renderer.getGAME_ZOOM() < 10){
											*renderer.getGAME_ZOOM() = *renderer.getGAME_ZOOM()+1;
										}
										renderer.moveMap(-(TILE_WIDTH-*renderer.getGAME_ZOOM()*4)/2,(TILE_HEIGHT-*renderer.getGAME_ZOOM()*2)/2);
										SDL_FlushEvents(sdl_main.getevent()->type,sdl_main.getevent()->type);
										break;
									default:
										break;
							}
							//SDL_Delay( 20);
							 // Show render on window
							//SDL_UpdateWindowSurface( gWindow );
						}else if (sdl_main.getevent()->type == SDL_MOUSEBUTTONDOWN){
							if (sdl_main.getevent()->button.button == SDL_BUTTON_LEFT){
								//printf("")
								//giveInfo(e.button.x,e.button.y);

							}
						}
					}
				}

		}
	}
	//Free resources and close SDL
  free(map_gen.getGameMap()->index);
	free(map_gen.getGameMap()->hdraw);
	free(map_gen.getGameMap()->height);
	free(map_gen.getGameMap());
	sdl_main.close();
	return 0;
}
