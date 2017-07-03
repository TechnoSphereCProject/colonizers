#include "CrossCorner.h"

std::string EnumInfo::cross_corner_str(CrossCorner cc)
{
    switch ((int) cc) {
        case (int)CrossCorner::BOTTOM: return "BOTTOM";
        case (int)CrossCorner::TOP: return "TOP";
        default: return "UNKNOWN CROSSCORNER";
    }
}
