#pragma once

class Tile {
public:
    Tile(int nord, int west, int east, int sud) :
            _nord(nord),
            _west(west),
            _east(east),
            _sud(sud) { this->_good_position = false; }

    int get_nord() const { return this->_nord; }
    int get_west() const { return this->_west; }
    int get_east() const { return this->_east; }
    int get_sud()  const { return this->_sud;  }

private:
    int _nord;
    int _west;
    int _east;
    int _sud;
    bool _good_position;
};
