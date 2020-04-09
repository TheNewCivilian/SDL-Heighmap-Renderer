#include "map_main.h"

bool MAP_GEN::load_highMap(const char *filepath){
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
	return true;
}


  bool MAP_GEN::checkmap(int hight){
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

  Heightmap *MAP_GEN::getGameMap(){
    return GameMap2;
  }
