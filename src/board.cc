#include "board.hh"

Board::Board(std::string filepath) {
    this->board = std::vector<Tile>();

    std::ifstream file(filepath);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            Tile t = get_tile_from_str(line);
            this->add_tile(t);
        }
        file.close();
    }
}

// ==============================
// ======== Pretty Print ========
// ==============================

void limit_line(std::ostream& os, int size) {
    os << '+';
    for (int i = 0; i < size; ++i) {
        os << "-----+";
    }
    os << '\n';
}

// Display one line for one row
void tiles_line(std::ostream& os,
                const Board& b,
                int size, int row_idx,
                LINE_TYPE type) {
    os << '|';

    for (int j = 0; j < size; ++j) {
        // Get the proper tile
        Tile t = b.get_tile(row_idx * size + j);

        // Display proper information according to the line
        switch (type) {
            case FIRST_LINE:
                os << "  " << t.get_nord() << "  |";
                break;
            case SECOND_LINE:
                os << " " << t.get_west() << " " << t.get_east() << " |";
                break;
            case THIRD_LINE:
                os << "  " << t.get_sud() << "  |";
                break;
            default:
                break;
        }
    }
    os << '\n';
}

std::ostream& operator<<(std::ostream& os, const Board& b) {
    // Get the size (number of tiles for rows and columns)
    int size = static_cast<int>(std::sqrt(b.get_nb_tiles()));

    // Display the first line
    limit_line(os, size);

    for (int i = 0; i < size; ++i) {
        // Display 3 lines (for each row tile)
        tiles_line(os, b, size, i, FIRST_LINE);
        tiles_line(os, b, size, i, SECOND_LINE);
        tiles_line(os, b, size, i, THIRD_LINE);

        // Display the limite line
        limit_line(os, size);
    }
    return os;
}
