#include "RoadSide.h"

std::string EnumInfo::road_side_str(RoadSide rs)
{
    switch ((int) rs) {
        case (int)RoadSide::DOWN: return "DOWN";
        case (int)RoadSide::RIGHT: return "RIGHT";
        case (int)RoadSide::UP: return "UP";
        default: return "UNKNOWN ROADSIDE";
    }
}
