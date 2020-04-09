#include "Bmp_loader.h"

void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	Uint32 *target_pixel = (Uint32 *) (surface->pixels + y * surface->pitch + x * sizeof(*target_pixel));
	*target_pixel = pixel;
}

long load_tile_to_sdl_surface(SDL_Surface *surface,const char* filepath,long offset,int num,int *with,int *heigt){
  //Testload
  SDL_Surface *RGBpixelmap = NULL;
  int i,j; //Laufvariable
  int x,y;
  int deb;
  long start = 0;
  int pw = 0;
  int ph = 0;
  bitmapRGB *input = (bitmapRGB *) malloc(sizeof(bitmapRGB));
  SDL_PixelFormat *fmt;

  deb = loadBitmapRGB(filepath, input);
  if (deb != BMP_OK) {
      printf("Fehler beim Laden");
      return 0;
  }
	for(j = 0; j< num;j++){
		for(i = offset;i<input->width*input->height;i++){
	    if(input->pixel[i].blue == 255 && input->pixel[i].red == 0 && input->pixel[i].green == 0&&
				input->pixel[i-1].blue == 255 && input->pixel[i-1].red == 255&& input->pixel[i-1].green == 255&&
				input->pixel[i-input->width].blue == 255 && input->pixel[i-input->width].red == 255&& input->pixel[i-input->width].green == 255&&
				!(input->pixel[i+input->width].blue == 255 && input->pixel[i+input->width].red == 255 && input->pixel[i+input->width].green == 255)&&
				!(input->pixel[i+input->width+1].blue == 255 && input->pixel[i+input->width+1].red == 255 && input->pixel[i+input->width+1].green == 255)){
	      start = i;
	      //printf("Start bei: [%i][%i] \n",start-(start/input->width)*input->width,start/input->width);
	      break;
	    }
	  }
		offset = start+1;
	}
  if(start == 0){
    printf("Kein Start gefunden \n");
    return 0;
  }
  //Breitenberechnung
  for(i = start;i<(start/input->width+1)*input->width;i++){
    if(input->pixel[i].blue == 255 && input->pixel[i].red == 255&& input->pixel[i].green == 255){
      pw = i-start-1;
      //printf("Breite: %i \n",pw);
      break;
    }
  }
  if(pw == 0){
    printf("Keine Breite gefunden \n");
    return 0;
  }
  //Hoehenberechnung
  for(i = start;i<input->width*input->height;i+=input->width){
    if(input->pixel[i].blue == 255 && input->pixel[i].red == 255&& input->pixel[i].green == 255){
      ph = (i-start)/input->width;
      //printf("Hoehe: %i \n",ph);
      break;
    }
  }
  if(ph == 0){
    printf("Keine Hoehe gefunden \n");
    return 0;
  }
  RGBpixelmap = SDL_CreateRGBSurface(0,pw,ph,32,0,0,0,0); //SDL_CreateRGBSurface(Uint32 flags,int    width,int    height,int    depth,Uint32 Rmask,Uint32 Gmask,Uint32 Bmask,Uint32 Amask)
  if( RGBpixelmap == NULL ) {
    printf( "Unable to load image %s! SDL Error: %s\n", "hello_world.bmp", SDL_GetError() );
    return 0;
  }
  fmt=RGBpixelmap->format;

  SDL_LockSurface(RGBpixelmap);
  for(i = 0; i<ph;i++){
    for(j = 0;j<pw;j++){
      y = i;
      x = j;
      if (input->pixel[start+i*input->width+j].blue == 255 && input->pixel[start+i*input->width+j].red == 0&& input->pixel[start+i*input->width+j].green == 0){
        set_pixel(RGBpixelmap, x, y,SDL_MapRGBA(fmt,0,0,0,0));

				//Muss Alpha sein
      }else{
        set_pixel(RGBpixelmap, x, y,  SDL_MapRGBA(fmt,input->pixel[start+i*input->width+j].red,input->pixel[start+i*input->width+j].green,input->pixel[start+i*input->width+j].blue,128));
        //printf("Pixel[%5i][%5i][%5i] r=%i b=%i g=%i\n",i,x,y,r,b,g);
      }
    }
  }
  SDL_UnlockSurface(RGBpixelmap);
  *surface = *RGBpixelmap;
	//printf("%i %i\n",pw,ph);
	*with = pw;
	*heigt = ph;
  if ((int)(start+2*pw)/input->width != (int)start/input->width){
    return ((start+(ph+10)*(int)input->width+pw)/input->width)*input->width;
  }
  return start+pw+1;
}


long load_tile_to_sdl_texture(SDL_Texture *texture,SDL_Renderer *renderer,const char* filepath,long offset,int num,int *with,int *heigt){
  //Testload
  int i,j; //Laufvariable
  int x,y;
  int deb;
  long start = 0;
	int pixel_offset;
  int pw = 0;
  int ph = 0;
  bitmapRGB *input = (bitmapRGB *) malloc(sizeof(bitmapRGB));
  SDL_PixelFormat *fmt;
  deb = loadBitmapRGB(filepath, input);
  if (deb != BMP_OK) {
      printf("Fehler beim Laden");
      return 0;
  }
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, input->width, input->height);
	int *pixels = (int *) malloc(4*(input->width*input->height)*sizeof(int));

	for(j = 0; j< num;j++){
		for(i = offset;i<input->width*input->height;i++){
	    if(input->pixel[i].blue == 255 && input->pixel[i].red == 0 && input->pixel[i].green == 0&&
				input->pixel[i-1].blue == 255 && input->pixel[i-1].red == 255&& input->pixel[i-1].green == 255&&
				input->pixel[i-input->width].blue == 255 && input->pixel[i-input->width].red == 255&& input->pixel[i-input->width].green == 255&&
				!(input->pixel[i+input->width].blue == 255 && input->pixel[i+input->width].red == 255 && input->pixel[i+input->width].green == 255)&&
				!(input->pixel[i+input->width+1].blue == 255 && input->pixel[i+input->width+1].red == 255 && input->pixel[i+input->width+1].green == 255)){
	      start = i;
	      //printf("Start bei: [%i][%i] \n",start-(start/input->width)*input->width,start/input->width);
	      break;
	    }
	  }
		offset = start+1;
	}
  if(start == 0){
    printf("Kein Start gefunden \n");
    return 0;
  }
  //Breitenberechnung
  for(i = start;i<(start/input->width+1)*input->width;i++){
    if(input->pixel[i].blue == 255 && input->pixel[i].red == 255&& input->pixel[i].green == 255){
      pw = i-start-1;
      //printf("Breite: %i \n",pw);
      break;
    }
  }
  if(pw == 0){
    printf("Keine Breite gefunden \n");
    return 0;
  }
  //Hoehenberechnung
  for(i = start;i<input->width*input->height;i+=input->width){
    if(input->pixel[i].blue == 255 && input->pixel[i].red == 255&& input->pixel[i].green == 255){
      ph = (i-start)/input->width;
      //printf("Hoehe: %i \n",ph);
      break;
    }
  }
  if(ph == 0){
    printf("Keine Hoehe gefunden \n");
    return 0;
  }
  for(i = 0; i<ph;i++){
    for(j = 0;j<pw;j++){
      y = i;
      x = j;
      if (input->pixel[start+i*input->width+j].blue == 255 && input->pixel[start+i*input->width+j].red == 0&& input->pixel[start+i*input->width+j].green == 0){
				pixel_offset = i*4; //Offset

				pixels[ pixel_offset + 0 ] = 0;        	// b
        pixels[ pixel_offset + 1 ] = 0;       	// g
        pixels[ pixel_offset + 2 ] = 0;       	// r
        pixels[ pixel_offset + 3 ] = 0;  			  // a
				//Muss Alpha sein
      }else{
				pixel_offset = i*4; //Offset

				pixels[ pixel_offset + 0 ] = input->pixel[start+i*input->width+j].red;        	// b
        pixels[ pixel_offset + 1 ] = input->pixel[start+i*input->width+j].green;       	// g
        pixels[ pixel_offset + 2 ] = input->pixel[start+i*input->width+j].blue;       	// r
        pixels[ pixel_offset + 3 ] = 255;  			  // a
        //printf("Pixel[%5i][%5i][%5i] r=%i b=%i g=%i\n",i,x,y,r,b,g);
      }
    }
  }
	SDL_UpdateTexture(texture,NULL,pixels,4*input->width);

	//printf("%i %i\n",pw,ph);
	*with = pw;
	*heigt = ph;
  if ((int)(start+2*pw)/input->width != (int)start/input->width){
    return ((start+(ph+10)*(int)input->width+pw)/input->width)*input->width;
  }
  return start+pw+1;
}

Tilemap *load_tilemap_2surface(const char* filepath,long offset,int num){
	Tilemap *tilemap = (Tilemap *) malloc(sizeof(Tilemap));
	tilemap->am = 0;
	SDL_Surface **surfaces = (SDL_Surface**) malloc(sizeof(SDL_Surface *));
	Tile **tile = (Tile **) malloc(sizeof(Tile *));
	int *with = (int *) malloc(sizeof(int));
	int *heigt = (int *) malloc(sizeof(int));
	long ret;
	while(1 == 1){
		//long load_tile_to_sdl(SDL_Surface *surface,const char* filepath,long offset,int num,int *with,int *heigt);
		surfaces[tilemap->am] = (SDL_Surface*) malloc(sizeof(SDL_Surface));
		ret = load_tile_to_sdl_surface(surfaces[tilemap->am], filepath, offset, num, with, heigt);
		offset = ret;
		if (ret != 0){
			tile = (Tile **) realloc(tile, (tilemap->am+1)*sizeof(Tile *));
			tile[tilemap->am] = (Tile *) malloc(sizeof(Tile));
			tile[tilemap->am]->surface = surfaces[tilemap->am];
			tile[tilemap->am]->w = *with;
			tile[tilemap->am]->h = *heigt;
			//printf("Gespeichert in: %i, Höhe: %i, Breite: %i\n",	tilemap->am,tile[tilemap->am]->h,tile[tilemap->am]->w);
			tilemap->am++;
			surfaces = (SDL_Surface**) realloc(surfaces,(tilemap->am+1)*sizeof(SDL_Surface *));
		}else{
			printf("Tilemap: %i\n",tilemap->am);
			tilemap->tile = tile;
			return tilemap;
		}
	}
}

Tilemap2 *load_tilemap_surface2texture(const char* filepath,SDL_Renderer *renderer,long offset,int num){
	Tilemap2 *tilemap = (Tilemap2 *) malloc(sizeof(Tilemap2));
	tilemap->am = 0;
	SDL_Texture **textures = (SDL_Texture**) malloc(sizeof(SDL_Texture *));
	Tile2 **tile = (Tile2 **) malloc(sizeof(Tile2 *));
	int *with = (int *) malloc(sizeof(int));
	int *heigt = (int *) malloc(sizeof(int));
	long ret;
	while(1 == 1){
		SDL_Surface *surface = (SDL_Surface*) malloc(sizeof(SDL_Surface));
		//long load_tile_to_sdl(SDL_Surface *surface,const char* filepath,long offset,int num,int *with,int *heigt);
		//textures[tilemap->am] = (SDL_Texture*) malloc(sizeof(SDL_Texture));
		ret = load_tile_to_sdl_surface(surface, filepath, offset, num, with, heigt);
		offset = ret;
		if (ret != 0){
			tile = (Tile2 **) realloc(tile, (tilemap->am+1)*sizeof(Tile2 *));
			tile[tilemap->am] = (Tile2 *) malloc(sizeof(Tile2));
			tile[tilemap->am]->texture =  SDL_CreateTextureFromSurface(renderer,surface);
			 SDL_FreeSurface(surface);
			tile[tilemap->am]->w = *with;
			tile[tilemap->am]->h = *heigt;
			//printf("Gespeichert in: %i, Höhe: %i, Breite: %i\n",	tilemap->am,tile[tilemap->am]->h,tile[tilemap->am]->w);
			tilemap->am++;
			textures = (SDL_Texture**) realloc(textures,(tilemap->am+1)*sizeof(SDL_Texture *));
		}else{
			printf("Tilemap: %i\n",tilemap->am);
			tilemap->tile = tile;
			return tilemap;
		}
	}
}


Tilemap2 *load_tilemap_2texture(const char* filepath,SDL_Renderer *renderer,long offset,int num){
	Tilemap2 *tilemap = (Tilemap2 *) malloc(sizeof(Tilemap2));
	tilemap->am = 0;
	SDL_Texture **textures = (SDL_Texture**) malloc(sizeof(SDL_Texture *));
	Tile2 **tile = (Tile2 **) malloc(sizeof(Tile2 *));
	int *with = (int *) malloc(sizeof(int));
	int *heigt = (int *) malloc(sizeof(int));
	long ret;
	while(1 == 1){
		//long load_tile_to_sdl(SDL_Surface *surface,const char* filepath,long offset,int num,int *with,int *heigt);
		//textures[tilemap->am] = (SDL_Texture*) malloc(sizeof(SDL_Texture));

		ret = load_tile_to_sdl_texture(textures[tilemap->am],renderer, filepath, offset, num, with, heigt);
		offset = ret;
		if (ret != 0){
			tile = (Tile2 **) realloc(tile, (tilemap->am+1)*sizeof(Tile2 *));
			tile[tilemap->am] = (Tile2 *) malloc(sizeof(Tile2));
			tile[tilemap->am]->texture = textures[tilemap->am];
			tile[tilemap->am]->w = *with;
			tile[tilemap->am]->h = *heigt;
			//printf("Gespeichert in: %i, Höhe: %i, Breite: %i\n",	tilemap->am,tile[tilemap->am]->h,tile[tilemap->am]->w);
			tilemap->am++;
			textures = (SDL_Texture**) realloc(textures,(tilemap->am+1)*sizeof(SDL_Texture *));
		}else{
			printf("Tilemap: %i\n",tilemap->am);
			tilemap->tile = tile;
			return tilemap;
		}
	}
}
