#include "Bmp_loader.h"
#include "bitmap.h"
#include "bittypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	Uint32 *target_pixel = (Uint32 *) (surface->pixels + y * surface->pitch + x * sizeof(*target_pixel));
	//printf("Size: %i",sizeof(*target_pixel));
	*target_pixel = pixel;
}

SDL_Surface *load_bmp_to_sdl(const char* filepath){
  //Testload
  SDL_Surface *RGBpixelmap = NULL;
  int i; //Laufvariable
  int x,y;
  int deb;
  Uint8 r = 20,g = 20,b = 10;
  bitmapRGB *input = (bitmapRGB *) malloc(sizeof(bitmapRGB));
  SDL_PixelFormat *fmt;

  deb = loadBitmapRGB(filepath, input);
  if (deb != BMP_OK) {
      printf("Fehler beim Laden");
      return NULL;
  }
  RGBpixelmap = SDL_CreateRGBSurface(0,input->width,input->height,32,0,0,0,0); //SDL_CreateRGBSurface(Uint32 flags,int    width,int    height,int    depth,Uint32 Rmask,Uint32 Gmask,Uint32 Bmask,Uint32 Amask)
  if( RGBpixelmap == NULL ) {
    printf( "Unable to load image %s! SDL Error: %s\n", "hello_world.bmp", SDL_GetError() );
    return NULL;
  }
  fmt=RGBpixelmap->format;
  SDL_LockSurface(RGBpixelmap);
  for(i = 0; i<input->width*input->height;i++){
    r = input->pixel[i].red;
    g = input->pixel[i].green;
    b = input->pixel[i].blue;
    y = i/input->width;
    x = i-((y)*input->width);
    set_pixel(RGBpixelmap, x, y,  SDL_MapRGB(fmt,r,g,b));
    //printf("Pixel[%5i][%5i][%5i] r=%i b=%i g=%i\n",i,x,y,r,b,g);
  }
  SDL_UnlockSurface(RGBpixelmap);
  return RGBpixelmap;

}

int load_tile_to_sdl(const char* filepath,int offset,SDL_Surface *surface){
  //Testload
  SDL_Surface *RGBpixelmap = NULL;
  int i,j; //Laufvariable
  int x,y;
  int deb;
  int start = 0;
  int pw = 0;
  int ph = 0;
  bitmapRGB *input = (bitmapRGB *) malloc(sizeof(bitmapRGB));
  SDL_PixelFormat *fmt;

  deb = loadBitmapRGB(filepath, input);
  if (deb != BMP_OK) {
      printf("Fehler beim Laden");
      return 0;
  }
  for(i = offset;i<input->width*input->height;i++){
    if(input->pixel[i].blue == 255 && input->pixel[i].red == 0 && input->pixel[i].green == 0){
      start = i;
      printf("Start bei: [%i][%i] \n",start-(start/input->width)*input->width,start/input->width);
      break;
    }
  }
  if(start == 0){
    printf("Kein Start gefunden \n");
    return 0;
  }
  //Breitenberechnung
  for(i = start;i<(start/input->width+1)*input->width;i++){
    if(input->pixel[i].blue == 255 && input->pixel[i].red == 255&& input->pixel[i].green == 255){
      pw = i-start-1;
      printf("Breite: %i \n",pw);
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
      printf("Hoehe: %i \n",ph);
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
        set_pixel(RGBpixelmap, x, y,SDL_MapRGBA(fmt,0,0,0,255));
      }else{
        set_pixel(RGBpixelmap, x, y,  SDL_MapRGB(fmt,input->pixel[start+i*input->width+j].red,input->pixel[start+i*input->width+j].green,input->pixel[start+i*input->width+j].blue));
        //printf("Pixel[%5i][%5i][%5i] r=%i b=%i g=%i\n",i,x,y,r,b,g);
      }
    }
  }
  SDL_UnlockSurface(RGBpixelmap);
  *surface = *RGBpixelmap;
  if ((start+2*pw)/input->width != start/input->width){
    return ((start+(ph+10)*input->width+pw)/input->width)*input->width;
  }
  return start+pw+1;
}
