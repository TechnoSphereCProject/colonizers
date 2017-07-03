#ifndef ROADSIDE_H
#define ROADSIDE_H

#include <string>

enum class RoadSide {UP, RIGHT, DOWN};

namespace EnumInfo
{
    std::string road_side_str(RoadSide rs);
}

#endif // ROADSIDE_H
