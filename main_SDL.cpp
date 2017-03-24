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
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 1200;

const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 32;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;
//The image we will load and show on the screen
SDL_Surface* gHelloWorld = NULL;
//MyPixelmap
SDL_Surface *RGBpixelmap = (SDL_Surface*) malloc(sizeof(SDL_Surface));
SDL_Surface* gMapSurface = NULL;

//Event handler
SDL_Event e;

Tilemap *tilemap;

Map *GameMap;
//Gameposition
int GAME_X = 0;
int GAME_Y = 0;


bool init() {
	//Initialization flag
	bool success = true;
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	} else {
		//Create window
		gWindow = SDL_CreateWindow( "TileMapRenderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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
	tilemap = load_tilemap("BaseMaptiles.bmp",0,1);
	printf("OFFSET: %i\n",offset);
	printf("loadMedia successfull!\n");

	return success;
}

bool loadMap(const char *filepath,int *mapsize,int *with){
	int deb;
	int i;
  bitmapRGB *input = (bitmapRGB *) malloc(sizeof(bitmapRGB));
	deb = loadBitmapRGB(filepath, input);
  if (deb != BMP_OK) {
      printf("Fehler beim Laden");
      return 0;
  }
	GameMap = (Map *) malloc(input->width*input->height*sizeof(Map));
	GameMap->index = (int *) malloc((input->width*input->height)*sizeof(int));
	for(i = 0; i < input->width*input->height;i++){
		if(input->pixel[i].red*255*2+input->pixel[i].green*255+input->pixel[i].blue < 222){
			GameMap->index[i] = input->pixel[i].red*255*2+input->pixel[i].green*255+input->pixel[i].blue;
		}else{
		if(input->pixel[i].red/32<4){
			GameMap->index[i] = 137+input->pixel[i].red/32;
		}else{
			GameMap->index[i] = 138;
		}
		}
		//printf("Tile: %i is %i for r:%i ,g:%i ,b:%i \n",i,GameMap->index[i],input->pixel[i].red*255*2,input->pixel[i].green*255,input->pixel[i].blue);

	}
	*with = input->width;
	*mapsize = input->width*input->height;
	return true;
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
    //SDL_BlitSurface(Surf_Src, &SrcR, Surf_Dest, &DestR);
		SDL_SetSurfaceBlendMode(Surf_Src,SDL_BLENDMODE_ADD);
		SDL_BlitSurface(Surf_Src, NULL, Surf_Dest, &DestR);
		return true;
}

bool renderTileMaptoScreen(){
	int i;
	int x = 0;
	int y = 0;
	for(i=0;i< tilemap->am;i++){
		bindsurface(gMapSurface, tilemap->tile[i]->surface, x, y, 0, 0, tilemap->tile[i]->w, tilemap->tile[i]->h);
		//printf("Placed by: %i,%i\n",x,y);
		x += tilemap->tile[i]->w;
		if(x > SCREEN_WIDTH){
			x = 0;
			y += 64;
		}
		if(y > SCREEN_HEIGHT){
			printf("Screen warning!\n");
		}
	}
	printf("Rendermap Laoded!\n");
	return true;
}



bool renderMap(int view_x, int view_y){
		int i;
		int x = 0;
		int y = 0;
		int posx,posy;
		int *w = (int *) malloc(sizeof(int));
		int *mapsize = (int *) malloc(sizeof(int));

		loadMap("map.bmp",mapsize,w);
		for (i=0; i < *mapsize;i++){
				posy = i/ *w;
				posx = i -(i/ *w)* *w ;
				x = (posx - posy) * TILE_WIDTH/2+view_x;
				y = (posx + posy) * TILE_HEIGHT/2+view_y;
				if(x < SCREEN_WIDTH && y <SCREEN_HEIGHT && y > -32 && x > -64){
					bindsurface(gMapSurface, tilemap->tile[GameMap->index[i]]->surface, x, y, 0, 0, tilemap->tile[GameMap->index[i]]->w, tilemap->tile[GameMap->index[i]]->h);
				}
			//58 Standard Grass

		}
		return true;
}

bool moveMap(int x,int y){
	GAME_X += x;
	GAME_Y += y;
	SDL_FillRect(gMapSurface, NULL, 0x000000);
	renderMap(GAME_X,GAME_Y);
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
	bool Lexit = false;
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
			renderMap(0,0);
			while(!Lexit){
				SDL_UpdateWindowSurface( gWindow );
				//moveMap(300,100);

				while( SDL_PollEvent( &e ) != 0 ) {
					//User requests quit
					if( e.type == SDL_QUIT ) {
						Lexit = true;
					}
					 //User presses a key
					else if( e.type == SDL_KEYDOWN ) {
							switch( e.key.keysym.sym ) {
								 	case SDLK_UP:
									  moveMap(0,15);
										//printf("UP pressed \n");
										break;
									case SDLK_DOWN:
										//printf("DOWN pressed \n");
										moveMap(0,-15);
										break;
									case SDLK_LEFT:
										//printf("LEFT pressed \n");
									 	moveMap(15,0);
										break;
									case SDLK_RIGHT:
										//printf("RIGHT pressed \n");
									 	moveMap(-15,0);
										break;
									case SDLK_ESCAPE:
										//printf("ESC pressed \n");
										Lexit = true;
										break;
									default:
										break;
							}
							//SDL_Delay( 20);
							SDL_UpdateWindowSurface( gWindow );
						}
					}
				}

		}
	}
	//Free resources and close SDL
	close();
	return 0;
}
