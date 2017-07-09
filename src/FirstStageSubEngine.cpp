#include "FirstStageSubEngine.h"
#include <exception>
#include <sstream>
#include <cstdlib>
#include <ctime>

#include "format.h"
//#include "format.cc"

using Logger::log;

using std::invalid_argument;

GameStage FirstStageSubEngine::make_dice(size_t dice)
{
    if ((dice < DICE_LOWER_BOUND) || (dice > DICE_HIGHER_BOUND)) {
        throw invalid_argument(fmt::format("cannot make dice: possible from {0} up to {1}", DICE_LOWER_BOUND, DICE_HIGHER_BOUND));
        
    } else if (dice == ROBBERS_MOVE) {
        log("robbers dice: starting robbing procedure");
        init_drop_list();
        return _drop_list.empty() ? GameStage::STAGE1_MOVE_ROBBER : GameStage::STAGE1_DROP_RESOURCES;
    } else {
        log("regular dice: dealing resources");
        deal_resources(dice);
        return GameStage::STAGE2;
    }
}

GameStage FirstStageSubEngine::drop_resource(const Player &player, Resource res)
{
    if (player.bank().resources(res) < SINGLE_DROP) {
        throw invalid_argument(fmt::format("cannot drop resource: player {0} hasn't enough {1} to drop", player.name(), EnumInfo::resource_str(res)));
    } else if (_drop_list.count(&player) == 0) {
        throw invalid_argument(fmt::format("cannot drop resource: player {0} doesn't have to drop resource", player.name()));
    } else if (_drop_list.find(&player)->second == 0) {
        throw invalid_argument(fmt::format("cannot drop resource: player {0} already dropped required resources count", player.name()));
    }

    player.bank().remove(res, SINGLE_DROP);
    _drop_list[&player] -= SINGLE_DROP;
    _game.field().bank().add(res, SINGLE_DROP);

    for (auto it = _drop_list.cbegin(); it != _drop_list.cend(); it++) {
        if (it->second != 0) {
            return GameStage::STAGE1_DROP_RESOURCES;
        }
    }
    return GameStage::STAGE1_MOVE_ROBBER;
}

GameStage FirstStageSubEngine::move_robber(Coord xy) const
{
    if (_game.field().has_robber() && (_game.field().robber() == xy)) {
        throw invalid_argument("cannot move robber: robber has to be moved to other place");
    } else if (!_game.field().has_hex(xy)) {
        throw invalid_argument(fmt::format("cannot move robber: spot {} is out of field", xy.str()));
    }

    _game.field().set_robber(xy);
    log(fmt::format("moving robber to {}", xy.str()));

    auto hex_corners = this->hex_corners(_game.field().robber());
    for (auto i: hex_corners) {
        if (_game.field().has_locality(i.first, i.second)) {
            return GameStage::STAGE1_ROBBING;
        }
    }
    return GameStage::STAGE2;
}

GameStage FirstStageSubEngine::rob(const Player &player, const Player &victim) const
{
    Field &field = _game.field();
    auto hex_corners = this->hex_corners(field.robber());
    bool has_locality_near_robber = false;

    for (auto i: hex_corners) {
        if (field.has_locality(i.first, i.second) &&
            (field.locality(i.first, i.second).player().name() == victim.name()))
        {
            has_locality_near_robber = true;
            break;
        }
    }

    if (!has_locality_near_robber) {
        throw invalid_argument(fmt::format("cannot rob: victim player {} hasn't any locality near robber", victim.name()));
    } else if (victim.bank().resources() != 0) {
        std::vector<Resource> res = {Resource::CLAY, Resource::GRAIN, Resource::ORE, Resource::WOOD, Resource::WOOL};
        Resource rob_res;

        std::srand(std::time(0));
        do {
            rob_res = res[std::rand() % res.size()];
        } while (victim.bank().resources(rob_res) == 0);

        player.bank().add(rob_res, SINGLE_ROBBERY);
        victim.bank().remove(rob_res, SINGLE_ROBBERY);
        log(fmt::format("{0} robbed {1}", player.name(), victim.name()));
    }
    return GameStage::STAGE2;
}

void FirstStageSubEngine::init_drop_list()
{
    if (!_drop_list.empty()) {
        throw std::logic_error("cannot init drop list: it isn't empty");
    }

    for (size_t i = 0; i < _game.num_players(); i++) {
        if (_game.player(i).bank().resources() >= RESOURCES_REQUIRED_TO_BE_ROBBED) {
            _drop_list.insert( {&_game.player(i), _game.player(i).bank().resources() / 2} );
        }
    }
}

void FirstStageSubEngine::deal_resources(size_t dice) const
{
    std::vector<Coord> dice_hexes;
    for (size_t i = 0; i < _game.field().num_hexes(); i++) {
        Coord hex_coord = _game.field().coord(_game.field().hex(i));
        if (_game.field().hex(i).has_number() && _game.field().hex(i).has_resource() &&
            (_game.field().robber() != hex_coord) && (_game.field().hex(i).number() == dice))
        {
            dice_hexes.push_back(hex_coord);
        }
    }

    for (size_t num_player = 0; num_player < _game.num_players(); num_player++) {
        for (size_t num_hex = 0; num_hex < dice_hexes.size(); num_hex++) {
            if (_game.field().bank().resources() < DEAL_PER_TOWN) {
                return;
            }

            auto hex_corners = this->hex_corners(dice_hexes[num_hex]);
            Player &player = _game.player(num_player);
            Resource res = _game.field().hex(dice_hexes[num_hex]).resource();

            for (size_t num_town = 0; num_town < player.num_towns(); num_town++) {
                if (_game.field().linked(player.town(num_town)) &&
                    hex_corners.count(_game.field().coord(player.town(num_town))) &&
                    (_game.field().bank().resources(res) >= DEAL_PER_TOWN))
                {
                    player.bank().add(res, DEAL_PER_TOWN);
                    _game.field().bank().remove(res, DEAL_PER_TOWN);
                }
            }
            for (size_t num_city = 0; num_city < player.num_cities(); num_city++) {
                if (_game.field().linked(player.city(num_city)) &&
                    hex_corners.count(_game.field().coord(player.city(num_city))) &&
                    (_game.field().bank().resources(res) >= DEAL_PER_CITY))
                {
                    player.bank().add(res, DEAL_PER_CITY);
                    _game.field().bank().remove(res, DEAL_PER_CITY);
                }
            }
        }
    }
}

std::set< std::pair<Coord, CrossCorner> > FirstStageSubEngine::hex_corners(Coord xy)
{
    std::set< std::pair<Coord, CrossCorner> > _hex_corners;

    _hex_corners.insert( {xy, CrossCorner::BOTTOM} );
    _hex_corners.insert( {xy, CrossCorner::TOP} );
    _hex_corners.insert( {xy.north_east(), CrossCorner::BOTTOM} );
    _hex_corners.insert( {xy.north_west(), CrossCorner::BOTTOM} );
    _hex_corners.insert( {xy.south_east(), CrossCorner::TOP} );
    _hex_corners.insert( {xy.south_west(), CrossCorner::TOP} );

    return _hex_corners;
}
