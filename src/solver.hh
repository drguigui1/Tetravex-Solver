#pragma once

#include "board.hh"

class Solver {
public:
    Solver(Board board) :  _board(board) {}

    void solve();
    int compute_board_dist();

private:
    int compute_tile_dist(int i, int j);

    Board _board;
};
