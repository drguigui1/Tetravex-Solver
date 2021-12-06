#include "solver.hh"

int Solver::compute_tile_dist(int i, int j) {
    int dist = 0;
    int width = this->_board.get_width();

    // Top case
    if (i - 1 > 0) {
        int nord_val = _board.get_tile(i, j).get_nord();
        int sud_val = _board.get_tile(i-1, j).get_sud();
        dist += nord_val == sud_val ? 0 : 1;
    }

    // Left case
    if (j - 1 > 0) {
        int west_val = _board.get_tile(i, j).get_west();
        int east_val = _board.get_tile(i, j-1).get_east();
        dist += west_val == east_val ? 0 : 1;
    }

    // Bottom case
    if (i + 1 < width) {
        int sud_val = _board.get_tile(i, j).get_sud();
        int nord_val = _board.get_tile(i+1, j).get_nord();
        dist += sud_val == nord_val ? 0 : 1;
    }

    // Right case
    if (j + 1 < width) {
        int east_val = _board.get_tile(i, j).get_east();
        int west_val = _board.get_tile(i, j+1).get_west();
        dist += east_val == west_val ? 0 : 1;
    }

    return dist;
}

int Solver::compute_board_dist() {
    int dist = 0;
    for (int i = 0; i < this->_board.get_width(); ++i) {
        for (int j = 0; j < this->_board.get_width(); ++j) {
            dist += compute_tile_dist(i, j);
        }
    }
    return dist;
}
