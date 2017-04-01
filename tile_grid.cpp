#include <iostream>

#include <mapnik/datasource_cache.hpp>
#include "map_tile.hpp"

int main(int argc, char** argv)
{
    mapnik::datasource_cache::instance().register_datasources("/usr/lib/mapnik/3.0/input/");

    std::string xml_file = argv[1];
    std::string sx = argv[2];
    std::string sy = argv[3];
    std::string sz = argv[4];
    std::string type = argv[5];

    int x, y, z;
    x = std::stoi(sx);
    y = std::stoi(sy);
    z = std::stoi(sz);

    MapTile map_tile = MapTile(xml_file);
    //map_tile.genTile(23512, 14765, 15);
    if (type.compare("grid") == 0) {
        map_tile.genGrid(x, y, z);
    }
    else {
        map_tile.genTile(x, y, z);
    }

    return 0;
}

