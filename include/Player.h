#ifndef PLAYER_H
#define PLAYER_H

#include <Road.h>
#include <Town.h>
#include <City.h>
#include <ResourcesHolder.h>

#include <stdexcept>
#include <vector>
using std::string;


class Player
{
    Player(const Player &) = delete;
    Player &operator= (const Player &) = delete;

public:
    explicit Player(const string &name) : _name(name) {_bank = new ResourcesHolder();}
    ~Player();

    const string &name() const noexcept {return _name;}
    ResourcesHolder &bank() const noexcept {return *_bank;}

    Road &add_road(const string &name) {_roads.push_back(new Road(name, *this)); return *_roads.back();}
    City &add_city(const string &name) {_cities.push_back(new City(name, *this)); return *_cities.back();}
    Town &add_town(const string &name) {_towns.push_back(new Town(name, *this)); return *_towns.back();}

    size_t num_infrastructures() const noexcept {return _roads.size() + _towns.size() + _cities.size();}
    size_t num_roads() const noexcept {return _roads.size();}
    size_t num_towns() const noexcept {return _towns.size();}
    size_t num_cities() const noexcept {return _cities.size();}

    Infrastructure &infrastructure(size_t i) const;
    Road &road(size_t i) const;
    Town &town(size_t i) const;
    City &city(size_t i) const;

    void remove_road(Road &r) {remove_infrastructure(_roads, r);}
    void remove_city(City &c) {remove_infrastructure(_cities, c);}
    void remove_town(Town &t) {remove_infrastructure(_towns, t);}

private:
    template<class Collection>
    void remove_infrastructure(Collection &, Infrastructure &);

    const string _name;
    ResourcesHolder *_bank;

    std::vector<Road *> _roads;
    std::vector<Town *> _towns;
    std::vector<City *> _cities;
};

template<class Collection>
void Player::remove_infrastructure(Collection &collection, Infrastructure &inf)
{
    for (auto i = collection.begin(); i != collection.end(); i++) {
        if (&inf == *i) {
            delete *i;
            collection.erase(i);
            return;
        }
    }
    std::string type = inf.is_road() ? "road" : (inf.is_city() ? "city" : "town");
    throw std::logic_error(name() + "/cannot remove " + type + " " + inf.name() + " : player hasn't such " + type);
}


#endif // PLAYER_H
