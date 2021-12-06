#pragma once

#include "board.hh"

class Solver {
public:
    Solver(Board board) :  _board(board) {}

    void solve();

private:
    Board _board;
};
