// SIMPLE TEST

#include <iostream>

#include <GameEngine.h>

using namespace std;

void infinity_res_cheat(const Player &p) {
    p.bank().add(Resource::CLAY, (size_t)-90);
    p.bank().add(Resource::GRAIN, (size_t)-90);
    p.bank().add(Resource::ORE, (size_t)-90);
    p.bank().add(Resource::WOOD, (size_t)-90);
    p.bank().add(Resource::WOOL, (size_t)-90);
}

int main()
{
    try {
        GameEngine g;

        g.join_player("excess");
        g.join_player("ya tut mimo");
        g.join_player("a");
        g.join_player("b");
        g.join_player("c");
        g.unjoin_player("ya tut mimo");
        g.unjoin_player("excess");

        g.start_game();

        for (char c = 'a'; c < 'd'; c++) {
            string player = "0";
            string inf = "0r0";
            inf[0] = c;
            player[0] = c;
            for (char i = 'a'; i - 'a' < PreparationEngine::MAX_ROADS; i++) {
                inf[2] = i;
                g.register_road(player, inf);
            }
            inf[1] = 'c';
            for (char i = 'a'; i - 'a' < PreparationEngine::MAX_CITIES; i++) {
                inf[2] = i;
                g.register_city(player, inf);
            }
            inf[1] = 't';
            for (char i = 'a'; i - 'a' < PreparationEngine::MAX_TOWNS; i++) {
                inf[2] = i;
                g.register_town(player, inf);
            }
        }

        for (char c = 'a', i =-1,j=-1; c < 'd'; c++,i++,j++) {
            string player = "0", road = "0ra", town = "0ta";
            road[0] = town[0] = c;
            player[0] = c;
            g.put_initial_infrastructure(player, town, Coord(i,j), CrossCorner::TOP, road, Coord(i-1,j+1), RoadSide::RIGHT);
        }
        for (char c = 'c', i =2,j=-2; c >= 'a'; c--,i--,j++) {
            string player = "0", road = "0rb", town = "0tb";
            road[0] = town[0] = c;
            player[0] = c;
            g.put_initial_infrastructure(player, town, Coord(i,j), CrossCorner::BOTTOM, road, Coord(i,j), RoadSide::DOWN);
        }

        g.make_dice("a", 11);
        g.end_exchanges("a");

        for(uint i = 0; i < g.game().num_players(); ++i)
            infinity_res_cheat(g.game().player(i));

        g.build_road("a", "ard", Coord(-2,0), RoadSide::UP);
        g.build_road("a", "arc", Coord(-3,1), RoadSide::DOWN);
        g.build_road("a", "are", Coord(-3,0), RoadSide::RIGHT);
        g.build_road("a", "arf", Coord(-2,-1), RoadSide::UP);
        g.build_road("a", "arg", Coord(-2,0), RoadSide::DOWN);

        g.build_road("a", "arh", Coord(-2,1), RoadSide::DOWN);
        g.build_road("a", "ark", Coord(-1,0), RoadSide::UP);
        g.build_road("a", "arj", Coord(-1,0), RoadSide::RIGHT);
        g.build_road("a", "ari", Coord(-1,0), RoadSide::DOWN);
        g.build_road("a", "arl", Coord(-1,-1), RoadSide::UP);
        g.build_road("a", "arm", Coord(-2,-1), RoadSide::RIGHT);
        g.build_road("a", "arn", Coord(-3,1), RoadSide::RIGHT);

        g.build_town("a", "atc", Coord(-2,-1), CrossCorner::TOP);
        g.build_town("a", "atd", Coord(-1,0), CrossCorner::TOP);

        g.build_city("a", "acb", Coord(-1,0), CrossCorner::TOP);
        g.build_city("a", "acc", Coord(-1,-1), CrossCorner::TOP);
        g.build_city("a", "acd", Coord(0,0), CrossCorner::BOTTOM);

        g.build_town("a", "ata", Coord(-1,-2), CrossCorner::TOP);
        g.build_town("a", "atb", Coord(-3,2), CrossCorner::BOTTOM);

        g.next_player("a");

        g.make_dice("b", 2);
        g.exchange_players_request("b", "c", {Resource::CLAY}, {Resource::GRAIN});
        g.exchange_players_accept("c", 0);
        g.end_exchanges("b");
        g.next_player("b");

        g.make_dice("c", 3);
        g.end_exchanges("c");
        g.next_player("c");

        g.make_dice("a", 10);
        g.end_exchanges("a");
        g.build_city("a", "aca", Coord(-3,2),CrossCorner::BOTTOM);

    } catch (exception &e) {
        cout << "EXCEPTION WAAAT: " << e.what() << endl;
    }
}
