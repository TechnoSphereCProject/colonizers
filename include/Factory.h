#ifndef FACTORY_H
#define FACTORY_H

#include <GameEngine.h>

namespace Factory
{

string make_road_name(const string &player, size_t number);
string make_town_name(const string &player, size_t number);
string make_city_name(const string &player, size_t number);

}

#endif // FACTORY_H
