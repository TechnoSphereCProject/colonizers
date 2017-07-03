#ifndef COORD_H
#define COORD_H

#include <sstream>

class Coord
    : public std::pair<int, int>
{
public:
    Coord() = default;
    Coord(int x, int y) {first = x; second = y;}

    std::string str() const {std::stringstream ss; ss << '(' << first << ',' << second << ')'; return ss.str();}

    Coord east() const noexcept {return Coord(first + 1, second);}
    Coord west() const noexcept {return Coord(first - 1, second);}
    Coord north_east() const noexcept {return Coord(first, second + 1);}
    Coord north_west() const noexcept {return Coord(first - 1, second + 1);}
    Coord south_east() const noexcept {return Coord(first + 1, second - 1);}
    Coord south_west() const noexcept {return Coord(first, second - 1);}
};

#endif // COORD_H
