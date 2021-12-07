#include "utils.hh"

int chr_digit_to_int(char digit) {
    return (int)digit - '0';
}

std::vector<std::string> string_split(std::string str, char delim) {
    std::vector<std::string> res;

    std::stringstream ss(str);
    std::string tmp;

    while (std::getline(ss, tmp, delim)) {
        res.push_back(tmp);
    }

    return res;
}

Tile get_tile_from_str(std::string str) {
    std::vector<std::string> s_input = string_split(str, ' ');

    int nord = chr_digit_to_int(s_input[0][0]);
    int west = chr_digit_to_int(s_input[0][1]);
    int east = chr_digit_to_int(s_input[0][2]);
    int sud = chr_digit_to_int(s_input[0][3]);

    bool good_pos = false;
    if (s_input.size() > 1 && std::strcmp(s_input[1].c_str(), "@")) {
        good_pos = true;
    }

    return Tile(nord, west, east, sud, good_pos);
}


// ========================
// Generate random numbers
// ========================

int randn(int range_from, int range_to) {
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int> distr(range_from, range_to);

    return distr(generator);
}

float randf() {
    // random generation
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_real_distribution<float> distribution(0.0, 1.0);

    return distribution(generator);
}
