#ifndef SECONDSTAGESUBENGINE_H
#define SECONDSTAGESUBENGINE_H

#include <vector>
#include <Game.h>
#include <Exchange.h>
#include <Logger.h>

class SecondStageSubEngine
{
    SecondStageSubEngine(const SecondStageSubEngine &) = delete;
    SecondStageSubEngine &operator= (const SecondStageSubEngine &) = delete;

public:
    enum {FIELD_EXCHANGE_SRC = 4, FIELD_EXCHANGE_TARGET = 1};
    enum {MAX_RESOURCES = 19};

    SecondStageSubEngine(ResourcesHolder &game_bank, const Player &p): _player(p), _game_bank(game_bank) {}

    void exchange_with_field(Resource src, Resource target);
    int exchange_players_request(const Player &other_player,
        const std::multiset<Resource> &src, const std::multiset<Resource> &target);
    void exchange_players_accept(const Player &player, int request);

    std::set<int> requests() const noexcept;
    std::set<int> exchanges() const noexcept;

    const Player &initiator(int request) const;
    const Player &other(int request) const;

    const std::multiset<Resource> &src(int request) const;
    const std::multiset<Resource> &target(int request) const;

    bool requested(int number) const noexcept {return number < (int)_exchange_list.size() && number >= 0;}
    bool performed(int request) const;

    static size_t count_res(const std::multiset<Resource> &set, Resource) noexcept;

private:
    const Player &_player;
    ResourcesHolder &_game_bank;

    std::vector<Exchange> _exchange_list;
};

#endif // SECONDSTAGESUBENGINE_H
