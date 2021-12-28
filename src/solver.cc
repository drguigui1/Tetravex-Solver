#include "solver.hh"

Solver::Solver(Board board) :
    _board(board)
{
    this->_t0 = this->init_temp_max_c();
    this->_t = this->_t0;
    this->_lambda = 0.98;
    this->_verbose = false;

    // TODO maybe change
    this->_cooling_type = EXP_MULT;
    this->_t_min = 1e-6;
}

Solver::Solver(Board board, float t0) :
    _board(board),
    _t(t0)
{
    this->_lambda = 0.999;
    this->_verbose = false;
}

Solver::Solver(Board board, float t0, float lambda) :
    _board(board),
    _t(t0),
    _lambda(lambda)
{
    this->_verbose = false;
}

float Solver::init_temp_max_c() {
    float max_dist = 0.0f;

    for (int i = 0; i < this->_board.get_width(); ++i) {
        for (int j = 0; j < this->_board.get_width(); ++j) {
            float curr_dist = compute_tile_dist(i, j);

            if (curr_dist > max_dist)
                max_dist = curr_dist;
        }
    }

    return max_dist;
}

float Solver::get_transition_prob(float dist_s1, float dist_s2) {
    return std::exp((dist_s1 - dist_s2) / this->_t);
}

void Solver::temp_decrement_fn() {
    _t *= _lambda;
    // switch (this->_cooling_type) {
    //     case EXP_MULT:
    //         _t = exp_mult_cooling(_t0, _lambda, _k);
    //         break;
    //     default:
    //         break;
    // }

    // TODO maybe change
    // Do something smarter
    if (_t < _t_min)
        _t += 2.0f;

    // Increment the counter
    // Keep the information of number of iteration
    _k++;
}

float Solver::compute_tile_dist(int i, int j) {
    float dist = 0.0f;
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

float Solver::compute_board_dist() {
    float dist = 0.0f;
    for (int i = 0; i < this->_board.get_width(); ++i) {
        for (int j = 0; j < this->_board.get_width(); ++j) {
            dist += compute_tile_dist(i, j);
        }
    }
    return dist;
}

void Solver::random_swap() {
    // Get tiles that are available to move
    std::vector<int> tiles_ids = _board.get_available_tiles_ids();

    // Select to random ids
    int id1 = randn(0, tiles_ids.size()-1);
    int id2 = randn(0, tiles_ids.size()-1);

    // Swap the tiles
    this->_board.swap_tiles(id1, id2);

    // Store the swap
    this->_last_swap_i = id1;
    this->_last_swap_j = id2;
}

void Solver::display_log(bool first_log) {
    std::cout << "It idx: " << _k << '\n';
    std::cout << _board << '\n';
    std::cout << "It idx: " << _k << '\n';

    if (first_log) {
        std::cout << "T0:     " << _t0 << '\n';
        std::cout << "T_min:  " << _t_min << '\n';
        std::cout << "Lambda: " << _lambda << '\n';
    }
    std::cout << "T:      " << _t << '\n';
    std::cout << "Cost:   " << _curr_cost << '\n';

    int w = _board.get_width();

    if (!first_log) {
        std::cout << "Swap: ("
                  << _last_swap_i / w << ","
                  << _last_swap_i % w << ") -> ("
                  << _last_swap_j / w << ","
                  << _last_swap_j % w << ")\n";
    }

    std::cout << '\n' << "---------------" << '\n';
}

void Solver::solve() {
    // Compute the distance of the board
    float d = compute_board_dist();
    int stuck_count = 0;

    this->_curr_cost = d;

    if (_verbose) {
        display_log(true);
    }

    while (d != 0.0) {
        // Apply random swap (keep in memory the swap)
        // Swap randomly
        random_swap();

        // compute new board dist
        float new_d = compute_board_dist();

        // Check for worst cases
        if (new_d > d) {
            float proba = get_transition_prob(d, new_d);
            float rd = randf();

            // make the transition with the probability 'proba'
            if (rd > proba) {
                // revert the swap
                this->_board.swap_tiles(this->_last_swap_i, this->_last_swap_j);
            }
            else {
                d = new_d;
            }
        }
        else {
            // Update current distance
            d = new_d;
        }

        // Update the temperature
        temp_decrement_fn();

        if (new_d >= d)
            stuck_count++;
        else
            stuck_count = 0;

        if (stuck_count > 80) {
            this->_t += 2;
            stuck_count = 0;
        }

        this->_curr_cost = d;

        if (_verbose) {
            display_log(false);
        }
    }
}


void Solver::save(std::string path) {
    this->_board.save(path);
}
