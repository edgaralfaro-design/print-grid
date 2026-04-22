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
void randomly_place_all_boats(std::string grid[SIZE][SIZE]);
char encode_player_cell(const std::string& cell);
char encode_enemy_cell(const std::string& cell);
void save_full_game(std::string your_board[SIZE][SIZE], std::string enemy_board[SIZE][SIZE]);
void save_single_cell(int board_offset, int row, int col, char value);

bool is_valid_shot_result(const std::string& result);
bool is_already_shot(const std::string& cell);
int count_cells_with_value(std::string grid[SIZE][SIZE], const std::string& value);
#endif