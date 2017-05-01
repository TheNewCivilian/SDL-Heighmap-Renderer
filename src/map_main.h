#ifndef MAP_MAIN_H
#define MAP_MAIN_H

#include "bitmap.h"
#include "bittypes.h"
#include "Bmp_loader.h"
#include "debug_color.h"

typedef struct _heighmap{
  int *index;
  int *height;
  int *hdraw;
  int mapsize;
  int mheight;
  int width;
}Heightmap;

class MAP_GEN{
public:
    Heightmap *GameMap2;
    
public:
    bool load_highMap(const char *filepath);
    Heightmap *getGameMap();
protected:
    bool checkmap(int hight);
};

#endif
