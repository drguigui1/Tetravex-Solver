#pragma once

#include <iostream>
#include <cmath>
#include <fstream>

#include "utils.hh"
#include "tile.hh"

class Board {
public:
    Board() { this->_board = std::vector<Tile>(); }
    Board(std::string filepath);

    void add_tile(Tile tile) { this->_board.push_back(tile); }
    void swap_tiles(int i1, int j1, int i2, int j2);

    // Getters
    int get_nb_tiles() const { return this->_board.size(); }
    int get_width() const { return this->_width; }
    Tile get_tile(int idx) const { return this->_board[idx]; }
    Tile get_tile(int i, int j) const { return this->_board[i * _width + j]; }

    // Pretty print
    friend std::ostream& operator<<(std::ostream& os, const Board& b);

private:
    int _width;
    std::vector<Tile> _board;
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
