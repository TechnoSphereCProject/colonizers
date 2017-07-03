#ifndef TOWN_H
#define TOWN_H

#include <Locality.h>


class Town
    : public Locality
{
    Town(const Town &) = delete;
    Town &operator= (const Town &) = delete;

public:
    Town(const string &name, Player &player) : Locality(name, player) {}

    bool is_town() const noexcept override {return true;}
    bool is_city() const noexcept override {return false;}
};

#endif // TOWN_H
