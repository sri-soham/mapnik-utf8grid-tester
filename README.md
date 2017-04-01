# Attempt to generate utf grids through c++
This has been developed on Ubuntu 16.04 with g++ 5.4.0.
My experience with c++ is very elementary. Code, particularly in the
grid_utils.cpp, comes from these two sources:
[Python Mapnik](https://github.com/mapnik/python-mapnik/blob/master/src/python_grid_utils.cpp)
[Mapnik JNI](https://github.com/wisllayvitrio/mapnik-jni/blob/master/csrc/class_grid.cpp)
with slight modifications.

2017-04-01:
On this date, the json grid is empty even when the corresponding tile contains features.
For example:
`./tile_grid "map.xml" "23512" "14765" "15" "tile"`
generates a tile with a polygon (multi-polygon?) on it. But
`./tile_grid "map.xml" "23512" "14765" "15" "grid"`
produces an empty grid.

tile_grid is the executable generated with `make all`.
map.xml is the mapnik xml file.
23512 is the x as in the xyz tile url.
14765 is the y as in the xyz tile url.
15 is the z as in the xyz tile url
Use "tile" to generate the 256x256 png image for the given x, y, z.
Use "grid" to generate the utf grid in json format for the given x, y, z.

When using "tile" the image will be written to standard output, redirect it
to a file to get the image. Something like this:
`./tile_grid "map.xml" "23512" "14765" "15" "tile" > 23512_14765_15.png`
