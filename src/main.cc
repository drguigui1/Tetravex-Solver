#include "solver.hh"

int main() {
    
    Board b("test_input3");
    Solver s(b);

    std::cout << b;
    s.solve();
    std::cout << s.get_board();
    return 0;
}
