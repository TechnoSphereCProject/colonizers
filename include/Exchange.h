#ifndef EXCHANGE_H
#define EXCHANGE_H

#include <set>
#include <Resource.h>

class Player;

class Exchange
{
public:
    Exchange(const Player &_initiator, const Player &_other,
        const std::multiset<Resource> &_src, const std::multiset<Resource> &_target):
        initiator(_initiator), other(_other), src(_src), target(_target) {}

    bool performed = false;
    const Player &initiator, &other;
    const std::multiset<Resource> src, target;
};

#endif // EXCHANGE_H
