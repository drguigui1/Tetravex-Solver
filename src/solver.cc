#include "solver.hh"

Solver::Solver(Board board) :
    _board(board)
{
    this->_t0 = this->_board.get_width() * (this->_board.get_width());
    // this->_t0 = this->init_temp_mean_c();
    // this->_t0 =  2 * (this->_board.get_width()) * this->init_temp_std_c();
    //this->_t0 = this->compute_board_dist();
    // this->_t0 = 50.0f;

    this->_t = this->_t0;

    this->_verbose = false;

    this->_t_min = 0.000001f;

    // Power coefficient (how to update the temperature)
    // this->_c = 100000.0f;

    // Number of iteration
    this->_nb_it = 0.0f;

    // Number of time stuck
    this->_stuck_time = 0.0f;

    this->_alpha = 0.999980298f;
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

void Solver::cooling_schedule() {
    _t *= _alpha;
    _nb_it++;
}

void Solver::heating_schedule() {
    _t += 1.0f;
    _stuck_count = 0.0f;
    //_c += 100000.0f;
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

void Solver::swap() {
    random_swap();

    // compute new board dist
    float new_d = compute_board_dist();

    // Check for worst case
    if (new_d > _curr_cost) {
        float proba = get_transition_prob(_curr_cost, new_d);

        // Store the current proba
        _curr_proba = proba;

        float rd = randf();

        if (rd > proba) {
            // revert the swap
            this->_board.swap_tiles(this->_last_swap_i, this->_last_swap_j);
        }
    }
}

void Solver::display_log(bool first_log) {
    std::cout << "It idx: " << _nb_it << '\n';
    std::cout << _board << '\n';
    std::cout << "It idx: " << _nb_it << '\n';

    if (first_log) {
        std::cout << "T0:     " << _t0 << '\n';
        std::cout << "T_min:  " << _t_min << '\n';
    }
    std::cout << "T:      " << _t << '\n';
    std::cout << "Cost:   " << _curr_cost << '\n';
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
    _curr_cost = d;

    if (_verbose) {
        display_log(true);
    }

    while (_curr_cost != 0.0) {
        // Apply swap
        swap();

        float new_d = compute_board_dist();        
        
        // Increment _stuck_count if stuck
        _stuck_count = (new_d == _curr_cost) ? _stuck_count + 1 : 0.0f;

        // Reheating
        if (_stuck_count == 100000) {
            heating_schedule();
            display_log(false);
        }

        cooling_schedule();
        _curr_cost = new_d;

        if (_verbose) {
            display_log(false);
        }

        if ((int) _nb_it % 1000000 == 0)
            display_log(false);
    }
}


void Solver::save(std::string path) {
    this->_board.save(path);
}
