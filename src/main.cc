#include "board.hh"

int main() {
    Board b("");
    b.add_tile(Tile(1,2,3,4));
    b.add_tile(Tile(1,2,3,4));
    b.add_tile(Tile(1,2,3,4));
    b.add_tile(Tile(1,2,3,4));

    std::cout << b;
    return 0;
}
