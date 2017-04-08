#include <iostream>
#include <sstream>

#include <mapnik/map.hpp>
#include <mapnik/layer.hpp>
#include <mapnik/rule.hpp>
#include <mapnik/feature_type_style.hpp>
#include <mapnik/symbolizer.hpp>
#include <mapnik/text/placements/dummy.hpp>
#include <mapnik/text/text_properties.hpp>
#include <mapnik/text/formatting/text.hpp>
#include <mapnik/datasource_cache.hpp>
#include <mapnik/font_engine_freetype.hpp>
#include <mapnik/agg_renderer.hpp>
#include <mapnik/expression.hpp>
#include <mapnik/color_factory.hpp>
#include <mapnik/image_util.hpp>
#include <mapnik/unicode.hpp>
#include <mapnik/save_map.hpp>
#include <mapnik/cairo_io.hpp>
#include <mapnik/projection.hpp>
#include <mapnik/load_map.hpp>
#include <mapnik/grid/grid.hpp>
#include <mapnik/grid/grid_renderer.hpp>
#include <boost/algorithm/string/replace.hpp>

#if defined(HAVE_CAIRO)
#include <mapnik/cairo/cairo_renderer.hpp>
#include <mapnik/cairo/cairo_image_util.hpp>
#endif

#include "google_projection.hpp"
#include "lat_lng.hpp"
#include "map_tile.hpp"
#include "json_object.hpp"
#include "python_grid_utils.hpp"

using namespace mapnik;

const std::string MapTile::SRS_MERC = "+proj=merc +a=6378137 +b=6378137 +lat_ts=0.0 +lon_0=0.0 +x_0=0.0 +y_0=0.0 +k=1.0 +units=m +nadgrids=@null +wktext +no_defs +over";

MapTile::MapTile(std::string xml_file)
{
    m_xml_file = xml_file;
}
void MapTile::genTile(int x, int y, int z)
{
    m_map = Map(TILE_WIDTH, TILE_HEIGHT);
    load_map(m_map, m_xml_file);
    box2d<double> box = getBox(x, y, z);
    m_map.zoom_to_box(box);

    image_rgba8 buf(m_map.width(), m_map.height());
    agg_renderer<image_rgba8> ren(m_map, buf);
    ren.apply();
    save_to_stream(buf, std::cout, "png");
}

void MapTile::genGrid(int x, int y, int z)
{
    m_map = Map(TILE_WIDTH, TILE_HEIGHT);
    load_map(m_map, m_xml_file);
    box2d<double> box = getBox(x, y, z);
    m_map.zoom_to_box(box);

    std::string known_id_key = "ID";
    mapnik::grid my_grid(m_map.width(), m_map.height(), known_id_key);
    std::vector<std::string> fields;
    fields.push_back("ID");
    fields.push_back("NAME");
    fields.push_back("TYPE");
    fields.push_back("ADMIN_LEVE");
    render_layer_for_grid(m_map, my_grid, 0, fields, 1.0, 0, 0);
    grid_json_data json = grid_encode<mapnik::grid>(my_grid, "utf", true, 4);
    std::string json_str = get_json_string(json);
    std::cout << json_str << std::endl;
}

box2d<double> MapTile::getBox(int x, int y, int z)
{
    GoogleProjection gprj;
    LatLng i0, i1, p0, p1;
    i0.x = TILE_WIDTH*x;
    i0.y = TILE_HEIGHT*(y+1);
    i1.x = TILE_WIDTH*(x+1);
    i1.y = TILE_HEIGHT*y;

    p0 = gprj.fromPixelToLL(i0, z);
    p1 = gprj.fromPixelToLL(i1, z);
    
    projection proj = projection(SRS_MERC, false);
    proj.forward(p0.x, p0.y);
    proj.forward(p1.x, p1.y);

    box2d<double> res = box2d<double>(p0.x, p0.y, p1.x, p1.y);

    return res;
}
