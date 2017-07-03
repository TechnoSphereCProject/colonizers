#ifndef CITY_H
#define CITY_H

#include <Locality.h>


class City
    : public Locality
{
    City(const City &) = delete;
    City &operator= (const City &) = delete;

public:
    City(const string &name, Player &player) : Locality(name, player) {}

    bool is_town() const noexcept override {return false;}
    bool is_city() const noexcept override {return true;}
};

#endif // CITY_H
