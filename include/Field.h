#ifndef FIELD_H
#define FIELD_H

#include <Coord.h>
#include <RoadSide.h>
#include <CrossCorner.h>
#include <Hex.h>
#include <Locality.h>
#include <Road.h>

#include <stdexcept>
#include <map>
#include <utility>
using std::pair;

class Field
{
    Field(const Field &) = delete;
    Field &operator= (const Field &) = delete;

public:
    Field() {_bank = new ResourcesHolder();}
    ~Field();

    ResourcesHolder &bank() const noexcept {return *_bank;}

    Hex &put_hex(Coord);
    void remove_hex(Coord);
    Hex &hex(Coord) const;
    Coord coord(Hex &) const;
    bool has_hex(Coord xy) const noexcept {return _hex_map.count(xy);}
    size_t num_hexes() const noexcept {return _hex_map.size();}
    Hex &hex(size_t i) const;

    void link_road(Road &, Coord, RoadSide);
    void unlink_road(Coord, RoadSide);
    void unlink_road(Road &);
    bool has_road(Coord xy, RoadSide rs) const noexcept {return _road_map.count({xy, rs});}
    Road &road(Coord, RoadSide) const;
    pair<Coord, RoadSide> coord(Road &) const;

    void link_locality(Locality &, Coord, CrossCorner);
    void unlink_locality(Coord, CrossCorner);
    void unlink_locality(Locality &);
    bool has_locality(Coord xy, CrossCorner cc) const noexcept {return _locality_map.count({xy, cc});}
    Locality &locality(Coord, CrossCorner) const;
    pair<Coord, CrossCorner> coord(Locality &) const;

    bool linked(const Infrastructure &) const noexcept;

    void set_robber(Coord xy) noexcept {_has_robber = true; _robber = xy;}
    void unset_robber();
    bool has_robber() const noexcept {return _has_robber;}
    Coord robber() const;

private:
    ResourcesHolder *_bank;

    std::map< Coord, Hex * > _hex_map;
    std::map< pair<Coord, RoadSide>, Road * > _road_map;
    std::map< pair<Coord, CrossCorner>, Locality * > _locality_map;

    bool _has_robber = false;
    Coord _robber;
};

#endif // FIELD_H
