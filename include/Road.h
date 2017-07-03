#ifndef ROAD_H
#define ROAD_H

#include <Infrastructure.h>


class Road
    : public Infrastructure
{
    Road(const Road &) = delete;
    Road &operator= (const Road &) = delete;

public:
    Road(const string &name, Player &player) : Infrastructure(name, player) {}

    bool is_town() const noexcept override {return false;}
    bool is_city() const noexcept override {return false;}
    bool is_road() const noexcept override {return true;}
};

#endif // ROAD_H
