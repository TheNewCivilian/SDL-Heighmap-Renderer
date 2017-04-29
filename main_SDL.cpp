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
	GameMap = (Map *) malloc(sizeof(Map));
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
	}
	*with = input->width;
	*mapsize = input->width*input->height;
	return true;
}

bool checkmap(int hight){
	long i;
	int progress = 0;
	int oprogress = -1;
	long pixels = GameMap2->mheight*GameMap2->width;
	bool N = false, NO = false, O = false, SO = false, S = false, SW = false, W = false, NW = false;
	const int BIT_01 = 53, BIT_02 = 111, BIT_03 = 732, BIT_04 = 173, BIT_05 = 1034, BIT_07 = 1735, BIT_06 = 236, BIT_08 = 2977;
	printf("Begin\n");
	if (hight == -1){
		return true;
	}else{
		for(i = 0; i < GameMap2->mapsize;i++){
			if(GameMap2->height[i] == hight){
				//Soroundcheck auf größere Höhenunterschiede
				if ((i/GameMap2->width) != 0 && (i/GameMap2->width)*GameMap2->width != i && !(hight == GameMap2->height[i-GameMap2->width-1] || hight == GameMap2->height[i-GameMap2->width-1]-1 || hight == GameMap2->height[i-GameMap2->width-1]+1)){
					GameMap2->height[i-GameMap2->width-1] = hight-1;
					i-=GameMap2->width+1;
					continue;
				}
				if ((i/GameMap2->width) != 0 && !(hight == GameMap2->height[i-GameMap2->width] || hight ==GameMap2->height[i-GameMap2->width]-1 || hight == GameMap2->height[i-GameMap2->width]+1)){
					GameMap2->height[i-GameMap2->width]= hight-1;
					i-=GameMap2->width+1;
					continue;
				}
				if ((i/GameMap2->width) != 0 && (((i+1)/GameMap2->width)*GameMap2->width) != i+1 && !(hight == GameMap2->height[i-GameMap2->width+1] || hight ==GameMap2->height[i-GameMap2->width+1]-1 || hight == GameMap2->height[i-GameMap2->width+1]+1)){
					GameMap2->height[i-GameMap2->width+1]= hight-1;
					i-=GameMap2->width+1;
					continue;
				}
				if ((((i+1)/GameMap2->width)*GameMap2->width) != i+1 && !(hight == GameMap2->height[i+1] || GameMap2->height[i] ==GameMap2->height[i+1]-1 || GameMap2->height[i] == GameMap2->height[i+1]+1)){
					GameMap2->height[i+1]= hight-1;
					i-=GameMap2->width+1;
					continue;
				}
				if ((((i+1)/GameMap2->width)*GameMap2->width) != i+1 && (i/GameMap2->width) != GameMap2->mheight-1 && !(hight == GameMap2->height[i+GameMap2->width+1] || hight ==GameMap2->height[i+GameMap2->width+1]-1 || hight == GameMap2->height[i+GameMap2->width+1]+1)){
					GameMap2->height[i+GameMap2->width+1]= hight-1;
					i-=GameMap2->width+1;
					continue;
				}
				if ((i/GameMap2->width) != GameMap2->mheight-1 && !(hight == GameMap2->height[i+GameMap2->width] || hight ==GameMap2->height[i+GameMap2->width]-1 || hight == GameMap2->height[i+GameMap2->width]+1)){
					GameMap2->height[i+GameMap2->width]= hight-1;
					i-=GameMap2->width+1;
					continue;
				}
				if ((i/GameMap2->width) != GameMap2->mheight-1 && (i/GameMap2->width)*GameMap2->width != i && !(hight == GameMap2->height[i+GameMap2->width-1] || hight ==GameMap2->height[i+GameMap2->width-1]-1 || hight == GameMap2->height[i+GameMap2->width-1]+1)){
					GameMap2->height[i+GameMap2->width-1]= hight-1;
					i-=GameMap2->width+1;
					continue;
				}
				if ((i/GameMap2->width)*GameMap2->width != i && !(hight == GameMap2->height[i-1] || hight ==GameMap2->height[i-1]-1 || GameMap2->height[i] == GameMap2->height[i-1]+1)){
					GameMap2->height[i-1]= hight-1;
					i-=GameMap2->width+1;
					continue;
				}
			}
			N = false, NO = false, O = false, SO = false, S = false, SW = false, W = false, NW = false;
			//Umgebungsscann Teil2
			if ((i/GameMap2->width) != 0 && (i/GameMap2->width)*GameMap2->width != i){
				N = GameMap2->height[i]<GameMap2->height[i-GameMap2->width-1];
			}
			if ((i/GameMap2->width) != 0){
				NO = GameMap2->height[i]<GameMap2->height[i-GameMap2->width];
			}
			if ((i/GameMap2->width) != 0 && (((i+1)/GameMap2->width)*GameMap2->width) != i+1){
				O = GameMap2->height[i]<GameMap2->height[i-GameMap2->width+1];
			}
			if ((((i+1)/GameMap2->width)*GameMap2->width) != i+1 ){
				SO = GameMap2->height[i]<GameMap2->height[i+1];
			}
			if((((i+1)/GameMap2->width)*GameMap2->width) != i+1 && (i/GameMap2->width) != GameMap2->mheight-1){
				S = GameMap2->height[i]<GameMap2->height[i+GameMap2->width+1];
			}
			if ((i/GameMap2->width) != GameMap2->mheight-1 && (i/GameMap2->width)*GameMap2->width != i){
				SW = GameMap2->height[i]<GameMap2->height[i+GameMap2->width];
			}
			if ((i/GameMap2->width)*GameMap2->width != i&& (i/GameMap2->width) != GameMap2->mheight-1){
				W = GameMap2->height[i]<GameMap2->height[i+GameMap2->width-1];
			}
			if ((i/GameMap2->width) != 0&& (i/GameMap2->width)*GameMap2->width != i){
				NW =  GameMap2->height[i]<GameMap2->height[i-1];
			}
			//Allgemeine Drawhöhenanpassung
			GameMap2->hdraw[i] = GameMap2->height[i];
			//Indexzuteilung
			switch((N?BIT_01:0)+(NO?BIT_02:0)+(O?BIT_03:0)+(SO?BIT_04:0)+(S?BIT_05:0)+(SW?BIT_06:0)+(W?BIT_07:0)+(NW?BIT_08:0)){
					case 0:
							if(GameMap2->height[i] == 0){
								GameMap2->index[i] = 137;
							}else{
								GameMap2->index[i] = 57;
							}
							break;

					case BIT_07+BIT_08+BIT_01:
					case BIT_08+BIT_07:
					case BIT_01+BIT_08:
					case BIT_08:
							if(GameMap2->height[i] == 0){
									GameMap2->index[i] = 137+8;
							}else{
									GameMap2->index[i] = 57+9;
							}
							GameMap2->hdraw[i]++;
							break;

					case BIT_02+BIT_03+BIT_01:
					case BIT_01+BIT_03:
					case BIT_03+BIT_02:
					case BIT_01+BIT_02:
					case BIT_02:
							if(GameMap2->height[i] == 0){
								GameMap2->index[i] = 137+6;
							}else{
								GameMap2->index[i] = 57+12;
							}
							GameMap2->hdraw[i]++;
							break;

					case BIT_04+BIT_03+BIT_05:
					case BIT_04+BIT_03:
					case BIT_04+BIT_05:
					case BIT_04:
							if(GameMap2->height[i] == 0){
								GameMap2->index[i] = 137+5;
							}else{
								GameMap2->index[i] = 57+6;
							}
							break;

					case BIT_07+BIT_06+BIT_05:
					case BIT_06+BIT_05:
					case BIT_06+BIT_07:
					case BIT_06:
							if(GameMap2->height[i] == 0){
								GameMap2->index[i] = 137+7;
							}else{
								GameMap2->index[i] = 57+3;
							}
							break;

					case BIT_01:
							if(GameMap2->height[i] == 0){
								GameMap2->index[i] = 137+4;
							}else{
								GameMap2->index[i] = 57+8;
							}
							GameMap2->hdraw[i]++;
							break;

					case BIT_07:
							// KWKL
							if(GameMap2->height[i] == 0){
								GameMap2->index[i] = 137+2;
							}else{
								GameMap2->index[i] = 57+1;
							}
							break;

					case BIT_03:
							if(GameMap2->height[i] == 0){
								GameMap2->index[i] = 137+1;
							}else{
								GameMap2->index[i] = 57+4;
							}
							break;

					case BIT_05:
							if(GameMap2->height[i] == 0){
								GameMap2->index[i] = 137+3;
							}else{
								GameMap2->index[i] = 57+2;
							}
							break;

					//Die Dreier Kantenzuteilung

					case BIT_02+BIT_03+BIT_04:
					case BIT_02+BIT_03+BIT_04+BIT_05:
					case BIT_01+BIT_02+BIT_04:
					case BIT_01+BIT_02+BIT_03+BIT_04:
					case BIT_01+BIT_02+BIT_03+BIT_04+BIT_05:
					case BIT_02+BIT_05:
							//KLKL_
							GameMap2->index[i] = 57+14;
							GameMap2->hdraw[i]++;
							break;

					case BIT_04+BIT_05+BIT_06:
					case BIT_04+BIT_07:
					case BIT_04+BIT_05+BIT_06+BIT_03:
					case BIT_04+BIT_05+BIT_06+BIT_03+BIT_07:
					case BIT_04+BIT_05+BIT_06+BIT_07:
							//L_KLK
							GameMap2->index[i] = 57+7;
							break;

					case BIT_08+BIT_07+BIT_06:
					case BIT_08+BIT_07+BIT_06+BIT_05:
					case BIT_01+BIT_05+BIT_06:
					case BIT_08+BIT_06:
							//KL_KL
							GameMap2->index[i] = 57+11;
							GameMap2->hdraw[i]++;
							break;

					case BIT_01+BIT_08+BIT_02:
					case BIT_02+BIT_08:
					case BIT_08+BIT_01+BIT_02+BIT_03:
					case BIT_07+BIT_08+BIT_01+BIT_02+BIT_03:
					case BIT_07+BIT_08+BIT_03:
							//LK_LK
							GameMap2->index[i] = 57+13 ;
							GameMap2->hdraw[i]++;
							break;

					case BIT_02+BIT_03+BIT_06+BIT_07:
					case BIT_03+BIT_07:
							if(GameMap2->height[i] == 0){
								GameMap2->index[i] = 0;
								GameMap2->height[i]++;
								i-=GameMap2->width+1;
							}
							GameMap2->index[i] = 57+5;
							break;

					case BIT_04+BIT_05+BIT_08+BIT_01:
					case BIT_01+BIT_05:
							if(GameMap2->height[i] == 0){
								GameMap2->index[i] = 0;
								GameMap2->height[i]++;
								i-=GameMap2->width+1;
							}
							GameMap2->index[i] = 57+10;
							break;

					default:
							GameMap2->index[i] = 0;
							GameMap2->height[i]++;
							i-=GameMap2->width+1;
							continue;
							break;
					}
					progress = (i*100)/pixels;
					if (oprogress < progress){
						printf("\rpreparing Highmap [%s%3i%s%]\n",KGREEN,progress,KNORMAL);
						oprogress = progress;
					}

					if (GameMap2->index[i] == 0 || GameMap2->index[i] == 1){
							printf("Hightset Fehler: %i \n",GameMap2->height[i]);
							printf("Interupt: %i %i %i %i %i %i %i %i = %i\n",N,NO,O,SO,S,SW,W,NW,(N?BIT_01:0)+(NO?BIT_02:0)+(O?BIT_03:0)+(SO?BIT_04:0)+(S?BIT_05:0)+(SW?BIT_06:0)+(W?BIT_07:0)+(NW?BIT_08:0));
							printf("MapLoad for %i Tile: %s %i %s\n",i,KRED,GameMap2->index[i],KNORMAL);
					}
				}
			}
	checkmap(hight-1);
	return true;
}

bool printMap2Console(){
	int i;
	for(i = 0; i < GameMap2->mapsize;i++){
		if (GameMap2->index[i] == 0){
				printf("%s%i%s ",KYELLOW,GameMap2->height[i],KNORMAL);
		}else {
				printf("%i ",GameMap2->height[i]);
		}

		if(i == (i/GameMap2->width)*GameMap2->width){
			printf("\n");
		}
	}
	return true;
}

bool printHDraw2Console(){
	int i;
	for(i = 0; i < GameMap2->mapsize;i++){
		printf("%i ",GameMap2->height[i]);
		if(i == (i/GameMap2->width)*GameMap2->width){
			printf("\n");
		}
	}
	return true;
}

bool load_highMap(const char *filepath){
	int deb;
	int i;
	int prez = 32;
	bitmapRGB *input = (bitmapRGB *) malloc(sizeof(bitmapRGB));
	deb = loadBitmapRGB(filepath, input);
  if (deb != BMP_OK) {
      printf("Fehler beim Laden");
      return false;
  }
	GameMap2 = (Heightmap *) malloc(sizeof(Heightmap));
	GameMap2->height = (int *) malloc((input->width*input->height)*sizeof(int));
	GameMap2->index = (int *) malloc((input->width*input->height)*sizeof(int));
	GameMap2->hdraw = (int *) malloc((input->width*input->height)*sizeof(int));
	GameMap2->width = input->width;
	GameMap2->mapsize = input->width*input->height;
	GameMap2->mheight = input->height;

	for(i = 0; i < input->width*input->height;i++){
		GameMap2->height[i] = input->pixel[i].red%(prez-1);
	}

	checkmap(255%(prez-1));
	//Map Debuging!
	//printMap2Console();
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

		for (i=0; i < GameMap2->mapsize;i++){
				posy = i/ GameMap2->width;
				posx = i -(i/ GameMap2->width)* GameMap2->width ;
				x = (posx - posy) * (TILE_WIDTH-GAME_ZOOM*4)/2+view_x;
				y = (posx + posy) * (TILE_HEIGHT-GAME_ZOOM*2)/2+view_y-(GameMap2->hdraw[i]*8);
				if(x < SCREEN_WIDTH && y <SCREEN_HEIGHT && y > -comp_w && x > -comp_h){
					SDL_Rect DestR;
			    DestR.x = x;
			    DestR.y = y;
					DestR.h = tilemap2->tile[GameMap2->index[i]]->h-GAME_ZOOM*2;
			    DestR.w = tilemap2->tile[GameMap2->index[i]]->w-GAME_ZOOM*4;
					SDL_SetTextureBlendMode(tilemap2->tile[GameMap2->index[i]]->texture,SDL_BLENDMODE_ADD);
					SDL_RenderCopy(render, tilemap2->tile[GameMap2->index[i]]->texture, NULL, &DestR); // Copy the texture into render
				}
		}
		return true;
}

bool giveInfo(int search_x,int search_y){
	int i;
	int x = 0;
	int y = 0;
	int comp_w = SCREEN_WIDTH, comp_h = SCREEN_HEIGHT;
	int posx,posy;
	if(GAME_ZOOM < 0){
		comp_w-=GAME_ZOOM*4;
		comp_h-=GAME_ZOOM*2;
	}
	for (i=0; i < GameMap2->mapsize;i++){
			posy = i/ GameMap2->width;
			posx = i -(i/ GameMap2->width)* GameMap2->width ;
			x = (posx - posy) * (TILE_WIDTH-GAME_ZOOM*4)/2+GAME_X;
			y = (posx + posy) * (TILE_HEIGHT-GAME_ZOOM*2)/2+GAME_Y-(GameMap2->hdraw[i]*8);
			if(x < SCREEN_WIDTH && y < SCREEN_HEIGHT && y > -comp_w && x > -comp_h){
				if ( x - TILE_WIDTH/2  <= (search_x/TILE_WIDTH)*TILE_WIDTH  && (search_x/TILE_WIDTH)*TILE_WIDTH <= x- TILE_WIDTH/2 + TILE_WIDTH && y- TILE_HEIGHT/2 <= (search_y/TILE_HEIGHT)*TILE_HEIGHT && search_y <= (search_y/TILE_HEIGHT)*TILE_HEIGHT- TILE_HEIGHT/2+TILE_HEIGHT){
					if (i < GameMap2->mapsize){
						printf("Overflow");
					}
					printf("Tile %i @%i,%i a %i on %s %i %s \n",i,posx,posy,GameMap2->index[i],KRED,GameMap2->height[i],KNORMAL);
				}
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
			load_highMap("Megatest.bmp");
			SDL_RenderClear(render);
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
										SDL_FlushEvents(e.type,e.type);
										//printf("UP pressed \n");
										break;
									case SDLK_DOWN:
										//printf("DOWN pressed \n");
										moveMap(0,-25);
										SDL_FlushEvents(e.type,e.type);
										break;
									case SDLK_LEFT:
										//printf("LEFT pressed \n");
									 	moveMap(25,0);
										SDL_FlushEvents(e.type,e.type);
										break;
									case SDLK_RIGHT:
										//printf("RIGHT pressed \n");
									 	moveMap(-25,0);
										SDL_FlushEvents(e.type,e.type);
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
										SDL_FlushEvents(e.type,e.type);
										break;
									case SDLK_MINUS:
										//printf("RIGHT pressed \n");
										if (GAME_ZOOM < 10){
											GAME_ZOOM++;
										}
										moveMap(-(TILE_WIDTH-GAME_ZOOM*4)/2,(TILE_HEIGHT-GAME_ZOOM*2)/2);
										SDL_FlushEvents(e.type,e.type);
										break;
									default:
										break;
							}
							//SDL_Delay( 20);
							 // Show render on window
							//SDL_UpdateWindowSurface( gWindow );
						}else if (e.type == SDL_MOUSEBUTTONDOWN){
							if (e.button.button == SDL_BUTTON_LEFT){
								//printf("")
								//giveInfo(e.button.x,e.button.y);

							}
						}
					}
				}

		}
	}
	//Free resources and close SDL
	close();
	return 0;
}
