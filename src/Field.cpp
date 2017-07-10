#include "Field.h"
#include "format.h"

using std::logic_error;
using std::out_of_range;


Field::~Field()
{
    for (auto &i: _hex_map) {
        delete i.second;
    }
    delete _bank;
}

Hex &Field::put_hex(Coord xy)
{
    if (has_hex(xy)) {
        throw logic_error(fmt::format("cannot put hex at {} : spot is occupied", xy.str()));
    }
    Hex *p = new Hex();
    _hex_map.insert( {xy, p} );
    return *p;
}

void Field::remove_hex(Coord xy)
{
    auto hex_it = _hex_map.find(xy);
    if (hex_it == _hex_map.end()) {
        throw logic_error(fmt::format("cannot remove hex from {}: there is no hex", xy.str()));
    }
    delete hex_it->second;
    _hex_map.erase(hex_it);
}

Hex &Field::hex(Coord xy) const
{
    if (!has_hex(xy)) {
        throw logic_error(fmt::format("cannot return hex at {}: there is no hex", xy.str()));
    }
    return *_hex_map.find(xy)->second;
}

Coord Field::coord(Hex &h) const
{
    for (auto i = _hex_map.begin(); i != _hex_map.end(); i++) {
        if (i->second == &h) {
            return i->first;
        }
    }
    throw logic_error("cannot find such hex");
}

Hex &Field::hex(size_t i) const
{
    if (i >= _hex_map.size()) {
        throw out_of_range("Hex &Field::hex() OUT OF RANGE");
    }
    auto j = _hex_map.begin();
    for (; i; i--) {
        j++;
    }
    return *j->second;
}

void Field::link_road(Road &r, Coord xy, RoadSide rdsd)
{
    if (linked(r)) {
        auto road_coord = coord(r);
        throw logic_error(fmt::format("cannot link road {0} : road is already linked to {1} {2}", r.name(), road_coord.first.str(), EnumInfo::road_side_str(road_coord.second)));
    }
    if (has_road(xy, rdsd)) {
        throw logic_error(fmt::format("cannot link road {0} to {1} {2}: spot is occupied", r.name(), xy.str(), EnumInfo::road_side_str(rdsd)));
    }
    _road_map.insert( {{xy, rdsd}, &r} );
}

void Field::unlink_road(Coord xy, RoadSide rdsd)
{
    if (!has_road(xy, rdsd)) {
        throw logic_error(fmt::format("cannot unlink road from {0} {1} : there is no road", xy.str(), EnumInfo::road_side_str(rdsd)));
    }
    _road_map.erase( {xy, rdsd} );
}

void Field::unlink_road(Road &r)
{
    for (auto i = _road_map.begin(); i != _road_map.end(); i++) {
        if (&r == i->second) {
            _road_map.erase(i);
            return;
        }
    }
    throw logic_error(fmt::format("cannot unlink road {} : no such road on the map", r.name()));
}

Road &Field::road(Coord xy, RoadSide rdsd) const
{
    if (!has_road(xy, rdsd)) {
        throw logic_error(fmt::format("cannot return road at {0} {1} : there is no road", xy.str(), EnumInfo::road_side_str(rdsd)));
    }
    return *_road_map.find( {xy, rdsd} )->second;
}

pair<Coord, RoadSide> Field::coord(Road &r) const
{
    for (auto i = _road_map.begin(); i != _road_map.end(); i++) {
        if (&r == i->second) {
            return i->first;
        }
    }
    throw logic_error(fmt::format("cannot find road {} on the map", r.name()));
}

void Field::link_locality(Locality &loc, Coord xy, CrossCorner cr_corn)
{
    if (linked(loc)) {
        auto loc_coord = coord(loc);
        throw logic_error(fmt::format("cannot link locality {0} : locality is already linked to {1} {2}", loc.name(), loc_coord.first.str(), EnumInfo::cross_corner_str(loc_coord.second)));
    }
    if (has_locality(xy, cr_corn)) {
        throw logic_error(fmt::format("cannot link locality {0} to {1} {2} : spot is occupied", loc.name(), xy.str(), EnumInfo::cross_corner_str(cr_corn)));
    }
    _locality_map.insert( {{xy, cr_corn}, &loc} );
}

void Field::unlink_locality(Coord xy, CrossCorner cr_corn)
{
    if (!has_locality(xy, cr_corn)) {
        throw logic_error(fmt::format("cannot unlink locality from {0} {1} : there is no locality", xy.str(), EnumInfo::cross_corner_str(cr_corn)));
    }
    _locality_map.erase( {xy, cr_corn} );
}

void Field::unlink_locality(Locality &loc)
{
    for (auto i = _locality_map.begin(); i != _locality_map.end(); i++) {
        if (&loc == i->second) {
            _locality_map.erase(i);
            return;
        }
    }
    throw logic_error(fmt::format("cannot unlink locality {} : no such locality on the map", loc.name()));
}

Locality &Field::locality(Coord xy, CrossCorner cr_corn) const
{
    if (!has_locality(xy, cr_corn)) {
        throw logic_error(fmt::format("cannot return locality at {0} {1} : there is no locality", xy.str(), EnumInfo::cross_corner_str(cr_corn)));
    }
    return *_locality_map.find( {xy, cr_corn} )->second;
}

pair<Coord, CrossCorner> Field::coord(Locality &loc) const
{
    for (auto i = _locality_map.begin(); i != _locality_map.end(); i++) {
        if (&loc == i->second) {
            return i->first;
        }
    }
    throw logic_error(fmt::format("cannot find locality {} on the map", loc.name()));
}

bool Field::linked(const Infrastructure &inf) const noexcept
{
    for (auto &i: _road_map) {
        if (i.second == &inf) {
            return true;
        }
    }
    for (auto &i: _locality_map) {
        if (i.second == &inf) {
            return true;
        }
    }
    return false;
}

void Field::unset_robber()
{
    if (!_has_robber) {
        throw logic_error("cannot unset robber: there is no robber on the map");
    }
    _has_robber = false;
}

Coord Field::robber() const
{
    if (!_has_robber) {
        throw logic_error("cannot return robber's spot: there is no robber on the map");
    }
    return _robber;
}
