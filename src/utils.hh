#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <random>

#include "tile.hh"

int chr_digit_to_int(char digit);
std::vector<std::string> string_split(std::string str, char delim);
Tile get_tile_from_str(std::string str);
int randn(int range_from, int range_to);
float randf();
