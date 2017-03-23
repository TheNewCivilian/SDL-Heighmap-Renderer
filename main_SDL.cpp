/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL and standard IO

#include "bitmap.h"
#include "bittypes.h"
#include "Bmp_loader.h"
#include "main_SDL.h"




//Starts up SDL and creates window
bool init();
//Loads media
bool loadMedia();
 //Frees media and shuts down SDL
void close();

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 600;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;
//The image we will load and show on the screen
SDL_Surface* gHelloWorld = NULL;
//MyPixelmap
SDL_Surface *RGBpixelmap = (SDL_Surface*) malloc(sizeof(SDL_Surface));
SDL_Surface* gMapSurface = NULL;

Tilemap *tilemap;

Map *GameMap;


bool init() {
	//Initialization flag
	bool success = true;
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	} else {
		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL ) {
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		} else {
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface( gWindow );
			gMapSurface = SDL_GetWindowSurface( gWindow );
		}
	}
	return success;
}



bool loadMedia() {
	//Loading success flag
	bool success = true;
	long offset;
	int *w = (int *) malloc(sizeof(int));
	int *h  = (int *) malloc(sizeof(int));
	//Load splash image
	gHelloWorld = SDL_LoadBMP("hello_world.bmp");
	if( gHelloWorld == NULL ) {
		printf( "Unable to load image %s! SDL Error: %s\n", "hello_world.bmp", SDL_GetError() );
		success = false;
	}
	//Loading NEW_RGBPixelmap


  RGBpixelmap = load_bmp_to_sdl("lena.bmp");
	//int load_tile_to_sdl(SDL_Surface *surface,const char* filepath,int offset,int num)
	//offset = load_tile_to_sdl(RGBpixelmap,"Planes01.bmp",0,1,w,h);
	tilemap = load_tilemap("Planes01.bmp",0,1);
	printf("OFFSET: %i\n",offset);
	printf("loadMedia successfull!\n");

	return success;
}

bool loadMap(char *filepath,int *mapsize){
	int deb;
	int i;
	long index;
	bool success = true;
  bitmapRGB *input = (bitmapRGB *) malloc(sizeof(bitmapRGB));
	deb = loadBitmapRGB(filepath, input);
  if (deb != BMP_OK) {
      printf("Fehler beim Laden");
      return 0;
  }
	GameMap = (Map *) malloc(input->width*input->height*sizeof(Map));
	for(i = 0; i < input->width*input->height;i++){
		index = input->pixel[i].red*1000000+input->pixel[i].green*1000+input->pixel[i].blue;
	}
	return success;
}

//CSurface::OnDraw(Surf_Display, Surf_Test, 100, 100, 0, 0, 50, 50);
bool bindsurface(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, int X2, int Y2, int W, int H) {
    if(Surf_Dest == NULL || Surf_Src == NULL) {
        return false;
    }
    SDL_Rect DestR;
    DestR.x = X;
    DestR.y = Y;
		DestR.w = W;
    DestR.h = H;
    SDL_Rect SrcR;
    SrcR.x = X2;
    SrcR.y = Y2;
    SrcR.w = W;
    SrcR.h = H;
    SDL_BlitSurface(Surf_Src, &SrcR, Surf_Dest, &DestR);
    return true;
}



bool renderMap(){
		int i;
		int x = 0;
		int y = 0;
		for(i=0;i< tilemap->am;i++){
			bindsurface(gMapSurface, tilemap->tile[i]->surface, x, y, 0, 0, tilemap->tile[i]->w, tilemap->tile[i]->h);
			printf("Placed by: %i,%i\n",x,y);
			x += tilemap->tile[i]->w;
			if(x > 640){
				x = 0;
				y += 64;
			}
			if(y >600){
				printf("Screen warning!\n");
			}
		}
		printf("Rendermap Laoded!\n");
		return true;
}

void close() {
	//Deallocate surface
	SDL_FreeSurface( gHelloWorld );
	gHelloWorld = NULL;
	SDL_FreeSurface( gMapSurface );
	gMapSurface = NULL;
	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	//Quit SDL subsystems
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Initialize SDL
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
	 	} else {
		//Apply the image
		//SDL_BlitSurface( gHelloWorld, NULL, gScreenSurface, NULL );
		renderMap();
		//SDL_BlitSurface( RGBpixelmap, NULL, gScreenSurface, NULL );
		//SDL_BlitSurface(gMapSurface, NULL, gScreenSurface, NULL );
		// SDL_Rect DestR;
    // DestR.x = 100;
    // DestR.y = 100;
		// SDL_BlitSurface(tilemap->tile[0]->surface, NULL, gScreenSurface, &DestR);
		// SDL_BlitSurface(tilemap->tile[0]->surface, NULL, gScreenSurface, NULL);
		//Update the surface
		SDL_UpdateWindowSurface( gWindow );
		//Wait two seconds
		SDL_Delay( 2000 );
		}
	}
	//Free resources and close SDL
	close();
	return 0;
}
