#include "Factory.h"

string Factory::make_road_name(const string &player, size_t number)
{
    return player + "_road#" + (char)(number + '0');
}

string Factory::make_town_name(const string &player, size_t number)
{
    return player + "_town#" + (char)(number + '0');
}

string Factory::make_city_name(const string &player, size_t number)
{
    return player + "_city#" + (char)(number + '0');
}

