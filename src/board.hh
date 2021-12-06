#pragma once

#include <iostream>
#include <cmath>
#include <fstream>

#include "utils.hh"
#include "tile.hh"

class Board {
public:
    Board() { this->board = std::vector<Tile>(); }
    Board(std::string filepath);

    void add_tile(Tile tile) { this->board.push_back(tile); }

    // Getters
    int get_nb_tiles() const { return this->board.size(); }
    Tile get_tile(int idx) const { return this->board[idx]; }

    // Pretty print
    friend std::ostream& operator<<(std::ostream& os, const Board& b);

private:

    std::vector<Tile> board;
};

// ==============================
// ======== Pretty Print ========
// ==============================

enum LINE_TYPE {
    FIRST_LINE,
    SECOND_LINE,
    THIRD_LINE
};

void limit_line(std::ostream& os, int nb_cols);
void tiles_line(std::ostream& os,
                const Board& b,
                int size, int row_idx,
                LINE_TYPE type);
