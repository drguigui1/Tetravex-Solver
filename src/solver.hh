#pragma once

#include "board.hh"
#include "cooling.hh"

class Solver {
public:
    Solver(Board board);
    Solver(Board board, float init_temp);
    Solver(Board board, float init_temp, float lambda);

    void solve();
    void save(std::string path);

    // Getters / Setters
    Board get_board() const { return this->_board; }
    void set_verbose() { this->_verbose = true; }

private:
    float compute_tile_dist(int i, int j);
    float compute_board_dist();
    float get_transition_prob(float dist_s1, float dist_s2);
    void temp_decrement_fn();
    void random_swap();
    float init_temp_max_c();
    void display_log(bool first_log);

    // all the board information
    Board _board;

    // current temperature
    float _t;

    // initial temperature
    float _t0;

    // minimum temperature
    float _t_min;

    // cooling factor
    float _lambda;

    // if true display information
    bool _verbose;

    // number of iteration
    // (temperature update)
    int _k;

    // Keep information of the index of the last
    // In order to revert if necessary
    int _last_swap_i;
    int _last_swap_j;

    // Current cost
    int _curr_cost;

    // Type of cooling schedule
    // How to change the temperature
    CoolingType _cooling_type;
};
