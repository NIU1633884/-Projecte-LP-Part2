#pragma once

#include <functional>
#include <string>
#include <vector>

typedef std::pair<std::string, std::string> PAIR_ATTR_VALUE;
typedef std::pair<std::string, std::vector<PAIR_ATTR_VALUE>> CHILD_NODE;


struct Coordinate {
    double lat;
    double lon;

    // Define comparison operators for coordinates
    bool operator<(const Coordinate& other) const {
        // Compare based on lat and lon, you can define your comparison logic here
        if (lat != other.lat) {
            return lat < other.lat;
        }
        return lon < other.lon;
    }

    bool operator>(const Coordinate& other) const {
        // Define the > operator similarly to <
        // Compare based on lat and lon
        if (lat != other.lat) {
            return lat > other.lat;
        }
        return lon > other.lon;
    }
};

extern "C" typedef struct {
    int i;
    double lat;
    double lon;
    unsigned int color;
    const char* title;
} PoiBridge;

extern "C" typedef struct {
    double* lats;
    double* lons;
    int size;
} WayBridge;

typedef struct {
    std::string id_element;
    std::vector<PAIR_ATTR_VALUE> atributs;
    std::vector<CHILD_NODE> fills;
} XmlElement;


