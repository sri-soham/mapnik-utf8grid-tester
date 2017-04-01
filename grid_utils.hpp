#ifndef GRID_UTILS_HPP
#define GRID_UTILS_HPP

#include <vector>
#include <mapnik/grid/grid.hpp>
#include "json_object.hpp"

void put_utf_grid(JsonObject& json, mapnik::grid& grid, std::vector<std::string> &key_order, int resolution);

void put_grid_keys(JsonObject& json, std::vector<std::string> &key_order);

void put_grid_features(JsonObject& json, mapnik::grid &grid, std::vector<std::string> &key_order);

void grid_encode(JsonObject& json, mapnik::grid& grid, int resolution);

#endif
