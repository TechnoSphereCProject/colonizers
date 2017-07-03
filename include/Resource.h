#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

enum class Resource {GRAIN, CLAY, ORE, WOOL, WOOD};

namespace EnumInfo
{
    std::string resource_str(Resource r);
}

#endif // RESOURCE_H
