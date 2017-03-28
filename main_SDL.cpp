#include "main_SDL.h"

bool init() {
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

bool loadMedia() {
	tilemap2 = load_tilemap_surface2texture("BaseMaptiles.bmp",render,0,1);
	printf("loadMedia successfull!\n");
	return true;
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

bool load_highMap(const char *filepath){
	int deb;
	int i,n;
	bool NNO, NOO, OSO, SOS, SSW, SWW, WNW, NWN;
	bool IN = false;
	const int BIT_01 = 53, BIT_02 = 111, BIT_03 = 732, BIT_04 = 173, BIT_05 = 1034, BIT_07 = 1735, BIT_06 = 236, BIT_08 = 2977,INBIT = 232323;
	// {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199 ...}
  bitmapRGB *input = (bitmapRGB *) malloc(sizeof(bitmapRGB));
	deb = loadBitmapRGB(filepath, input);
  if (deb != BMP_OK) {
      printf("Fehler beim Laden");
      return false;
  }
	GameMap2 = (Heightmap *) malloc(input->width*input->height*sizeof(Heightmap));
	GameMap2->height = (int *) malloc((input->width*input->height)*sizeof(int));
	GameMap2->index = (int *) malloc((input->width*input->height)*sizeof(int));
	GameMap2->hdraw = (int *) malloc((input->width*input->height)*sizeof(int));

	for(i = 0; i < input->width*input->height;i++){
		GameMap2->height[i] = input->pixel[i].red%64;
		GameMap2->hdraw[i] = input->pixel[i].red%64;
	}
	for(i = 0; i < input->width*input->height;i++){
		if (GameMap2->height[i] == 1){
			GameMap2->index[i] = 57;
		}else if( GameMap2->height[i] == 0 && ((i/input->width)*input->width == i || (i/input->width) == input->height-1 || (i/input->width) == 0 || (i/input->width)*input->width == i-1)){
			GameMap2->index[i] = 137;
		}else{
			NNO = GameMap2->height[i-input->width-1] != GameMap2->height[i-input->width];
			NOO = GameMap2->height[i-input->width] != GameMap2->height[i-input->width+1];
			OSO = GameMap2->height[i-input->width+1] != GameMap2->height[i+1];
			SOS = GameMap2->height[i+1] != GameMap2->height[i+input->width+1];
			SSW = GameMap2->height[i+input->width+1] != GameMap2->height[i+input->width];
			SWW = GameMap2->height[i+input->width] != GameMap2->height[i+input->width-1] ;
			WNW = GameMap2->height[i+input->width-1] != GameMap2->height[i-1];
			NWN = GameMap2->height[i-1] != GameMap2->height[i-input->width-1];
			IN = false;
			if(NNO || NOO || OSO || SOS || SSW || SWW || WNW || NWN){
				if(NNO == true){
					IN = GameMap2->height[i-input->width-1] < GameMap2->height[i-input->width];
				}else if(NOO == true){
					IN = GameMap2->height[i-input->width] < GameMap2->height[i-input->width+1];
				}else if(OSO == true){
					IN = GameMap2->height[i-input->width+1] < GameMap2->height[i+1];
				}else if(SOS == true){
					IN = GameMap2->height[i+1] < GameMap2->height[i+input->width+1];
				}else if(SSW == true){
					IN = GameMap2->height[i+input->width+1] < GameMap2->height[i+input->width];
				}else if(SWW == true){
					IN = GameMap2->height[i+input->width] < GameMap2->height[i+input->width-1];
				}else if(WNW == true){
					IN = GameMap2->height[i+input->width-1] < GameMap2->height[i-1];
				}else {
					IN = false;
				}
			}
			//if((NNO || NOO || OSO || SOS || SSW || SWW || WNW || NWN)&&GameMap2->height[i] == 0){
				switch((NNO?BIT_01:0)+(NOO?BIT_02:0)+(OSO?BIT_03:0)+(SOS?BIT_04:0)+(SSW?BIT_05:0)+(SWW?BIT_06:0)+(WNW?BIT_07:0)+(NWN?BIT_08:0)+(IN?INBIT:0)){
					case 0:
							GameMap2->index[i] = 137;
							break;
					case BIT_01+BIT_02+INBIT:
					case BIT_08+BIT_02:
					case BIT_01+BIT_03+INBIT:
					case BIT_08+BIT_03:
							GameMap2->index[i] = 137+6;
							GameMap2->hdraw[i]++;
							break;
					case BIT_02+BIT_03+INBIT:
							GameMap2->index[i] = 137+1;
							break;
					case BIT_03+BIT_04+INBIT:
					case BIT_02+BIT_04+INBIT:
					case BIT_03+BIT_05+INBIT:
					case BIT_02+BIT_05+INBIT:
							GameMap2->index[i] = 137+5;
							break;
					case BIT_04+BIT_05+INBIT:
							GameMap2->index[i] = 137+3;
							break;
					case BIT_05+BIT_06+INBIT:
					case BIT_04+BIT_06+INBIT:
					case BIT_05+BIT_07+INBIT:
					case BIT_04+BIT_07+INBIT:
							GameMap2->index[i] = 137+7;
							break;
					case BIT_06+BIT_07+INBIT:
							// KWKL
							GameMap2->index[i] = 137+2;
							break;
					case BIT_07+BIT_08+INBIT:
					case BIT_06+BIT_08+INBIT:
					case BIT_07+BIT_01:
					case BIT_06+BIT_01:
							GameMap2->index[i] = 137+8;
							GameMap2->hdraw[i]++;
							break;
					case BIT_08+BIT_01:
							GameMap2->index[i] = 137+4;
							GameMap2->hdraw[i]++;
							break;
					//Die Dreier Cases V
					case BIT_01+BIT_05+INBIT:
					case BIT_01+BIT_04+INBIT:
					case BIT_08+BIT_04:
					case BIT_08+BIT_05:
							//KLKL_
							GameMap2->index[i] = 57+14;
							GameMap2->hdraw[i]++;
							break;

					case BIT_03+BIT_06+INBIT:
					case BIT_07+BIT_03+INBIT:
					case BIT_02+BIT_06+INBIT:
							//L_KLK
							GameMap2->index[i] = 57+7;
							//GameMap2->hdraw[i]++;
							break;
					case BIT_05+BIT_08+INBIT:
					case BIT_04+BIT_08+INBIT:
					case BIT_04+BIT_01:
					case BIT_05+BIT_01:
					//KL_KL
							GameMap2->index[i] = 57+11;
							GameMap2->hdraw[i]++;
							break;
					case BIT_02+BIT_07+INBIT:
					case BIT_07+BIT_03:
					case BIT_07+BIT_02:
							//LK_LK
							GameMap2->index[i] = 57+13 ;
							GameMap2->hdraw[i]++;
							break;
					default:
							GameMap2->index[i] = 1;
							break;
				}
			//}
		}
		if (GameMap2->index[i] == 0 || GameMap2->index[i] == 1){
		printf("Hightset: %i \n",GameMap2->height[i]);
		printf("Interupt: %i %i %i %i %i %i %i %i = %i\n",NNO,NOO,OSO,SOS,SSW,SWW,WNW,NWN,(NNO?BIT_01:0)+(NOO?BIT_02:0)+(OSO?BIT_03:0)+(SOS?BIT_04:0)+(SSW?BIT_05:0)+(SWW?BIT_06:0)+(WNW?BIT_07:0)+(NWN?BIT_08:0));
		printf("MapLoad for %i Tile: %s %i %s\n",i,KRED,GameMap2->index[i],KNORMAL);
		}
	}
	GameMap2->width = input->width;
	GameMap2->mapsize = input->width*input->height;
	return true;
}

bool renderTileMaptoScreen(){
	int i;
	int x = 0;
	int y = 0;
	for(i=0;i< tilemap2->am;i++){
		SDL_Rect DestR;
		DestR.x = x;
		DestR.y = y;
		SDL_RenderCopy(render, tilemap2->tile[GameMap->index[i]]->texture, NULL, &DestR);
		x += tilemap2->tile[i]->w;
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
		int comp_w = SCREEN_WIDTH, comp_h = SCREEN_HEIGHT;
		int posx,posy;
		if(GAME_ZOOM < 0){
			comp_w-=GAME_ZOOM*4;
			comp_h-=GAME_ZOOM*2;
		}
		//SDL_SetRenderDrawBlendMode(render,SDL_BLENDMODE_ADD);
		for (i=0; i < *mapsize;i++){
				posy = i/ *w;
				posx = i -(i/ *w)* *w ;
				x = (posx - posy) * (TILE_WIDTH-GAME_ZOOM*4)/2+view_x;
				y = (posx + posy) * (TILE_HEIGHT-GAME_ZOOM*2)/2+view_y;
				if(x < SCREEN_WIDTH && y <SCREEN_HEIGHT && y > -comp_w && x > -comp_h){
					SDL_Rect DestR;
			    DestR.x = x;
			    DestR.y = y;
					DestR.h = tilemap2->tile[GameMap->index[i]]->h-GAME_ZOOM*2;
			    DestR.w = tilemap2->tile[GameMap->index[i]]->w-GAME_ZOOM*4;
					SDL_SetTextureBlendMode(tilemap2->tile[GameMap->index[i]]->texture,SDL_BLENDMODE_ADD);
					SDL_RenderCopy(render, tilemap2->tile[GameMap->index[i]]->texture, NULL, &DestR); // Copy the texture into render
					//printf("Tile: %i, placed @%i|%i \n",GameMap->index[i],x,y);
				}
		}
		return true;
}

bool renderMap2(int view_x, int view_y){
		int i;
		int x = 0;
		int y = 0;
		int comp_w = SCREEN_WIDTH, comp_h = SCREEN_HEIGHT;
		int posx,posy;
		if(GAME_ZOOM < 0){
			comp_w-=GAME_ZOOM*4;
			comp_h-=GAME_ZOOM*2;
		}
		//SDL_SetRenderDrawBlendMode(render,SDL_BLENDMODE_ADD);

		for (i=0; i < GameMap2->mapsize;i++){
			//printf("Debug %i\n",GameMap2->mapsize);
				posy = i/ GameMap2->width;
				posx = i -(i/ GameMap2->width)* GameMap2->width ;
				x = (posx - posy) * (TILE_WIDTH-GAME_ZOOM*4)/2+view_x;
				y = (posx + posy) * (TILE_HEIGHT-GAME_ZOOM*2)/2+view_y;
				//printf("Debug2 %i | %i | %i \n",GameMap2->mapsize,GameMap2->height[i],GameMap2->index[i]);
				if(x < SCREEN_WIDTH && y <SCREEN_HEIGHT && y > -comp_w && x > -comp_h){
					SDL_Rect DestR;
			    DestR.x = x;
			    DestR.y = y-(GameMap2->hdraw[i]*8);
					DestR.h = tilemap2->tile[GameMap2->index[i]]->h-GAME_ZOOM*2;
			    DestR.w = tilemap2->tile[GameMap2->index[i]]->w-GAME_ZOOM*4;
					SDL_SetTextureBlendMode(tilemap2->tile[GameMap2->index[i]]->texture,SDL_BLENDMODE_ADD);
					SDL_RenderCopy(render, tilemap2->tile[GameMap2->index[i]]->texture, NULL, &DestR); // Copy the texture into render
					//printf("Tile: %i, placed @%i|%i \n",GameMap2->index[i],x,y);
				}
		}
		return true;
}

bool moveMap(int x,int y){
	GAME_X += x;
	GAME_Y += y;
	SDL_RenderClear(render);
	renderMap2(GAME_X,GAME_Y);
	SDL_RenderPresent(render);
	return true;
}

void close() {
	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	SDL_DestroyRenderer(render);
	render = NULL;
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
			//loadMap("map.bmp",mapsize,w);
			load_highMap("highmap.bmp");
			renderMap2(0,0);
			SDL_RenderPresent(render);
			while(!Lexit){
				while( SDL_PollEvent( &e ) != 0 ) {
					//User requests quit
					if( e.type == SDL_QUIT ) {
						Lexit = true;
					}
					 //User presses a key
					else if( e.type == SDL_KEYDOWN ) {
							switch( e.key.keysym.sym ) {
								 	case SDLK_UP:
									  moveMap(0,25);
										//printf("UP pressed \n");
										break;
									case SDLK_DOWN:
										//printf("DOWN pressed \n");
										moveMap(0,-25);
										break;
									case SDLK_LEFT:
										//printf("LEFT pressed \n");
									 	moveMap(25,0);
										break;
									case SDLK_RIGHT:
										//printf("RIGHT pressed \n");
									 	moveMap(-25,0);
										break;
									case SDLK_ESCAPE:
										//printf("ESC pressed \n");
										Lexit = true;
										break;
									case SDLK_PLUS:
										//printf("RIGHT pressed \n");
										if (GAME_ZOOM > -32){
											GAME_ZOOM--;
										}
										moveMap((TILE_WIDTH-GAME_ZOOM*4)/2,-(TILE_HEIGHT-GAME_ZOOM*2)/2);
										break;
									case SDLK_MINUS:
										//printf("RIGHT pressed \n");
										if (GAME_ZOOM < 8){
											GAME_ZOOM++;
										}
										moveMap(-(TILE_WIDTH-GAME_ZOOM*4)/2,(TILE_HEIGHT-GAME_ZOOM*2)/2);
										break;
									default:
										break;
							}
							//SDL_Delay( 20);
							 // Show render on window
							//SDL_UpdateWindowSurface( gWindow );
						}
					}
				}

		}
	}
	//Free resources and close SDL
	close();
	return 0;
}
