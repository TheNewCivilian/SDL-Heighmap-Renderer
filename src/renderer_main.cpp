#include "renderer_main.h"


  bool RENDERER::moveMap(int x,int y){
  	GAME_X += x;
  	GAME_Y += y;
  	SDL_RenderClear(render);
  	renderMap(GAME_X,GAME_Y);
  	SDL_RenderPresent(render);
  	return true;
  }

  bool RENDERER::renderMap(int view_x, int view_y){
  		int i;
  		int x = 0;
  		int y = 0;
  		int comp_w = SCREEN_WIDTH, comp_h = SCREEN_HEIGHT;
  		int posx,posy;
  		if(GAME_ZOOM < 0){
  			comp_w-=GAME_ZOOM*4;
  			comp_h-=GAME_ZOOM*2;
  		}
      Heightmap *GameMap2 = map_gen->getGameMap();

  		for (i=0; i < GameMap2->mapsize;i++){
  				posy = i/ GameMap2->width;
  				posx = i -(i/ GameMap2->width)* GameMap2->width ;
  				x = (posx - posy) * (TILE_WIDTH-GAME_ZOOM*4)/2+view_x;
  				y = (posx + posy) * (TILE_HEIGHT-GAME_ZOOM*2)/2+view_y-(GameMap2->hdraw[i]*8);
  				if(x < SCREEN_WIDTH && y <SCREEN_HEIGHT && y > -comp_w && x > -comp_h){
  					SDL_Rect DestR;
  			    DestR.x = x;
  			    DestR.y = y;
  					DestR.h = base_tilemap->tile[GameMap2->index[i]]->h-GAME_ZOOM*2;
  			    DestR.w = base_tilemap->tile[GameMap2->index[i]]->w-GAME_ZOOM*4;
  					SDL_SetTextureBlendMode(base_tilemap->tile[GameMap2->index[i]]->texture,SDL_BLENDMODE_ADD);
  					SDL_RenderCopy(render, base_tilemap->tile[GameMap2->index[i]]->texture, NULL, &DestR); // Copy the texture into render
  				}
  		}
  		return true;
  }

  int RENDERER::setbase_tilemap(Tilemap2 *newbase_tilemap){
    base_tilemap = newbase_tilemap;
    return 0;
  }

  int RENDERER::setmapgen(MAP_GEN *newmap_gen){
    map_gen = newmap_gen;
    return 0;
  }

  int RENDERER::setrenderer(SDL_Renderer *newrender){
    render = newrender;
    return 0;
  }

  int *RENDERER::getGAME_X(){
    return &GAME_X;
  }

  int *RENDERER::getGAME_Y(){
    return &GAME_Y;
  }
  int *RENDERER::getGAME_ZOOM(){
    return &GAME_ZOOM;
  }
