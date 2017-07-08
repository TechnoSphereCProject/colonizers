#include <sstream>
#include "GameEngine.h"

using Logger::log;
using std::invalid_argument;

GameEngine::~GameEngine()
{
    delete _preparation_stage;
    delete _first_stage;
    delete _second_stage;
    delete _third_stage;
    delete _game;
}

//---------------------------------------------------------------------
//------------------------ PREPARATION --------------------------------
//---------------------------------------------------------------------

void GameEngine::join_player(const std::string &player)
{
    if (_game_stage != GameStage::PLAYERS_REGISTRATION) {
        throw invalid_argument("cannot join player \"" + player + "\": game is on " +
            EnumInfo::game_stage_str(_game_stage));
    }
    _preparation_stage->join_player(player);
}

void GameEngine::unjoin_player(const std::string &player)
{
    if (_game_stage != GameStage::PLAYERS_REGISTRATION) {
        throw invalid_argument("cannot unjoin player \"" + player + "\": game is on " +
            EnumInfo::game_stage_str(_game_stage));
    }
    _preparation_stage->unjoin_player(player);
}

void GameEngine::start_game()
{
    if (_game_stage != GameStage::PLAYERS_REGISTRATION) {
        throw invalid_argument("cannot start game: game is on " + EnumInfo::game_stage_str(_game_stage));
    }

    _preparation_stage->fix_players();
    _preparation_stage->init_bank();
    _preparation_stage->init_classical_field();

    _game_stage = GameStage::INFRASTRUCTURES_REGISTRATION;

    log("starting game");
}

void GameEngine::register_road(const std::string &player, const std::string &name)
{
    if (_game_stage != GameStage::INFRASTRUCTURES_REGISTRATION) {
        throw invalid_argument("cannot register " + player + "'s road \"" + name +
            "\": game is on " + EnumInfo::game_stage_str(_game_stage));
    }
    Player *player_ptr = nullptr;
    try {
        player_ptr = &_game->player(player_number(player));
    } catch (invalid_argument &except) {
        throw invalid_argument("cannot register " + player + "'s road \"" + name + "\" player doesn't exist");
    }

    _game_stage = _preparation_stage->register_road(*player_ptr, name);
}

void GameEngine::register_town(const std::string &player, const std::string &name)
{
    if (_game_stage != GameStage::INFRASTRUCTURES_REGISTRATION) {
        throw invalid_argument("cannot register " + player + "'s town \"" + name +
            "\": game is on " + EnumInfo::game_stage_str(_game_stage));
    }
    Player *player_ptr = nullptr;
    try {
        player_ptr = &_game->player(player_number(player));
    } catch (invalid_argument &except) {
        throw invalid_argument("cannot register " + player + "'s town \"" + name + "\" player doesn't exist");
    }

    _game_stage = _preparation_stage->register_town(*player_ptr, name);
}

void GameEngine::register_city(const std::string &player, const std::string &name)
{
    if (_game_stage != GameStage::INFRASTRUCTURES_REGISTRATION) {
        throw invalid_argument("cannot register " + player + "'s city \"" + name +
            "\": game is on " + EnumInfo::game_stage_str(_game_stage));
    }
    Player *player_ptr = nullptr;
    try {
        player_ptr = &_game->player(player_number(player));
    } catch (invalid_argument &except) {
        throw invalid_argument("cannot register " + player + "'s city \"" + name + "\" player doesn't exist");
    }

    _game_stage = _preparation_stage->register_city(*player_ptr, name);
}

void GameEngine::put_initial_infrastructure(const std::string &player,
        const std::string &town, Coord town_coord, CrossCorner town_corner,
        const std::string &road, Coord road_coord, RoadSide road_side)
{
    if (_game_stage != GameStage::PUT_INITIAL_INFRASTRUCTURES) {
        throw invalid_argument("cannot put initial infrastructure: game is on " +
            EnumInfo::game_stage_str(_game_stage));
    }

    Player *player_ptr = nullptr;
    try {
        player_ptr = &_game->player(player_number(player));
    } catch (invalid_argument &except) {
        throw invalid_argument("cannot put initial infrastructure: " + string(except.what()));
    }

    _game_stage = _preparation_stage->put_initial_infrastructure(*player_ptr, town, town_coord, town_corner,
        road, road_coord, road_side);

    if (_game_stage == GameStage::STAGE1_DICE) {
        _first_stage = new FirstStageSubEngine(*_game);
        delete _preparation_stage;
        _preparation_stage = nullptr;
        log("moving onto stage 1");
    }
}

//---------------------------------------------------------------------
//------------------------ STAGE 1 ------------------------------------
//---------------------------------------------------------------------


void GameEngine::make_dice(const std::string &player, size_t dice)
{
    if (_game_stage != GameStage::STAGE1_DICE) {
        throw invalid_argument("cannot make dice: game is on " + EnumInfo::game_stage_str(_game_stage));
    } else if (current_player().name() != player) {
        throw invalid_argument("cannot make dice: it is " +
            current_player().name() + "'s move, not " + player + "'s");
    }

    _game_stage = _first_stage->make_dice(dice);

    if (_game_stage == GameStage::STAGE2) {
        _second_stage = new SecondStageSubEngine(_game->field().bank(), current_player());
        delete _first_stage;
        _first_stage = nullptr;
        log("moving onto stage 2");
    }
}

void GameEngine::drop_resource(const std::string &player, Resource res)
{
    if (_game_stage != GameStage::STAGE1_DROP_RESOURCES) {
        throw invalid_argument("cannot drop resource: game is on " + EnumInfo::game_stage_str(_game_stage));
    }

    size_t num;
    try {
        num = player_number(player);
    } catch (invalid_argument &except) {
        throw invalid_argument("cannot drop resource: " + string(except.what()));
    }

    _game_stage = _first_stage->drop_resource(_game->player(num), res);
}

void GameEngine::move_robber(const std::string &player, Coord xy)
{
    if (_game_stage != GameStage::STAGE1_MOVE_ROBBER) {
        throw invalid_argument("cannot move robber: game is on " + EnumInfo::game_stage_str(_game_stage));
    } else if (current_player().name() != player) {
        throw invalid_argument("cannot move robber: player " + player + " isn't current");
    }

    _game_stage = _first_stage->move_robber(xy);
    if (_game_stage == GameStage::STAGE2) {
        _second_stage = new SecondStageSubEngine(_game->field().bank(), current_player());
        delete _first_stage;
        _first_stage = nullptr;
        log("moving onto stage 2");
    }
}

void GameEngine::rob(const std::string &player, const std::string &victim)
{
    if (_game_stage != GameStage::STAGE1_ROBBING) {
        throw invalid_argument("cannot rob: game is on " + EnumInfo::game_stage_str(_game_stage));
    } else if (current_player().name() != player) {
        throw invalid_argument("cannot rob: it is " + current_player().name() + "'s move, not " + player + "'s");
    }
    size_t num_victim;
    try {
        num_victim = player_number(victim);
    } catch (invalid_argument &except) {
        throw invalid_argument("cannot rob: " + string(except.what()));
    }

    _game_stage = _first_stage->rob(current_player(), _game->player(num_victim));

    _second_stage = new SecondStageSubEngine(_game->field().bank(), current_player());
    delete _first_stage;
    _first_stage = nullptr;
    log("moving onto stage 2");
}

//---------------------------------------------------------------------
//------------------------ STAGE 2 ------------------------------------
//---------------------------------------------------------------------


void GameEngine::exchange_with_field(const std::string &player, Resource src, Resource target)
{
    if (_game_stage != GameStage::STAGE2) {
        throw invalid_argument("cannot exchange with field: game is on " + EnumInfo::game_stage_str(_game_stage));
    } else if (current_player().name() != player) {
        throw invalid_argument("cannot exchange with field: it is " +
            current_player().name() + "'s move, not " + player + "'s");
    }

    _second_stage->exchange_with_field(src, target);
}

int GameEngine::exchange_players_request(const std::string &player, const std::string &other_player,
    const std::multiset<Resource> &src, const std::multiset<Resource> &target)
{
    if (_game_stage != GameStage::STAGE2) {
        throw invalid_argument("cannot register exchange: game is on " + EnumInfo::game_stage_str(_game_stage));
    } else if (current_player().name() != player) {
        throw invalid_argument("cannot register exchange: it is " + current_player().name() +
            "'s move, not " + player + "'s");
    }
    Player *other_player_ptr = nullptr;
    try {
        other_player_ptr = &_game->player(player_number(other_player));
    } catch (invalid_argument &except) {
        throw invalid_argument("cannot register exchange: " + string(except.what()));
    }
    if (current_player().name() == other_player_ptr->name()) {
        throw invalid_argument("cannot register exchange: self-exchanges are forbidden");
    }

    return _second_stage->exchange_players_request(*other_player_ptr, src, target);
}

void GameEngine::exchange_players_accept(const std::string &player, int request)
{
    if (_game_stage != GameStage::STAGE2) {
        throw invalid_argument("cannot accept exchange: game is on " + EnumInfo::game_stage_str(_game_stage));
    }
    Player *player_ptr = nullptr;
    try {
        player_ptr = &_game->player(player_number(player));
    } catch (invalid_argument &except) {
        throw invalid_argument("cannot accept exchange: " + string(except.what()));
    }
    if (current_player().name() == player_ptr->name()) {
        throw invalid_argument("cannot accept exchange: wrong player \"" + player + "\"");
    }

    _second_stage->exchange_players_accept(*player_ptr, request);
}

std::set<int> GameEngine::requests() const
{
    if (_game_stage != GameStage::STAGE2) {
        throw invalid_argument("cannot get exchange requests: game is on " + EnumInfo::game_stage_str(_game_stage));
    }
    return _second_stage->requests();
}

std::set<int> GameEngine::exchanges() const
{
    if (_game_stage != GameStage::STAGE2) {
        throw invalid_argument("cannot get exchanges: game is on " + EnumInfo::game_stage_str(_game_stage));
    }
    return _second_stage->exchanges();
}

const Player &GameEngine::initiator(int request) const
{
    if (_game_stage != GameStage::STAGE2) {
        throw invalid_argument("cannot get exchange initiator: game is on " + EnumInfo::game_stage_str(_game_stage));
    }
    return _second_stage->initiator(request);
}

const Player &GameEngine::other(int request) const
{
    if (_game_stage != GameStage::STAGE2) {
        throw invalid_argument("cannot get exchange other player: game is on "+EnumInfo::game_stage_str(_game_stage));
    }
    return _second_stage->other(request);
}

const std::multiset<Resource> &GameEngine::src(int request) const
{
    if (_game_stage != GameStage::STAGE2) {
        throw invalid_argument("cannot get exchange src set: game is on " + EnumInfo::game_stage_str(_game_stage));
    }
    return _second_stage->src(request);
}

const std::multiset<Resource> &GameEngine::target(int request) const
{
    if (_game_stage != GameStage::STAGE2) {
        throw invalid_argument("cannot get exchange target set: game is on " + EnumInfo::game_stage_str(_game_stage));
    }
    return _second_stage->target(request);
}

bool GameEngine::requested(int number) const
{
    if (_game_stage != GameStage::STAGE2) {
        throw invalid_argument("cannot identify request: game is on " + EnumInfo::game_stage_str(_game_stage));
    }
    return _second_stage->requested(number);
}

bool GameEngine::performed(int request) const
{
    if (_game_stage != GameStage::STAGE2) {
        throw invalid_argument("cannot identify request: game is on " + EnumInfo::game_stage_str(_game_stage));
    }
    return _second_stage->performed(request);
}

void GameEngine::end_exchanges(const std::string &player)
{
    if (_game_stage != GameStage::STAGE2) {
        throw invalid_argument("cannot end exchanges: game is on " + EnumInfo::game_stage_str(_game_stage));
    } else if (current_player().name() != player) {
        throw invalid_argument("cannot end exchanges: it is " + current_player().name() +
            "'s move, not " + player + "'s");
    }

    _third_stage = new ThirdStageSubEngine(_game->field(), current_player());
    _game_stage = GameStage::STAGE3;
    delete _second_stage;
    _second_stage = nullptr;
    log("moving onto stage 3");
}

//---------------------------------------------------------------------
//------------------------ STAGE 3 ------------------------------------
//---------------------------------------------------------------------

void GameEngine::build_road(const std::string &player, const std::string &road, Coord xy, RoadSide road_side)
{
    if (_game_stage != GameStage::STAGE3) {
        throw invalid_argument("cannot build: game is on " + EnumInfo::game_stage_str(_game_stage));
    } else if (current_player().name() != player) {
        throw invalid_argument("cannot build: it is " + current_player().name() +
            "'s move, not " + player + "'s");
    }

    _third_stage->build_road(_third_stage->road(road), xy, road_side);
}

void GameEngine::build_town(const std::string &player, const std::string &town, Coord xy, CrossCorner cross_corner)
{
    if (_game_stage != GameStage::STAGE3) {
        throw invalid_argument("cannot build: game is on " + EnumInfo::game_stage_str(_game_stage));
    } else if (current_player().name() != player) {
        throw invalid_argument("cannot build: it is " + current_player().name() +
            "'s move, not " + player + "'s");
    }

    _third_stage->build_town(_third_stage->town(town), xy, cross_corner);

    if (score(player) >= VICTORY_POINTS) {
        delete _third_stage;
        _third_stage = nullptr;
        _game_stage = GameStage::FINAL_STAGE;
        log("game over");
    }
}

void GameEngine::build_city(const std::string &player, const std::string &city, Coord xy, CrossCorner cross_corner)
{
    if (_game_stage != GameStage::STAGE3) {
        throw invalid_argument("cannot build: game is on " + EnumInfo::game_stage_str(_game_stage));
    } else if (current_player().name() != player) {
        throw invalid_argument("cannot build: it is " + current_player().name() +
            "'s move, not " + player + "'s");
    }

    _third_stage->build_city(_third_stage->city(city), xy, cross_corner);

    if (score(player) >= VICTORY_POINTS) {
        delete _third_stage;
        _third_stage = nullptr;
        _game_stage = GameStage::FINAL_STAGE;
        log("game over");
    }
}

const std::set< std::pair<Coord, RoadSide> > &GameEngine::valid_road_spots(const string &player) const
{
    if (_game_stage != GameStage::STAGE3) {
        throw invalid_argument("cannot get valid road spots: game is on " + EnumInfo::game_stage_str(_game_stage));
    } else if (current_player().name() != player) {
        throw invalid_argument("cannot get valid road spots: it is " + current_player().name() +
            "'s move, not " + player + "'s");
    }
    return _third_stage->valid_road_spots();
}

const std::set< std::pair<Coord, CrossCorner> > &GameEngine::valid_town_spots(const string &player) const
{
    if (_game_stage != GameStage::STAGE3) {
        throw invalid_argument("cannot get valid town spots: game is on " + EnumInfo::game_stage_str(_game_stage));
    } else if (current_player().name() != player) {
        throw invalid_argument("cannot get valid town spots: it is " + current_player().name() +
            "'s move, not " + player + "'s");
    }
    return _third_stage->valid_town_spots();
}

const std::set< std::pair<Coord, CrossCorner> > &GameEngine::valid_city_spots(const string &player) const
{
    if (_game_stage != GameStage::STAGE3) {
        throw invalid_argument("cannot get valid city spots: game is on " + EnumInfo::game_stage_str(_game_stage));
    } else if (current_player().name() != player) {
        throw invalid_argument("cannot get valid city spots: it is " + current_player().name() +
            "'s move, not " + player + "'s");
    }
    return _third_stage->valid_city_spots();
}

void GameEngine::next_player(const std::string &player)
{
    if (_game_stage != GameStage::STAGE3) {
        throw invalid_argument("cannot switch player: game is on " + EnumInfo::game_stage_str(_game_stage));
    } else if (current_player().name() != player) {
        throw invalid_argument("cannot switch player: it is " + current_player().name() +
            "'s move, not " + player + "'s");
    }

    _first_stage = new FirstStageSubEngine(game());
    delete _third_stage;
    _third_stage = nullptr;

    _game_stage = GameStage::STAGE1_DICE;

    _current_player = (_current_player + 1) % game().num_players();
    log("switching to the next player: " + current_player().name());
}

//---------------------------------------------------------------------
//------------------------ OTHER --------------------------------------
//---------------------------------------------------------------------
const Player &GameEngine::current_player() const
{
    if ((_game_stage == GameStage::PLAYERS_REGISTRATION) || (_game_stage == GameStage::FINAL_STAGE)) {
        throw invalid_argument("cannot get current player: game is on " + EnumInfo::game_stage_str(_game_stage));
    }
    return _game->player(_current_player);
}

size_t GameEngine::player_number(const string &player_name) const
{
    if (_game_stage == GameStage::PLAYERS_REGISTRATION) {
        throw invalid_argument("cannot get players number: game is on " + EnumInfo::game_stage_str(_game_stage));
    }

    for (size_t i = 0; i < _game->num_players(); i++) {
        if (_game->player(i).name() == player_name) {
            return i;
        }
    }
    throw invalid_argument("player \"" + player_name + "\" doesn't exist");
}

int GameEngine::score(const std::string &player)
{
    if (_game_stage == GameStage::PLAYERS_REGISTRATION) {
        throw invalid_argument("cannot get score: game is on " + EnumInfo::game_stage_str(_game_stage));
    }
    Player *player_ptr = nullptr;
    try {
        player_ptr = &_game->player(player_number(player));
    } catch (invalid_argument &except) {
        throw invalid_argument("cannot get score: " + string(except.what()));
    }

    int points = 0;

    for (size_t i = 0; i < player_ptr->num_towns(); i++) {
        if (_game->field().linked(player_ptr->town(i))) {
            points += POINTS_PER_TOWN;
        }
    }
    for (size_t i = 0; i < player_ptr->num_cities(); i++) {
        if (_game->field().linked(player_ptr->city(i))) {
            points += POINTS_PER_CITY;
        }
    }
    return points;
}

const Player &GameEngine::winner() const
{
    if (_game_stage != GameStage::FINAL_STAGE) {
        throw invalid_argument("cannot get winner: game is on " + EnumInfo::game_stage_str(_game_stage));
    }

    return _game->player(_current_player);
}
