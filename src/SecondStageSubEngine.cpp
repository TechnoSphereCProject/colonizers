#include "SecondStageSubEngine.h"
#include <exception>
#include <algorithm>
using std::invalid_argument;

void SecondStageSubEngine::exchange_with_field(Resource src, Resource target)
{
    if (_player.bank().resources(src) < FIELD_EXCHANGE_SRC) {
        throw invalid_argument("cannot exchange with field: player " + _player.name() +
            " hasn't enough " + EnumInfo::resource_str(src));
    } else if (_game_bank.resources(target) < FIELD_EXCHANGE_TARGET) {
        throw invalid_argument("cannot exchange with field: game bank hasn't enough " + EnumInfo::resource_str(target));
    }

    _player.bank().remove(src, FIELD_EXCHANGE_SRC);
    _game_bank.remove(target, FIELD_EXCHANGE_TARGET);

    _game_bank.add(src, FIELD_EXCHANGE_SRC);
    _player.bank().add(target, FIELD_EXCHANGE_TARGET);
}

int SecondStageSubEngine::exchange_players_request(const Player &other_player,
    const std::multiset<Resource> &src, const std::multiset<Resource> &target)
{
    if (src.empty() || target.empty()) {
        throw invalid_argument("cannot register exchange: one of the request sets is empty");
    } else if (count_res(src, Resource::CLAY) > MAX_RESOURCES || count_res(target, Resource::CLAY) > MAX_RESOURCES ||
        count_res(src, Resource::GRAIN) > MAX_RESOURCES || count_res(target, Resource::GRAIN) > MAX_RESOURCES ||
        count_res(src, Resource::ORE) > MAX_RESOURCES || count_res(target, Resource::ORE) > MAX_RESOURCES ||
        count_res(src, Resource::WOOD) > MAX_RESOURCES || count_res(target, Resource::WOOD) > MAX_RESOURCES ||
        count_res(src, Resource::WOOL) > MAX_RESOURCES || count_res(target, Resource::WOOL) > MAX_RESOURCES)
    {
        throw invalid_argument("cannot register exchange: request size exceeds game resources count");
    } else if (count_res(src, Resource::CLAY) > _player.bank().resources(Resource::CLAY) ||
        count_res(src, Resource::GRAIN) > _player.bank().resources(Resource::GRAIN) ||
        count_res(src, Resource::ORE) > _player.bank().resources(Resource::ORE) ||
        count_res(src, Resource::WOOD) > _player.bank().resources(Resource::WOOD) ||
        count_res(src, Resource::WOOL) > _player.bank().resources(Resource::WOOL) )
    {
        throw invalid_argument("cannot register exchange: player " + _player.name() + " hasn't enough resources");
    }

    _exchange_list.push_back(Exchange(_player, other_player, src, target));
    return _exchange_list.size() - 1;
}

void SecondStageSubEngine::exchange_players_accept(const Player &player, int request)
{
    if (!requested(request)) {
        throw invalid_argument("cannot accept exchange: request is incorrect");
    } else if (performed(request)) {
        throw invalid_argument("cannot accept exchange: this exchange already performed");
    } else if (other(request).name() != player.name()) {
        throw invalid_argument("cannot accept exchange: request applies to " + _exchange_list[request].other.name() +
            ", not to " + player.name());
    } else if (count_res(target(request), Resource::CLAY) > player.bank().resources(Resource::CLAY) ||
        count_res(target(request), Resource::GRAIN) > player.bank().resources(Resource::GRAIN) ||
        count_res(target(request), Resource::ORE) > player.bank().resources(Resource::ORE) ||
        count_res(target(request), Resource::WOOD) > player.bank().resources(Resource::WOOD) ||
        count_res(target(request), Resource::WOOL) > player.bank().resources(Resource::WOOL)  ||

        count_res(src(request), Resource::CLAY) > initiator(request).bank().resources(Resource::CLAY) ||
        count_res(src(request), Resource::GRAIN) > initiator(request).bank().resources(Resource::GRAIN) ||
        count_res(src(request), Resource::ORE) > initiator(request).bank().resources(Resource::ORE) ||
        count_res(src(request), Resource::WOOD) > initiator(request).bank().resources(Resource::WOOD) ||
        count_res(src(request), Resource::WOOL) > initiator(request).bank().resources(Resource::WOOL))
    {
        throw invalid_argument("cannot accept exchange: " + initiator(request).name() + " or "
            + player.name() +  " hasn't enough resources");
    }

    for (Resource i: _exchange_list[request].src) {
        initiator(request).bank().remove(i, 1);
        player.bank().add(i, 1);
    }
    for (Resource i: _exchange_list[request].target) {
        initiator(request).bank().add(i, 1);
        player.bank().remove(i, 1);
    }
    _exchange_list[request].performed = true;
}

std::set<int> SecondStageSubEngine::requests() const noexcept
{
    std::set<int> tmp;

    for (size_t i = 0; i < _exchange_list.size(); i++) {
        if (! _exchange_list[i].performed) {
            tmp.insert(i);
        }
    }
    return tmp;
}

std::set<int> SecondStageSubEngine::exchanges() const noexcept
{
    std::set<int> tmp;

    for (size_t i = 0; i < _exchange_list.size(); i++) {
        if (_exchange_list[i].performed) {
            tmp.insert(i);
        }
    }
    return tmp;
}

const Player &SecondStageSubEngine::initiator(int request) const
{
    if (!requested(request)) {
        throw invalid_argument("cannot get exchange initiator: request is incorrect");
    }
    return _player;
}

const Player &SecondStageSubEngine::other(int request) const
{
    if (!requested(request)) {
        throw invalid_argument("cannot get exchange other player: request is incorrect");
    }
    return _exchange_list[request].other;
}

const std::multiset<Resource> &SecondStageSubEngine::src(int request) const
{
    if (!requested(request)) {
        throw invalid_argument("cannot get exchange src set: request is incorrect");
    }
    return _exchange_list[request].src;
}

const std::multiset<Resource> &SecondStageSubEngine::target(int request) const
{
    if (!requested(request)) {
        throw invalid_argument("cannot get exchange target set: request is incorrect");
    }
    return _exchange_list[request].target;
}

bool SecondStageSubEngine::performed(int request) const
{
    if (!requested(request)) {
        throw invalid_argument("cannot identify request: it doesn't requested");
    }
    return _exchange_list[request].performed;
}

size_t SecondStageSubEngine::count_res(const std::multiset<Resource> &set, Resource res) noexcept
{
    return std::count(set.begin(), set.end(), res);
}
