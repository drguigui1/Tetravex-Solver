#include "solver.hh"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: ./a.out <input_file> <out_file>\n";
        std::cout << "Usage: ./a.out <input_file>\n";
    } else if (argc == 2) {
        // Test case
        std::string input_file(argv[1]);
        Board b(input_file);
        Solver s(b);
        s.set_verbose();
        s.solve();
        std::cout << s.get_board();
    } else if (argc >= 3) {
        // First argument input file / Second argument output file
        std::string input_file(argv[1]);
        std::string output_file(argv[2]);

        Board b(input_file);
        Solver s(b);
        s.solve();
        s.save(output_file);
    }

    return 0;
}
