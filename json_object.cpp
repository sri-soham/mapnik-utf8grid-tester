#include <iostream>
#include <sstream>
#include <map>
#include <typeinfo>
#include <unicode/unistr.h>
#include "json_object.hpp"

// JsonObject
void JsonObject::put(const std::string& key, json_type value) {
    json[key] = value;
}

json_type JsonObject::get(std::string key) {
    std::map<std::string, json_type>::iterator it = json.find(key);
    if (it == json.end()) {
        return json_type(mapnik::value_null());
    }
    return it->second;
}

json_type& JsonObject::operator[](const std::string& key) {
    return json[key];
}

std::string JsonObject::to_string() {
    std::map<std::string, json_type>::iterator it;

    std::ostringstream os;
    os << "{";
    for (it=json.begin(); it != json.end(); it++) {
        if (it != json.begin()) {
            os << ",";
        }
        os << "\"" << it->first << "\":" << boost::apply_visitor(to_string_visitor::get_instance(), json[it->first]);
    }
    os << "}";

    return os.str();
}

// JsonArray
void JsonArray::add(json_type value) {
	json_array.push_back(value);
}

json_type JsonArray::get(int index) {
	return json_array.at(index);
}

json_type& JsonArray::operator[](int index) {
	return json_array[index];
}

int JsonArray::size() {
	return json_array.size();
}

std::string JsonArray::to_string() {
	std::ostringstream os;
	os << "[";
	for(int i = 0, size = json_array.size(); i < size; i++) {
		if(i != 0)
			os << ",";
		os << boost::apply_visitor(to_string_visitor::get_instance(), json_array[i]);
	}
	os << "]";

	return os.str();
}

//to_string_visitor

to_string_visitor* to_string_visitor::instance = 0;

to_string_visitor& to_string_visitor::get_instance() {
	if(!instance) {
		instance = new to_string_visitor();
	}
	return *instance;
}

std::string to_string_visitor::operator()(mapnik::value& value) const {
    if (typeid(value) == typeid(mapnik::value_unicode_string)) {
		std::ostringstream os;
		std::string s;
        mapnik::value_unicode_string uni = value.to_unicode();
        mapnik::to_utf8(uni, s);
        os << "\"" << s << "\"";
		return os.str();
	} else if(typeid(value) == typeid(mapnik::value_null)){
		return std::string("null");
	} else {
		return value.to_string();
	}
}

std::string to_string_visitor::operator()(JsonObject& value) const {
	return value.to_string();
}

std::string to_string_visitor::operator()(JsonArray& value) const {
	return value.to_string();
}

std::string to_string_visitor::operator()(std::string& value) const {
	std::ostringstream os;
	os << "\"" << value << "\"";
	return os.str();
}
// END:: JsonObject, JsonArray
