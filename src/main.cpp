#include <iostream>
#include <exception>

#include <GameEngine.h>

#include "format.h"
//#include "format.cc"

using namespace std;

void show_res_num(const Game &g, size_t num_player) {
    fmt::print("bank res = {}\n", g.field().bank().resources());
    fmt::print("player res = {}\n", g.player(num_player).bank().resources());
}

void show_res_full(const string &name, const ResourcesHolder &bank) {
    fmt::print("{}'s resources = ", name);
    fmt::print("{0} ({1})", EnumInfo::resource_str(Resource::CLAY), bank.resources(Resource::CLAY));
    fmt::print("{0} ({1})", EnumInfo::resource_str(Resource::GRAIN), bank.resources(Resource::GRAIN));
    fmt::print("{0} ({1})", EnumInfo::resource_str(Resource::ORE), bank.resources(Resource::ORE));
    fmt::print("{0} ({1})", EnumInfo::resource_str(Resource::WOOD), bank.resources(Resource::WOOD));
    fmt::print("{0} ({1})\n", EnumInfo::resource_str(Resource::WOOL), bank.resources(Resource::WOOL));
}

void show_res_all(const Game &g) {
    for (size_t i = 0; i < g.num_players(); i++) {
        show_res_full(g.player(i).name(), g.player(i).bank());
    }
    show_res_full("bank", g.field().bank());
}

void list_inf(const Game &g) {
    for (size_t i = 0; i < g.num_players(); i++) {
        for (size_t j = 0; j < g.player(i).num_roads(); j++) {
            fmt::print("player {0}  road {1}\n", g.player(i).name(), g.player(i).road(j).name());
        }
        cout << endl;
        for (size_t j = 0; j < g.player(i).num_towns(); j++) {
            fmt::print("player {0}  town {1}\n", g.player(i).name(), g.player(i).town(j).name());
        }
        cout << endl;
        for (size_t j = 0; j < g.player(i).num_cities(); j++) {
            fmt::print("player {0}  city {1}\n", g.player(i).name(), g.player(i).city(j).name());
        }
        cout << endl;
    }
}

void spots(ThirdStageSubEngine &ts) {
    auto r = ts.valid_road_spots();
    auto t = ts.valid_town_spots();
    auto c = ts.valid_city_spots();
    fmt::print("road spots:\n");
    for (auto &i: r) {
        cout << i.first.str() << ' ' << EnumInfo::road_side_str(i.second) << endl;
    }
    fmt::print("town spots:\n");
    for (auto &i: t) {
        cout << i.first.str() << ' ' << EnumInfo::cross_corner_str(i.second) << endl;
    }

    fmt::print("city spots:\n");
    for (auto &i: c) {
        cout << i.first.str() << ' ' << EnumInfo::cross_corner_str(i.second) << endl;
    }
}

int main()
{
    try {
        GameEngine g;

        g.join_player("a");
        g.join_player("b");
        g.join_player("c");

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

        //g.put_initial_infrastructure("a", "ata", Coord(0,-2), CrossCorner::BOTTOM, "ara", Coord(0,-3), RoadSide::RIGHT);
        //g.put_initial_infrastructure("b", "bta", Coord(0,0), CrossCorner::TOP, "bra", Coord(0,0), RoadSide::UP);
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


        g.game().player(0).bank().add(Resource::CLAY, 90);
        g.game().player(0).bank().add(Resource::GRAIN, 90);
        g.game().player(0).bank().add(Resource::ORE, 90);
        g.game().player(0).bank().add(Resource::WOOD, 90);
        g.game().player(0).bank().add(Resource::WOOL, 90);

        g.build_road("a", "ard", Coord(-2,0), RoadSide::UP);
        g.build_road("a", "arc", Coord(-3,1), RoadSide::DOWN);
        g.build_road("a", "are", Coord(-3,0), RoadSide::RIGHT);
        g.build_road("a", "arf", Coord(-2,-1), RoadSide::UP);
        show_res_num(g.game(),0);
        g.build_road("a", "arg", Coord(-2,0), RoadSide::DOWN);
        show_res_num(g.game(),0);

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

        g.build_city("a", "aca", Coord(-3,2),CrossCorner::BOTTOM);

        //spots(g.ts());

        fmt::print("score = {}", g.score("a"));

        fmt::print("\n{}\n", EnumInfo::game_stage_str(g.stage()));
        fmt::print("OK!!!\n");
    } catch (exception &e) {
        fmt::print("EXCEPTION WAAAT: {}", e.what());
    } catch (size_t i) {
        fmt::print("EXCEPTION INT = {}", i);
    }
    
    fmt::print("SUCCESS!!!\n");
}
