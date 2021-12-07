#pragma once

#include "board.hh"

class Solver {
public:
    Solver(Board board);
    Solver(Board board, float init_temp);
    Solver(Board board, float init_temp, float lambda);

    void solve();

    Board get_board() const { return this->_board; }

private:
    float compute_tile_dist(int i, int j);
    float compute_board_dist();
    float get_transition_prob(float dist_s1, float dist_s2);
    void temp_decrement_fn();
    void random_swap();

    Board _board;
    float _temp;
    float _lambda;

    // Keep information of the index of the last
    // In order to revert if necessary
    int last_swap_i;
    int last_swap_j;
};
