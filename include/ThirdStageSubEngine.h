#ifndef THIRDSTAGESUBENGINE_H
#define THIRDSTAGESUBENGINE_H

#include <Field.h>
#include <Player.h>
#include <Logger.h>
#include <set>

class ThirdStageSubEngine
{
    ThirdStageSubEngine(const ThirdStageSubEngine &) = delete;
    ThirdStageSubEngine &operator= (const ThirdStageSubEngine &) = delete;

public:
    static const std::map<Resource, size_t> resources_per_road, resources_per_town, resources_per_city;

    ThirdStageSubEngine(Field &, const Player &);

    void build_road(Road &road, Coord, RoadSide);
    void build_town(Town &town, Coord, CrossCorner);
    void build_city(City &city, Coord, CrossCorner);

    bool enough_resources(const std::map<Resource, size_t> &resources_per_infrastructure) const noexcept;
    bool in_field(Coord, RoadSide) const noexcept;
    bool in_field(Coord, CrossCorner) const noexcept;
    bool free_locality_spot(Coord, CrossCorner) const noexcept;

    static std::set< std::pair<Coord, RoadSide> > road_spots_around(Coord, RoadSide) noexcept;
    static std::set< std::pair<Coord, CrossCorner> > town_spots_around(Coord, RoadSide) noexcept;

    Road &road(const std::string &name) const;
    Town &town(const std::string &name) const;
    City &city(const std::string &name) const;

    const std::set< std::pair<Coord, RoadSide> > &valid_road_spots() const noexcept {return _road_spots;}
    const std::set< std::pair<Coord, CrossCorner> > &valid_town_spots() const noexcept {return _town_spots;}
    const std::set< std::pair<Coord, CrossCorner> > &valid_city_spots() const noexcept {return _city_spots;}

private:
    void _modify_road_spots(Coord used_spot_coord, RoadSide used_spot_road_side);
    void _modify_town_spots(Coord used_spot_coord, RoadSide used_spot_road_side);
    void _modify_town_spots(Coord used_spot_coord, CrossCorner used_spot_cross_corner);

    Field &_field;
    const Player &_player;

    std::set< std::pair<Coord, RoadSide> > _road_spots;
    std::set< std::pair<Coord, CrossCorner> > _town_spots;
    std::set< std::pair<Coord, CrossCorner> > _city_spots;
};

#endif // THIRDSTAGESUBENGINE_H
