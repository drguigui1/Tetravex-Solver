#include "board.hh"

int main() {
    Board b("test_input");

    std::cout << b;
    b.swap_tiles(0, 0, 1, 1);
    std::cout << b;
    return 0;
}
