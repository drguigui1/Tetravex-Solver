#include "solver.hh"

int main() {
    Board b("test_input");

    std::cout << b;
    Solver s(b);
    std::cout << s.compute_board_dist() << '\n';
    return 0;
}
