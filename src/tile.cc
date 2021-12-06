#include "tile.hh"

std::ostream& operator<<(std::ostream& os, const Tile& t) {
    os << t.get_nord() << ' '
       << t.get_west() << ' '
       << t.get_east() << ' '
       << t.get_sud()  << ' '
       << t.get_good_position() << '\n';

    return os;
}
