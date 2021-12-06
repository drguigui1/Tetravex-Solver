#pragma once

#include <iostream>

class Tile {
public:
    Tile(int nord, int west, int east, int sud, bool good_pos) :
            _nord(nord),
            _west(west),
            _east(east),
            _sud(sud),
            _good_position(good_pos) {}

    int get_nord() const { return this->_nord; }
    int get_west() const { return this->_west; }
    int get_east() const { return this->_east; }
    int get_sud()  const { return this->_sud;  }
    bool get_good_position() const { return this->_good_position; }

    // Pretty print for the tile
    friend std::ostream& operator<<(std::ostream& os, const Tile& t);

private:
    int _nord;
    int _west;
    int _east;
    int _sud;
    bool _good_position;
};
