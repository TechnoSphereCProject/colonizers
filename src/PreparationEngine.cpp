#include "PreparationEngine.h"

#include <algorithm>

using Logger::log;
using Logger::warnif;
using std::invalid_argument;

bool PreparationEngine::player_is_waiting(const string &name) const noexcept
{
    return std::count(_waiting_queue.begin(), _waiting_queue.end(), name);
}

void PreparationEngine::join_player(const string &player)
{
    if (player_is_waiting(player)) {
        throw invalid_argument("cannot join player \"" + player +
            "\": player with this name is already waiting for a game");
    }
    _waiting_queue.push_back(player);

    log("player " + player + " joined the game");
    warnif(_waiting_queue.size() > NUM_PLAYERS_HIGHER_BOUND, "number of players exceeds game limit");
}

void PreparationEngine::unjoin_player(const string &player)
{
    if (!player_is_waiting(player)) {
        throw invalid_argument("cannot unjoin player \"" + player +
            "\": player with this name doesn't exist");
    }
    _waiting_queue.remove(player);

    log("player " + player + " left the game");
}

bool PreparationEngine::has_road(const Player &player, const string &name) const noexcept
{
    for (size_t i = 0; i < player.num_roads(); i++) {
        if (player.road(i).name() == name) {
            return true;
        }
    }
    return false;
}

bool PreparationEngine::has_town(const Player &player, const string &name) const noexcept
{
    for (size_t i = 0; i < player.num_towns(); i++) {
        if (player.town(i).name() == name) {
            return true;
        }
    }
    return false;
}

bool PreparationEngine::has_city(const Player &player, const string &name) const noexcept
{
    for (size_t i = 0; i < player.num_cities(); i++) {
        if (player.city(i).name() == name) {
            return true;
        }
    }
    return false;
}

GameStage PreparationEngine::register_road(Player &player, const string &name)
{
    if (has_road(player, name)) {
        throw invalid_argument("cannot register " + player.name() + "'s road \"" + name +
            "\": player already has such road");
    } else if (player.num_roads() >= MAX_ROADS) {
        throw invalid_argument("cannot register " + player.name() + "'s road \"" + name +
            "\": player already has all possible roads");
    }

    player.add_road(name);
    log(player.name() + " registered road " + name);
    return registration_is_over() ? GameStage::PUT_INITIAL_INFRASTRUCTURES : GameStage::INFRASTRUCTURES_REGISTRATION;
}

GameStage PreparationEngine::register_town(Player &player, const string &name)
{
    if (has_town(player, name)) {
        throw invalid_argument("cannot register " + player.name() + "'s town \"" + name +
            "\": player already has such town");
    } else if (player.num_towns() >= MAX_TOWNS) {
        throw invalid_argument("cannot register " + player.name() + "'s town \"" + name +
            "\": player already has all possible towns");
    }

    player.add_town(name);
    log(player.name() + " registered town " + name);
    return registration_is_over() ? GameStage::PUT_INITIAL_INFRASTRUCTURES : GameStage::INFRASTRUCTURES_REGISTRATION;
}

GameStage PreparationEngine::register_city(Player &player, const string &name)
{
    if (has_city(player, name)) {
        throw invalid_argument("cannot register " + player.name() + "'s city \"" + name +
            "\": player already has such city");
    } else if (player.num_cities() >= MAX_CITIES) {
        throw invalid_argument("cannot register " + player.name() + "'s city \"" + name +
            "\": player already has all possible cities");
    }

    player.add_city(name);
    log(player.name() + " registered city " + name);
    return registration_is_over() ? GameStage::PUT_INITIAL_INFRASTRUCTURES : GameStage::INFRASTRUCTURES_REGISTRATION;
}

void PreparationEngine::fix_players()
{
    if (_waiting_queue.size() < NUM_PLAYERS_LOWER_BOUND || _waiting_queue.size() > NUM_PLAYERS_HIGHER_BOUND) {
        throw invalid_argument("cannot register players: wrong players count");
    }

    for (auto &i: _waiting_queue) {
        _game.add_player(i);
    }
    _waiting_queue.clear();

    log("registration is over");
}

void PreparationEngine::init_bank() const
{
    ResourcesHolder &bank = _game.field().bank();

    bank.add(Resource::CLAY, RESOURCES_INITIAL_COUNT);
    bank.add(Resource::GRAIN, RESOURCES_INITIAL_COUNT);
    bank.add(Resource::ORE, RESOURCES_INITIAL_COUNT);
    bank.add(Resource::WOOD, RESOURCES_INITIAL_COUNT);
    bank.add(Resource::WOOL, RESOURCES_INITIAL_COUNT);

    log("game bank initialized");
}

void PreparationEngine::init_classical_field() const
{
    Field &field = _game.field();

    if (field.num_hexes() != 0) {
        throw std::logic_error("field cannot be initialized: it isn't empty");
    }
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            field.put_hex(Coord(i, j));
        }
    }
    for (int i = 0; i < 3; i++) {
        field.put_hex(Coord(-2, i));
        field.put_hex(Coord(2, -i));
    }
    for (int i = 0; i < 2; i++) {
        field.put_hex(Coord(i, -2));
        field.put_hex(Coord(-i, 2));
    }

    field.hex(Coord(-2, 2)).set_number(4);
    field.hex(Coord(-2, 2)).set_resource(Resource::GRAIN);

    field.hex(Coord(-2, 1)).set_number(2);
    field.hex(Coord(-2, 1)).set_resource(Resource::CLAY);

    field.hex(Coord(-2, 0)).set_number(9);
    field.hex(Coord(-2, 0)).set_resource(Resource::WOOL);

    field.hex(Coord(-1, 2)).set_number(6);
    field.hex(Coord(-1, 2)).set_resource(Resource::WOOD);

    field.hex(Coord(-1, 1)).set_number(5);
    field.hex(Coord(-1, 1)).set_resource(Resource::WOOD);

    field.hex(Coord(-1, 0)).set_number(8);
    field.hex(Coord(-1, 0)).set_resource(Resource::CLAY);

    field.hex(Coord(-1, -1)).set_number(3);
    field.hex(Coord(-1, -1)).set_resource(Resource::WOOD);

    field.hex(Coord(0, 2)).set_number(9);
    field.hex(Coord(0, 2)).set_resource(Resource::GRAIN);

    field.hex(Coord(0, 1)).set_number(12);
    field.hex(Coord(0, 1)).set_resource(Resource::WOOL);

    field.hex(Coord(0, -1)).set_number(5);
    field.hex(Coord(0, -1)).set_resource(Resource::ORE);

    field.hex(Coord(0, -2)).set_number(3);
    field.hex(Coord(0, -2)).set_resource(Resource::GRAIN);

    field.hex(Coord(1, 1)).set_number(4);
    field.hex(Coord(1, 1)).set_resource(Resource::WOOL);

    field.hex(Coord(1, 0)).set_number(8);
    field.hex(Coord(1, 0)).set_resource(Resource::ORE);

    field.hex(Coord(1, -1)).set_number(10);
    field.hex(Coord(1, -1)).set_resource(Resource::CLAY);

    field.hex(Coord(1, -2)).set_number(6);
    field.hex(Coord(1, -2)).set_resource(Resource::GRAIN);

    field.hex(Coord(2, 0)).set_number(10);
    field.hex(Coord(2, 0)).set_resource(Resource::WOOL);

    field.hex(Coord(2, -1)).set_number(11);
    field.hex(Coord(2, -1)).set_resource(Resource::WOOD);

    field.hex(Coord(2, -2)).set_number(11);
    field.hex(Coord(2, -2)).set_resource(Resource::ORE);

    field.set_robber(Coord(0, 0));

    log("classical field initialized");
}

bool PreparationEngine::registration_is_over() const noexcept
{
    for (size_t i = 0; i < _game.num_players(); i++) {
        if (_game.player(i).num_roads() < MAX_ROADS || _game.player(i).num_towns() < MAX_TOWNS ||
            _game.player(i).num_cities() < MAX_CITIES) {
            return false;
        }
    }
    return true;
}

GameStage PreparationEngine::put_initial_infrastructure(const Player &player,
        const std::string &town, Coord town_coord, CrossCorner town_corner,
        const std::string &road, Coord road_coord, RoadSide road_side)
{
    Town *town_ptr = nullptr;
    Road *road_ptr = nullptr;
    for (size_t i = 0; i < player.num_roads(); i++) {
        if (player.road(i).name() == road) {
            road_ptr = &player.road(i);
            break;
        }
    }
    if (road_ptr == nullptr) {
        throw invalid_argument("cannot put initial infrastructure: player " + player.name() +
            " hasn't road \"" + road + "\"");
    }
    for (size_t i = 0; i < player.num_towns(); i++) {
        if (player.town(i).name() == town) {
            town_ptr = &player.town(i);
            break;
        }
    }
    if (town_ptr == nullptr) {
        throw invalid_argument("cannot put initial infrastructure: player " + player.name() +
            " hasn't town \"" + town + "\"");
    }

    Field &field = _game.field();

    if (((town_corner == CrossCorner::BOTTOM) &&
            !( ((road_coord == town_coord) && (road_side == RoadSide::DOWN)) ||
            ((road_coord == town_coord.south_west()) && (road_side != RoadSide::DOWN)) ))
        ||
        ((town_corner == CrossCorner::TOP) &&
            !( ((road_coord == town_coord) && (road_side == RoadSide::UP)) ||
            ((road_coord == town_coord.north_west()) && (road_side != RoadSide::UP))) )   )
    {
        throw invalid_argument("cannot put initial infrastructure: initial infrastructures aren't adjacent");
    } else if (_game.player(_current_player).name() != player.name()) {
        throw invalid_argument("cannot put initial infrastructure: it is " + _game.player(_current_player).name() +
            "'s move, not " + player.name() + "'s");
    } else if (((town_corner == CrossCorner::BOTTOM) &&
            (field.has_locality(town_coord.south_east(), CrossCorner::TOP)
            || field.has_locality(town_coord.south_west(), CrossCorner::TOP)
            || field.has_locality(town_coord.south_east().south_west(), CrossCorner::TOP)))
        || ((town_corner == CrossCorner::TOP) &&
            (field.has_locality(town_coord.north_east(), CrossCorner::BOTTOM)
            || field.has_locality(town_coord.north_west(), CrossCorner::BOTTOM)
            || field.has_locality(town_coord.north_east().north_west(), CrossCorner::BOTTOM))) )
    {
        throw invalid_argument("cannot put initial infrastructure: you cannot put town near others player town");
    } else if (field.linked(*road_ptr) || field.linked(*town_ptr)) {
        throw invalid_argument("cannot put initial infrastructure: some of infrastructures is already put");
    } else if ( !(
        (field.has_hex(town_coord) ||
            ((town_corner == CrossCorner::BOTTOM) &&
                (field.has_hex(town_coord.south_east()) || field.has_hex(town_coord.south_west())))
            ||
            ((town_corner == CrossCorner::TOP) &&
                (field.has_hex(town_coord.north_east()) || field.has_hex(town_coord.north_west()))))
        &&
        (field.has_hex(road_coord) ||
            ((road_side == RoadSide::DOWN) && field.has_hex(road_coord.south_east())) ||
            ((road_side == RoadSide::RIGHT) && field.has_hex(road_coord.east())) ||
            ((road_side == RoadSide::UP) && field.has_hex(road_coord.north_east()))) )    )
    {
        throw invalid_argument("cannot put initial infrastructure: spot is out of field");
    } else if (field.has_road(road_coord, road_side) || field.has_locality(town_coord, town_corner)) {
        throw invalid_argument("cannot put initial infrastructure: spot is occupied");
    }

    field.link_locality(*town_ptr, town_coord, town_corner);
    field.link_road(*road_ptr, road_coord, road_side);

    log("player " + player.name() + " successfully put road " + road + " and town " + town);

    if (!_reverse && (_current_player == _game.num_players() - 1)) {
        _reverse = true;
        log("straight arrangement is over, starting reverse");
    } else if (_reverse) {
        std::vector<Coord> hex_with_resource;
        if (field.has_hex(town_coord) && field.hex(town_coord).has_resource()) {
            hex_with_resource.push_back(town_coord);
        }
        if (town_corner == CrossCorner::BOTTOM) {
            if (field.has_hex(town_coord.south_east()) && field.hex(town_coord.south_east()).has_resource()) {
                hex_with_resource.push_back(town_coord.south_east());
            }
            if (field.has_hex(town_coord.south_west()) && field.hex(town_coord.south_west()).has_resource()) {
                hex_with_resource.push_back(town_coord.south_west());
            }
        } else if (town_corner == CrossCorner::TOP) {
            if (field.has_hex(town_coord.north_east()) && field.hex(town_coord.north_east()).has_resource()) {
                hex_with_resource.push_back(town_coord.north_east());
            }
            if (field.has_hex(town_coord.north_west()) && field.hex(town_coord.north_west()).has_resource()) {
                hex_with_resource.push_back(town_coord.north_west());
            }
        }
        for (auto i: hex_with_resource) {
            Resource rec = field.hex(i).resource();
            player.bank().add(rec, RESOURCES_PORTION);
            field.bank().remove(rec, RESOURCES_PORTION);
            log(player.name() + " receives resources from initial infrastructure");
        }

        if (_current_player == 0) {
            return GameStage::STAGE1_DICE;
        } else {
            _current_player--;
        }
    } else {
        _current_player++;
    }

    return GameStage::PUT_INITIAL_INFRASTRUCTURES;
}
