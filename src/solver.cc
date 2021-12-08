#include "solver.hh"

Solver::Solver(Board board) :
    _board(board)
{
    this->_temp = 5;
    this->_lambda = 0.99999;
    this->_verbose = false;
}

Solver::Solver(Board board, float init_temp) :
    _board(board),
    _temp(init_temp)
{
    this->_lambda = 0.98;
    this->_verbose = false;
}

Solver::Solver(Board board, float init_temp, float lambda) :
    _board(board),
    _temp(init_temp),
    _lambda(lambda)
{
    this->_verbose = false;
}

float Solver::get_transition_prob(float dist_s1, float dist_s2) {
    return std::exp((dist_s1 - dist_s2) / this->_temp);
}

void Solver::temp_decrement_fn() {
    this->_temp *= this->_lambda;
    if (this->_temp < 1.0) {
        this->_temp = 1.0;
    }
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
    this->last_swap_i = id1;
    this->last_swap_j = id2;
}

void Solver::solve() {
    // Compute the distance of the board
    float d = compute_board_dist();

    while (d != 0.0) {
        // Apply random swap (keep in memory the swap)
        // Swap randomly
        random_swap();

        // compute new board dist
        float new_d = compute_board_dist();

        if (_verbose)
            std::cout << d << "  " << new_d << '\n';

        // Check for worst cases
        if (new_d > d) {
            float proba = get_transition_prob(d, new_d);
            float rd = randf();

            if (_verbose)
                std::cout << "Worst case proba: " << proba << "/ rd: " << rd << '\n';

            // make the transition with the probability 'proba'
            if (rd > proba) {
                // revert the swap
                if (_verbose)
                    std::cout << "No transition with proba: " << proba << '\n';
                this->_board.swap_tiles(this->last_swap_i, this->last_swap_j);
            }
            else {
                d = new_d;
            }
        }
        else {
            // Update current distance
            d = new_d;
        }

        if (_verbose)
            std::cout << '\n';

        // Update the temperature
        temp_decrement_fn();

        if (d <= 10 && _verbose) {
            std::cout << _board;
        }
    }
}


void Solver::save(std::string path) {
    this->_board.save(path);
}
