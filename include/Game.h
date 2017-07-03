#ifndef GAME_H
#define GAME_H

#include <Field.h>
#include <Player.h>

#include<stdexcept>
#include<string>

class Game
{
    Game(const Game &) = delete;
    Game &operator= (const Game &) = delete;

public:
    Game() {_field = new Field();}
    ~Game();

    Field &field() const noexcept {return *_field;}

    Player &add_player(const string &name) {_players.push_back(new Player(name)); return *_players.back();}
    size_t num_players() const noexcept {return _players.size();}
    Player &player(size_t i) const;

private:
    Field *_field;
    std::vector<Player *> _players;
};


#endif // GAME_H
