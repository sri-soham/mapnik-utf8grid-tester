.RECIPEPREFIX = >
CC = g++
CFLAGS = -g -std=c++11 -Wall
UNAME_P = $(shell uname -p)
ifeq ($(UNAME_P),x86_64)
    CFLAGS += -D BIGINT
endif


google_projection.o : google_projection.cpp google_projection.hpp lat_lng.hpp
>   $(CC) $(CFLAGS) -c google_projection.cpp

json_object.o : json_object.cpp json_object.hpp
>   $(CC) $(CFLAGS) -D GRID_RENDERER -licuuc -c json_object.cpp

grid_utils.o : grid_utils.cpp json_object.hpp json_object.cpp grid_utils.hpp
>   $(CC) $(CFLAGS) -D GRID_RENDERER -lmapnik -c grid_utils.cpp

map_tile.o : map_tile.cpp map_tile.hpp grid_utils.cpp json_object.cpp json_object.hpp
>   $(CC) $(CFLAGS) -D GRID_RENDERER -lmapnik -lpng -lcairo -lfreetype -licuuc -c map_tile.cpp

tile_grid.o : tile_grid.cpp
>   $(CC) $(CFLAGS) -c tile_grid.cpp

all : google_projection.o json_object.o map_tile.o grid_utils.o tile_grid.o
>   $(CC) google_projection.o json_object.o map_tile.o grid_utils.o tile_grid.o $(CFLAGS) -D GRID_RENDERER -lmapnik -lpng -lcairo -lfreetype -licuuc -o tile_grid

clean:
>   rm -f *.o tile_grid
