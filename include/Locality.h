#ifndef LOCALITY_H
#define LOCALITY_H

#include <Infrastructure.h>


class Locality
    : public Infrastructure
{
public:
    Locality(const std::string &name, Player &player) : Infrastructure(name, player) {}

    virtual bool is_town() const noexcept = 0;
    virtual bool is_city() const noexcept = 0;
    bool is_road() const noexcept override {return false;}
};

#endif // LOCALITY_H
