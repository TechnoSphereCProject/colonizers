#ifndef INFRASTRUCTURE_H
#define INFRASTRUCTURE_H

#include <string>
using std::string;

class Player;

class Infrastructure
{
public:
    Infrastructure(const string &name, Player &player) : _name(name), _player(player) {}
    virtual ~Infrastructure() {}

    Player &player() const noexcept {return _player;}
    const string &name() const noexcept {return _name;}

    virtual bool is_town() const noexcept = 0;
    virtual bool is_city() const noexcept = 0;
    virtual bool is_road() const noexcept = 0;

private:
    const string _name;
    Player &_player;
};

#endif // INFRASTRUCTURE_H
