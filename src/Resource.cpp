#include "Resource.h"

std::string EnumInfo::resource_str(Resource r) {
    switch ((int) r) {
        case (int)Resource::CLAY: return "CLAY";
        case (int)Resource::WOOD: return "WOOD";
        case (int)Resource::WOOL: return "WOOL";
        case (int)Resource::ORE: return "ORE";
        case (int)Resource::GRAIN: return "GRAIN";
        default: return "UNKNOWN RESOURCE";
    }
}
