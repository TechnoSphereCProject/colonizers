#include "Player.h"

using std::logic_error;
using std::out_of_range;

Player::~Player ()
{
    for (auto &i: _roads) {
        delete i;
    }
    for (auto &i: _towns) {
        delete i;
    }
    for (auto &i: _cities) {
        delete i;
    }
    delete _bank;
}

Infrastructure &Player::infrastructure(size_t i) const
{
    if (i < _roads.size()) {
        return *_roads[i];
    } else if ((i -= _roads.size()) < _cities.size()) {
        return *_cities[i];
    } else if ((i -= _cities.size()) < _towns.size()) {
        return *_towns[i];
    }
    throw out_of_range(name() + "/Infrastructure &Player::infrastructure() OUT OF RANGE");
}

Road &Player::road(size_t i) const
{
    if (i >= _roads.size()) {
        throw out_of_range(name() + "/Road &Player::road() OUT OF RANGE");
    }
    return *_roads[i];
}

Town &Player::town(size_t i) const
{
    if (i >= _towns.size()) {
        throw out_of_range(name() + "/Town &Player::town() OUT OF RANGE");
    }
    return *_towns[i];
}

City &Player::city(size_t i) const
{
    if (i >= _cities.size()) {
        throw out_of_range(name() + "/City &Player::city() OUT OF RANGE");
    }
    return *_cities[i];
}
