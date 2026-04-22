#ifndef BATTLESHIP_HPP
#define BATTLESHIP_HPP

#include <string>

const int SIZE = 10;

int start_game(int argc, char* argv[]);

void print_grid(const std::string& title, const std::string& color, std::string grid[SIZE][SIZE]);
int row_from_letter(char row_char);
bool parse_position(const std::string& pos, int& row, int& col);

bool fits_on_board(char direction, int row, int col, int boat_size);
bool area_is_clear(std::string grid[SIZE][SIZE], char direction, int row, int col, int boat_size);
void place_boat(std::string grid[SIZE][SIZE], char direction, int row, int col, int boat_size);
#endif