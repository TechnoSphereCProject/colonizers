#include "ThirdStageSubEngine.h"
#include <exception>
using std::invalid_argument;

const std::map<Resource, size_t>
    ThirdStageSubEngine::resources_per_road = {{Resource::CLAY, 1}, {Resource::WOOD, 1}},
    ThirdStageSubEngine::resources_per_city = {{Resource::ORE, 3}, {Resource::GRAIN, 2}},
    ThirdStageSubEngine::resources_per_town =
        {{Resource::CLAY, 1}, {Resource::WOOD, 1}, {Resource::GRAIN, 1}, {Resource::WOOL, 1}};



ThirdStageSubEngine::ThirdStageSubEngine(Field &field, const Player &player)
    :_field(field), _player(player)
{
    //init city spots
    for (size_t i = 0; i < _player.num_towns(); i++) {
        if (_field.linked(_player.town(i))) {
            _city_spots.insert( _field.coord(_player.town(i)));
        }
    }


    //init town & road spots
    for (size_t i = 0; i <_player.num_roads(); i++) {
        try {
            std::pair<Coord, RoadSide> road_coord = _field.coord(_player.road(i));

            auto road_spots = road_spots_around(road_coord.first, road_coord.second);
            for (auto &rs: road_spots) {
                if (in_field(rs.first, rs.second) && !_field.has_road(rs.first, rs.second)) {
                    _road_spots.insert(rs);
                }
            }

            auto town_spots = town_spots_around(road_coord.first, road_coord.second);
            for (auto &ts: town_spots) {
                if (in_field(ts.first, ts.second) && free_locality_spot(ts.first, ts.second)) {
                    _town_spots.insert(ts);
                }
            }
        } catch (std::logic_error) {

        }
    }
}

void ThirdStageSubEngine::build_road(Road &road, Coord xy, RoadSide road_side)
{
    if (_field.linked(road)) {
        throw invalid_argument("cannot build road: " + _player.name() + "'s road " + road.name() + " is already built");
    } else if (_road_spots.count( {xy, road_side} ) == 0) {
        throw invalid_argument("cannot build road: spot " + xy.str() +
            " " + EnumInfo::road_side_str(road_side) + " is incorrect");
    } else if (! enough_resources(resources_per_road)) {
        throw invalid_argument("cannot build road: player " + _player.name() + " hasn't enough resources");
    }

    _field.link_road(road, xy, road_side);

    _modify_road_spots(xy, road_side);
    _modify_town_spots(xy, road_side);

    for (auto &i: resources_per_road) {
        _player.bank().remove(i.first, i.second);
        _field.bank().add(i.first, i.second);
    }
}

void ThirdStageSubEngine::build_town(Town &town, Coord xy, CrossCorner cross_corner)
{
    if (_field.linked(town)) {
        throw invalid_argument("cannot build town: " + _player.name() + "'s town " + town.name() + " is already built");
    } else if (_town_spots.count( {xy, cross_corner} ) == 0) {
        throw invalid_argument("cannot build town: spot " + xy.str() +
            " " + EnumInfo::cross_corner_str(cross_corner) + " is incorrect");
    } else if (! enough_resources(resources_per_town)) {
        throw invalid_argument("cannot build town: player " + _player.name() + " hasn't enough resources");
    }

    _field.link_locality(town, xy, cross_corner);

    _modify_town_spots(xy, cross_corner);
    _city_spots.insert( {xy, cross_corner} );

    for (auto &i: resources_per_town) {
        _player.bank().remove(i.first, i.second);
        _field.bank().add(i.first, i.second);
    }
}

void ThirdStageSubEngine::build_city(City &city, Coord xy, CrossCorner cross_corner)
{
    if (_field.linked(city)) {
        throw invalid_argument("cannot build city: " + _player.name() + "'s city " + city.name() + " is already built");
    } else if (_city_spots.count( {xy, cross_corner} ) == 0) {
        throw invalid_argument("cannot build city: spot " + xy.str() +
            " " + EnumInfo::cross_corner_str(cross_corner) + " is incorrect");
    } else if (! enough_resources(resources_per_city)) {
        throw invalid_argument("cannot build city: player " + _player.name() + " hasn't enough resources");
    }

    _field.unlink_locality(xy, cross_corner);
    _field.link_locality(city, xy, cross_corner);

    _city_spots.erase( {xy, cross_corner} );

    for (auto &i: resources_per_city) {
        _player.bank().remove(i.first, i.second);
        _field.bank().add(i.first, i.second);
    }
}

bool ThirdStageSubEngine::enough_resources(const std::map<Resource, size_t> &resources_per_infrastructure)
    const noexcept
{
    for (auto &i: resources_per_infrastructure) {
        if (_player.bank().resources(i.first) < i.second) {
            return false;
        }
    }
    return true;
}

bool ThirdStageSubEngine::free_locality_spot(Coord xy, CrossCorner cross_corner) const noexcept
{
    if (_field.has_locality(xy, cross_corner)) {
        return false;
    }
    if (cross_corner == CrossCorner::BOTTOM) {
        return !_field.has_locality(xy.south_west(), CrossCorner::TOP) &&
            !_field.has_locality(xy.south_east(), CrossCorner::TOP) &&
            !_field.has_locality(xy.south_east().south_west(), CrossCorner::TOP);
    } else {
        return !_field.has_locality(xy.north_west(), CrossCorner::BOTTOM) &&
            !_field.has_locality(xy.north_east(), CrossCorner::BOTTOM) &&
            !_field.has_locality(xy.north_east().north_west(), CrossCorner::BOTTOM);
    }
}

bool ThirdStageSubEngine::in_field(Coord xy, CrossCorner cross_corner) const noexcept
{
    if (_field.has_hex(xy)) {
        return true;
    } else if (cross_corner == CrossCorner::BOTTOM) {
        return _field.has_hex(xy.south_east()) || _field.has_hex(xy.south_west());
    } else {
        return _field.has_hex(xy.north_east()) || _field.has_hex(xy.north_west());
    }
}

bool ThirdStageSubEngine::in_field(Coord xy, RoadSide road_side) const noexcept
{
    if (_field.has_hex(xy)) {
        return true;
    } else if (road_side == RoadSide::DOWN) {
        return _field.has_hex(xy.south_east());
    } else if (road_side == RoadSide::RIGHT) {
        return _field.has_hex(xy.east());
    } else {
        return _field.has_hex(xy.north_east());
    }
}

std::set<std::pair<Coord, RoadSide>> ThirdStageSubEngine::road_spots_around(Coord xy, RoadSide road_side) noexcept
{
    std::set< std::pair<Coord, RoadSide> > tmp;
    if (road_side == RoadSide::DOWN) {
        tmp.insert( {xy, RoadSide::RIGHT} );
        tmp.insert( {xy.south_east(), RoadSide::UP} );
        tmp.insert( {xy.south_west(), RoadSide::UP} );
        tmp.insert( {xy.south_west(), RoadSide::RIGHT} );
    } else if (road_side == RoadSide::RIGHT) {
        tmp.insert( {xy, RoadSide::DOWN} );
        tmp.insert( {xy, RoadSide::UP} );
        tmp.insert( {xy.north_east(), RoadSide::DOWN} );
        tmp.insert( {xy.south_east(), RoadSide::UP} );
    } else {
        tmp.insert( {xy, RoadSide::RIGHT} );
        tmp.insert( {xy.north_east(), RoadSide::DOWN} );
        tmp.insert( {xy.north_west(), RoadSide::RIGHT} );
        tmp.insert( {xy.north_west(), RoadSide::DOWN} );
    }
    return tmp;
}

std::set<std::pair<Coord, CrossCorner>> ThirdStageSubEngine::town_spots_around(Coord xy, RoadSide road_side) noexcept
{
    std::set< std::pair<Coord, CrossCorner> > tmp;
    if (road_side == RoadSide::DOWN) {
        tmp.insert( {xy, CrossCorner::BOTTOM} );
        tmp.insert( {xy.south_east(), CrossCorner::TOP} );
    } else if (road_side == RoadSide::RIGHT) {
        tmp.insert( {xy.north_east(), CrossCorner::BOTTOM} );
        tmp.insert( {xy.south_east(), CrossCorner::TOP} );
    } else {
        tmp.insert( {xy, CrossCorner::TOP} );
        tmp.insert( {xy.north_east(), CrossCorner::BOTTOM} );
    }
    return tmp;
}

Road &ThirdStageSubEngine::road(const std::string &name) const
{
    for (size_t i = 0; i < _player.num_roads(); i++) {
        if (_player.road(i).name() == name) {
            return _player.road(i);
        }
    }
    throw invalid_argument("cannot build: player " + _player.name() + " hasn't road \"" + name + '\"');
}

Town &ThirdStageSubEngine::town(const std::string &name) const
{
    for (size_t i = 0; i < _player.num_towns(); i++) {
        if (_player.town(i).name() == name) {
            return _player.town(i);
        }
    }
    throw invalid_argument("cannot build: player " + _player.name() + " hasn't town \"" + name + '\"');
}

City &ThirdStageSubEngine::city(const std::string &name) const
{
    for (size_t i = 0; i < _player.num_cities(); i++) {
        if (_player.city(i).name() == name) {
            return _player.city(i);
        }
    }
    throw invalid_argument("cannot build: player " + _player.name() + " hasn't city \"" + name + '\"');
}

void ThirdStageSubEngine::_modify_road_spots(Coord used_spot_coord, RoadSide used_spot_road_side) {
    _road_spots.erase( {used_spot_coord, used_spot_road_side} );
    auto possible_road_spots = road_spots_around(used_spot_coord, used_spot_road_side);
    for (auto &i: possible_road_spots) {
        if (in_field(i.first, i.second) && !_field.has_road(i.first, i.second)) {
            _road_spots.insert(i);
        }
    }
}

void ThirdStageSubEngine::_modify_town_spots(Coord used_spot_coord, RoadSide used_spot_road_side) {
    auto possible_town_spots = town_spots_around(used_spot_coord, used_spot_road_side);
    for (auto &i: possible_town_spots) {
        if (in_field(i.first, i.second) && free_locality_spot(i.first, i.second)) {
            _town_spots.insert(i);
        }
    }
}

void ThirdStageSubEngine::_modify_town_spots(Coord used_spot_coord, CrossCorner used_spot_cross_corner) {
    _town_spots.erase( {used_spot_coord, used_spot_cross_corner} );
    std::set<std::pair<Coord, CrossCorner>> has_to_delete;
    for (auto &i: _town_spots) {
        if (!free_locality_spot(i.first, i.second)) {
            has_to_delete.insert(i);
        }
    }
    for (auto &i: has_to_delete) {
        _town_spots.erase(i);
    }
}
