#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <Game.h>
#include <GameStage.h>
#include <PreparationEngine.h>
#include <FirstStageSubEngine.h>
#include <SecondStageSubEngine.h>
#include <ThirdStageSubEngine.h>

//#include "format.h"
//#include "format.cc"

class GameEngine
{
    GameEngine(const GameEngine &) = delete;
    GameEngine &operator= (const GameEngine &) = delete;

public:
    enum {VICTORY_POINTS = 10};
    enum {POINTS_PER_TOWN = 1, POINTS_PER_CITY = 2};

    GameEngine() = default;
    ~GameEngine();

    //PREPARATION STAGE
    void join_player(const std::string &);
    void unjoin_player(const std::string &);

    void start_game();

    void register_road(const std::string &player, const std::string &name);
    void register_city(const std::string &player, const std::string &name);
    void register_town(const std::string &player, const std::string &name);

    void put_initial_infrastructure(const std::string &player,
        const std::string &town, Coord town_coord, CrossCorner town_corner,
        const std::string &road, Coord road_coord, RoadSide road_side);

    //STAGE 1
    void make_dice(const std::string &player, size_t dice);
    void drop_resource(const std::string &player, Resource);
    void move_robber(const std::string &player, Coord);
    void rob(const std::string &player, const std::string &victim);

    //STAGE 2
    void exchange_with_field(const std::string &player, Resource src, Resource target);
    int exchange_players_request(const std::string &player, const std::string &other_player,
        const std::multiset<Resource> &src, const std::multiset<Resource> &target);
    void exchange_players_accept(const std::string &player, int request);

    std::set<int> requests() const;
    std::set<int> exchanges() const;

    const Player &initiator(int request) const;
    const Player &other(int request) const;

    const std::multiset<Resource> &src(int request) const;
    const std::multiset<Resource> &target(int request) const;

    bool requested(int number) const;
    bool performed(int request) const;

    void end_exchanges(const std::string &player);

    //STAGE 3
    void build_road(const std::string &player, const std::string &road, Coord, RoadSide);
    void build_town(const std::string &player, const std::string &town, Coord, CrossCorner);
    void build_city(const std::string &player, const std::string &city, Coord, CrossCorner);

    const std::set< std::pair<Coord, RoadSide> > &valid_road_spots(const string &player) const;
    const std::set< std::pair<Coord, CrossCorner> > &valid_town_spots(const string &player) const;
    const std::set< std::pair<Coord, CrossCorner> > &valid_city_spots(const string &player) const;

    void next_player(const std::string &player);

    //OTHER
    const Game &game() const noexcept {return *_game;}

    const Player &current_player() const;
    size_t player_number(const std::string &player_name) const;

    int score(const std::string &player);
    GameStage stage() const noexcept {return _game_stage;}

    const Player &winner() const;

private:
    GameStage _game_stage = GameStage::PLAYERS_REGISTRATION;
    Game *_game = new Game();
    size_t _current_player = 0;

    PreparationEngine *_preparation_stage = new PreparationEngine(*_game);
    FirstStageSubEngine *_first_stage = nullptr;
    SecondStageSubEngine *_second_stage = nullptr;
    ThirdStageSubEngine *_third_stage = nullptr;
};

#endif // GAMEENGINE_H

