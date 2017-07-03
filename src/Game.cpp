#include "Game.h"

Game::~Game()
{
    for (auto i: _players) {
        delete i;
    }
    delete _field;
}

Player &Game::player(size_t i) const
{
    if (i >= _players.size()) {
        throw std::out_of_range("Player &Game::player() OUT OF RANGE");
    }
    return *_players[i];
}
