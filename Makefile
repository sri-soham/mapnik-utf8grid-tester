.RECIPEPREFIX = >
CC = g++
CFLAGS = -std=c++11 -Wall
UNAME_P = $(shell uname -p)
ifeq ($(UNAME_P),x86_64)
    CFLAGS += -DBIGINT
endif


google_projection.o : google_projection.cpp google_projection.hpp lat_lng.hpp
>   $(CC) $(CFLAGS) -c google_projection.cpp

python_grid_utils.o : python_grid_utils.hpp python_grid_utils.cpp
>   $(CC) $(CFLAGS) -DGRID_RENDERER -lmapnik -licuuc -c python_grid_utils.cpp

map_tile.o : map_tile.cpp map_tile.hpp python_grid_utils.hpp python_grid_utils.cpp
>   $(CC) $(CFLAGS) -DGRID_RENDERER -lmapnik -lpng -lcairo -lfreetype -licuuc -c map_tile.cpp

tile_grid.o : tile_grid.cpp
>   $(CC) $(CFLAGS) -DGRID_RENDERER -c tile_grid.cpp

all : google_projection.o map_tile.o python_grid_utils.o tile_grid.o
>   $(CC) $(CFLAGS) -DGRID_RENDERER google_projection.o python_grid_utils.o map_tile.o tile_grid.o -lmapnik -lpng -lcairo -lfreetype -licuuc -o tile_grid

clean:
>   rm -f *.o tile_grid
