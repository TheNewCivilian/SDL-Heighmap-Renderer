
#OBJS specifies which files to compile as part of the project
OBJS = main.cpp game_sdl.cpp map_main.cpp renderer_main.cpp ./bmpLoader/bitmap.c ./bmpLoader/bittypes.c BMP_Loader.c
#02_getting_an_image_on_the_screen.cpp
#01_hello_SDL.cpp

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = HigthmapRenderer.out

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
