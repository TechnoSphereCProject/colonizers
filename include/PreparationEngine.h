#ifndef PREPARATIONENGINE_H
#define PREPARATIONENGINE_H

#include <stdexcept>
#include <list>
#include <Game.h>
#include <GameStage.h>
#include <Logger.h>
using std::string;

class PreparationEngine
{
    PreparationEngine(const PreparationEngine &) = delete;
    PreparationEngine &operator= (const PreparationEngine &) = delete;

public:
    enum {NUM_PLAYERS_LOWER_BOUND = 3, NUM_PLAYERS_HIGHER_BOUND = 4};
    enum {RESOURCES_INITIAL_COUNT = 19};
    enum {MAX_ROADS = 15, MAX_TOWNS = 5, MAX_CITIES = 4};
    enum {RESOURCES_PORTION = 1};

    explicit PreparationEngine(Game &g): _game(g) {}

    void join_player(const string &);
    void unjoin_player(const string &);

    bool player_is_waiting(const std::string &name) const noexcept;

    bool has_road(const Player &player, const string &name) const noexcept;
    bool has_town(const Player &player, const string &name) const noexcept;
    bool has_city(const Player &player, const string &name) const noexcept;

    GameStage register_road(Player &player, const string &name);
    GameStage register_town(Player &player, const string &name);
    GameStage register_city(Player &player, const string &name);

    GameStage put_initial_infrastructure(const Player &player,
        const std::string &town, Coord town_coord, CrossCorner town_corner,
        const std::string &road, Coord road_coord, RoadSide road_side);

    void fix_players();

    void init_bank() const;
    void init_classical_field() const;

    bool registration_is_over() const noexcept;

private:
    std::list<std::string> _waiting_queue;

    Game &_game;

    bool _reverse = false;
    size_t _current_player = 0;
};

#endif // PREPARATIONENGINE_H
