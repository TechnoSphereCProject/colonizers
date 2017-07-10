#ifndef FIRSTSTAGESUBENGINE_H
#define FIRSTSTAGESUBENGINE_H

#include <GameStage.h>
#include <Game.h>
#include <Logger.h>
#include <set>

class FirstStageSubEngine
{
    FirstStageSubEngine(const FirstStageSubEngine &) = delete;
    FirstStageSubEngine &operator= (const FirstStageSubEngine &) = delete;

public:
    enum {DICE_LOWER_BOUND = 2, DICE_HIGHER_BOUND = 12, ROBBERS_MOVE = 7};
    enum {RESOURCES_REQUIRED_TO_BE_ROBBED = 8};
    enum {SINGLE_DROP = 1};
    enum {SINGLE_ROBBERY = 1};
    enum {DEAL_PER_TOWN = 1, DEAL_PER_CITY = 2};

    explicit FirstStageSubEngine(const Game &g): _game(g) {}

    GameStage make_dice(size_t dice);
    GameStage drop_resource(const Player &player, Resource);
    GameStage move_robber(Coord) const;
    GameStage rob(const Player &player, const Player &victim) const;

    static std::set< std::pair<Coord, CrossCorner> > hex_corners(Coord);

private:
    void deal_resources(size_t dice) const;
    void init_drop_list();

    std::map<const Player *, size_t> _drop_list;
    const Game &_game;
};

#endif // FIRSTSTAGESUBENGINE_H
