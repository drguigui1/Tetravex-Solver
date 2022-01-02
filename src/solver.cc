#include "solver.hh"

Solver::Solver(Board board) :
    _board(board)
{
    this->_t0 = 2 * this->init_temp_max_c();
    // this->_t0 = this->init_temp_mean_c();
    // this->_t0 =  2 * (this->_board.get_width()) * this->init_temp_std_c();
    this->_t = this->_t0;
    this->_alpha= 0.999f;
    this->_verbose = false;

    this->_cooling_type = LIN_MULT;
    this->_t_min = 0.1f;

    // For geometric reheating
    this->_beta = 0.8f;

    this->_k = 0.0f;
}

Solver::Solver(Board board, float t0) :
    _board(board),
    _t(t0)
{
    this->_alpha = 0.999;
    this->_verbose = false;
}

Solver::Solver(Board board, float t0, float alpha) :
    _board(board),
    _t(t0),
    _alpha(alpha)
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

float Solver::init_temp_mean_c() {
    float sum_dist = 0.0f;

    for (int i = 0; i < this->_board.get_width(); ++i) {
        for (int j = 0; j < this->_board.get_width(); ++j) {
            sum_dist += compute_tile_dist(i, j);
        }
    }

    return sum_dist / this->_board.get_nb_tiles();
}

float Solver::init_temp_std_c()  {
    float sum = 0.0f;
    float sumsquare = 0.0f;
    int n = this->_board.get_nb_tiles();

    for (int i = 0; i < this->_board.get_width(); ++i) {
        for (int j = 0; j < this->_board.get_width(); ++j) {
            float dist = compute_tile_dist(i, j);

            sum += dist;
            sumsquare += dist * dist;
        }
    }

    float mean = sum / n;
    return sqrt(sumsquare / n - mean * mean);
}

float Solver::get_transition_prob(float dist_s1, float dist_s2) {
    return std::exp((dist_s1 - dist_s2) / this->_t);
}

void Solver::temp_decrement_fn() {
    switch (this->_cooling_type) {
        case EXP_MULT:
            _t = exp_mult_cooling(_t, _t0, _alpha);
            break;
        case LOG_MULT:
            _t = log_mult_cooling(_t, _t0, _alpha);
            break;
        case LIN_MULT:
            _t = lin_mult_cooling(_t, _t0, _alpha);
            break;
        default:
            break;
    }

    // TODO maybe change
    // Do something smarter
    // if (_t < _t_min)
    //     _t += 1.5f; // Increment according to the size of the board

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
    std::vector<int> tiles_ids = _board.get_available_tiles_ids();

    // Select to random ids
    int id1 = randn(0, tiles_ids.size()-1);
    int id2 = randn(0, tiles_ids.size()-1);

    // Swap the tiles
    this->_board.swap_tiles(tiles_ids[id1], tiles_ids[id2]);

    // Store the swap
    this->_last_swap_i = tiles_ids[id1];
    this->_last_swap_j = tiles_ids[id2];
}

void Solver::display_log(bool first_log) {
    std::cout << "It idx: " << _k << '\n';
    std::cout << _board << '\n';
    std::cout << "It idx: " << _k << '\n';

    if (first_log) {
        std::cout << "T0:     " << _t0 << '\n';
        std::cout << "T_min:  " << _t_min << '\n';
        std::cout << "Lambda: " << _alpha << '\n';
    }
    std::cout << "T:      " << _t << '\n';
    std::cout << "Cost:   " << _curr_cost << '\n';
    std::cout << "Proba:  " << _curr_proba << '\n';
    std::cout << "Stuck C " << _stuck_count << '\n';

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

    _stuck_count = 0.0f;
    _curr_cost = d;

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

            // Store the current proba
            _curr_proba = proba;

            float rd = randf();

            if (rd > proba) {
                // revert the swap
                this->_board.swap_tiles(this->_last_swap_i, this->_last_swap_j);
            }
            else { // make the transition with the probability 'proba'
                d = new_d;
            }
            _stuck_count++;
        } else if (new_d < d) {
            _stuck_count = 0.0f;
            d = new_d;
        }
        else {
            _stuck_count++;
        }

        // Update the temperature
        temp_decrement_fn();

        if (_stuck_count > 20) {
            // this->_t += this->_t0; // Increment according to the size of the board;
            _t = _t / _beta;
        }

        _curr_cost = d;

        if (_verbose) {
            display_log(false);
        }
    }
}


void Solver::save(std::string path) {
    this->_board.save(path);
}
