#include "solver.hh"

int main() {
    
    Board b("tests/base_cases/test_input6");
    Solver s(b);

    std::cout << b;
    s.solve();
    std::cout << s.get_board();
    return 0;
}
