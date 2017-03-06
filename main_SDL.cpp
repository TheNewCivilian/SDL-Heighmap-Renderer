/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include "bitmap.h"
#include "bittypes.h"
#include "Bmp_loader.h"




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
SDL_Surface *RGBpixelmap = NULL;



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
		}
	}
	return success;
}



bool loadMedia() {
	//Loading success flag
	bool success = true;
	int offset;
	//Load splash image
	gHelloWorld = SDL_LoadBMP("hello_world.bmp");
	if( gHelloWorld == NULL ) {
		printf( "Unable to load image %s! SDL Error: %s\n", "hello_world.bmp", SDL_GetError() );
		success = false;
	}
	//Loading NEW_RGBPixelmap


  RGBpixelmap = load_bmp_to_sdl("lena.bmp");
	//int load_tile_to_sdl(const char* filepath,int offset,SDL_Surface *surface)
	offset = load_tile_to_sdl("Planes01.bmp",0,RGBpixelmap);
	printf("OFFSET: %i\n",offset);

	printf("loadMedia successfull!\n");

	return success;
}

void close() {
	//Deallocate surface
	SDL_FreeSurface( gHelloWorld ); gHelloWorld = NULL;
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
		SDL_BlitSurface( RGBpixelmap, NULL, gScreenSurface, NULL );
		//Update the surface
		SDL_UpdateWindowSurface( gWindow );
		//Wait two seconds
		SDL_Delay( 20000 );
		}
	}
	//Free resources and close SDL
	close();
	return 0;
}
