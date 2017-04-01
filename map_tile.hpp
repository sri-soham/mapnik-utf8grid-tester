#ifndef MAP_TILE_H
#define MAP_TILE_H

#include <iostream>
#include <mapnik/map.hpp>

class MapTile {
    private:
        mapnik::Map m_map;
        std::string m_xml_file;

        mapnik::box2d<double> getBox(int x, int y, int z);

    public:
        static const int TILE_WIDTH = 256;
        static const int TILE_HEIGHT = 256;
        static const std::string SRS_MERC;
        MapTile(std::string xml_file);
        void genTile(int x, int y, int z);
        void genGrid(int x, int y, int z);

};

#endif
