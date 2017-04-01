#include <map>
#include <mapnik/grid/grid.hpp>
#include "json_object.hpp"
#include "grid_utils.hpp"

void put_utf_grid(JsonObject& json, mapnik::grid& grid, std::vector<std::string> &key_order, int resolution) {
    JsonArray lines;

    typedef std::map<std::string, mapnik::grid::value_type> keys_map;
    typedef keys_map::const_iterator keys_iterator;

    mapnik::grid::feature_key_type const& feature_keys = grid.get_feature_keys();
    mapnik::grid::feature_key_type::const_iterator feature_pos;

	keys_map keys;

	// start counting at utf8 codepoint 32, aka space character
	uint16_t codepoint = 32;
	std::vector<std::string> utf_grid;
	std::vector<UChar> line;

	for (unsigned y = 0; y < grid.height(); y+=resolution) {
		mapnik::grid::value_type const* row = grid.get_row(y);
		line.clear();

		for (unsigned x = 0; x < grid.width(); x+=resolution) {
			feature_pos = feature_keys.find(row[x]);
			if (feature_pos != feature_keys.end()) {
				std::string val = feature_pos->second;
				keys_iterator key_pos = keys.find(val);

				if (key_pos == keys.end()) {
					// Create a new entry for this key. Skip the codepoints that
					// can't be encoded directly in JSON.
					if (codepoint == 34) ++codepoint;      				// Skip "
					else if (codepoint == 92) ++codepoint; 				// Skip backslash

                    if (row[x] == mapnik::grid::base_mask) {
                        keys[""] = codepoint;
                        key_order.push_back("");
                    }
                    else {
                        keys[val] = codepoint;
                        key_order.push_back(val);
                    }

					line.push_back(static_cast<UChar>(codepoint));
					++codepoint;
				}
				else {
					line.push_back(static_cast<UChar>(key_pos->second));
				}
			}
			// else, shouldn't get here...
		}

		//convert line to unicode string and append to utf_grid
		UnicodeString ustr(&line[0], line.size());
		lines.add(ustr);
	}

	json["grid"] = lines;
}

void put_grid_keys(JsonObject& json, std::vector<std::string> &key_order) {
	JsonArray keys;
	for(int i = 0, size = key_order.size(); i < size; i++) {
		keys.add(key_order[i]);
	}
	json["keys"] = keys;
}

void put_grid_features(JsonObject& json, mapnik::grid &grid, std::vector<std::string> &key_order) {
	mapnik::grid::feature_type const& g_features = grid.get_grid_features();

	if (g_features.size() <= 0){
		return;
	}

	std::set<std::string> const& attributes = grid.get_fields();
	mapnik::grid::feature_type::const_iterator feat_end = g_features.end();

	JsonObject data;

    for (std::string const& key_item : key_order) {
		if (key_item.empty()){
			continue;
		}

		mapnik::grid::feature_type::const_iterator feat_itr = g_features.find(key_item);
		if (feat_itr == feat_end){
			continue;
		}

		mapnik::feature_ptr feature = feat_itr->second;
		JsonObject attrJson;
        bool found = false;

        for (std::string const& attr : attributes) {
			if (feature->has_key(attr)){
				const mapnik::value& value = feature->get(attr);
				attrJson[attr] = value;
                found = true;
			}
		}

        if (found) {
		    data[key_item] = attrJson;
        }
	}

	json["data"] = data;
}

void grid_encode(JsonObject& json, mapnik::grid& grid, int resolution) {
    std::vector<std::string> key_order;

    put_utf_grid(json, grid, key_order, resolution);
    put_grid_keys(json, key_order);
    put_grid_features(json, grid, key_order);
}

