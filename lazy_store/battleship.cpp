#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include <cstdint>
#include "boats.hpp"
#include "battleship.hpp"

const uint32_t MAGIC = 0x00BA7713;
const int PLAYER_BOARD_OFFSET = 4;
const int ENEMY_BOARD_OFFSET = 104;

char encode_player_cell(const std::string& cell) {
    if (cell == "  ") return ' ';
    if (cell == "[]") return '0';
    if (cell == "M") return 'M';
    if (cell == "H") return 'H';
    if (cell == "S") return 'S';
    return ' ';
}

char encode_enemy_cell(const std::string& cell) {
    if (cell == "  ") return ' ';
    if (cell == "M") return 'M';
    if (cell == "H") return 'H';
    if (cell == "S") return 'S';
    return ' ';
}

void save_full_game(std::string your_board[SIZE][SIZE], std::string enemy_board[SIZE][SIZE]) {
    std::ofstream file("battleship.sav", std::ios::binary);
    if (!file) {
        return;
    }

    file.write(reinterpret_cast<const char*>(&MAGIC), sizeof(MAGIC));

    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            char ch = encode_player_cell(your_board[r][c]);
            file.write(&ch, 1);
        }
    }

    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            char ch = encode_enemy_cell(enemy_board[r][c]);
            file.write(&ch, 1);
        }
    }
}
void save_single_cell(int board_offset, int row, int col, char value) {
    std::fstream file("battleship.sav", std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        return;
    }

    int offset = board_offset + (row * SIZE) + col;
    file.seekp(offset);
    file.write(&value, 1);
}

bool is_valid_shot_result(const std::string& result) {
    return result == "H" || result == "M" || result == "S";
}

bool is_already_shot(const std::string& cell) {
    return cell == MISS || cell == HIT || cell == SUNK;
}

int count_cells_with_value(std::string grid[SIZE][SIZE], const std::string& value) {
    int count = 0;
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            if (grid[r][c] == value) {
                count++;
            }
        }
    }
    return count;
}

int start_game(int argc, char* argv[]) {
    bool lazy_mode = false;
    std::string name;

    if (argc == 2) {
        name = argv[1];
    } else if (argc == 3 && std::string(argv[1]) == "--lazy") {
        lazy_mode = true;
        name = argv[2];
    } else {
        std::cout << "Usage: ./battleship.exe [--lazy] <name>\n";
        return 1;
    }

    std::string answer;

    std::cout << "Hello " << name << ". Do you want to play a game?\n";

    if (!(std::cin >> answer)) {
        return 0;
    }

    if (answer == "no" || answer == "No") {
        std::cout << "Run me when you are ready!\n";
        return 0;
    }

    if (answer != "yes" && answer != "Yes") {
        std::cout << "Invalid input\n";
        return 2;
    }

    std::string your_board[SIZE][SIZE];
    std::string enemy_board[SIZE][SIZE];

    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            your_board[r][c] = "  ";
            enemy_board[r][c] = "  ";
        }
    }

    if (lazy_mode) {
        std::cout << "Randomly placing your 5 boats on the board.\n";
        randomly_place_all_boats(your_board);
        print_grid("Your Fleet", YELLOW, your_board);
    } else {
        std::cout << "Start by placing your 5 boats on the board.\n";
        print_grid("Your Fleet", YELLOW, your_board);

        std::string boat_names[5] = { "Carrier", "Battleship", "Destroyer", "Submarine", "Patrol" };
        int boat_sizes[5] = { 5, 4, 3, 3, 2 };

        for (int i = 0; i < 5; i++) {
            while (true) {
                std::string direction_text;
                std::string position_text;

                std::cout << boat_names[i] << "(" << boat_sizes[i] << ") Pick a location to place a boat:\n";
                std::cin >> direction_text >> position_text;

                if (direction_text.length() != 1) {
                    std::cout << "Invalid input. Format: H A1\n";
                    continue;
                }

                char direction = direction_text[0];
                if (direction != 'H' && direction != 'V') {
                    std::cout << "Invalid input. Format: H A1\n";
                    continue;
                }

                int row, col;
                if (!parse_position(position_text, row, col)) {
                    std::cout << "Invalid input. Format: H A1\n";
                    continue;
                }

                if (!fits_on_board(direction, row, col, boat_sizes[i])) {
                    if (direction == 'H') {
                        std::cout << "Not enough space horizontally for boat\n";
                    } else {
                        std::cout << "Not enough space vertically for boat\n";
                    }
                    continue;
                }

                if (!area_is_clear(your_board, direction, row, col, boat_sizes[i])) {
                    std::cout << "Spot taken or too close to another ship, try again\n";
                    continue;
                }

                place_boat(your_board, direction, row, col, boat_sizes[i]);

                std::cout << "A boat of size " << boat_sizes[i]
                          << " will be placed at " << static_cast<char>('A' + row)
                          << (col + 1) << " "
                          << (direction == 'H' ? "horizontally" : "vertically") << "\n";

                print_grid("Your Fleet", YELLOW, your_board);
                break;
            }
        }
    }
save_full_game(your_board, enemy_board);

std::cout << "Done with board setup\n";
std::cout << "Let the Games Begin!\n";

while (true) {
    print_grid("Your Fleet", YELLOW, your_board);
    print_grid("Enemy Waters", RED, enemy_board);

    std::string coord;
    std::string result;

    std::cout << "Your turn! Enter your shot (expected: A1 H/M/S):\n";
    std::cin >> coord >> result;

    int row, col;
    if (!parse_position(coord, row, col) || !is_valid_shot_result(result)) {
        std::cout << "Invalid input. Format: A1 H\n";
        continue;
    }

    if (is_already_shot(enemy_board[row][col])) {
        std::cout << "You already fired at this location\n";
        continue;
    }

    if (result == "M") {
        enemy_board[row][col] = MISS;
        save_single_cell(ENEMY_BOARD_OFFSET, row, col, 'M');
        std::cout << "You shot at " << coord << ": MISS!\n";
    } else if (result == "H") {
        enemy_board[row][col] = HIT;
        save_single_cell(ENEMY_BOARD_OFFSET, row, col, 'H');
        std::cout << "You shot at " << coord << ": HIT!\n";
    } else {
        enemy_board[row][col] = SUNK;
        save_single_cell(ENEMY_BOARD_OFFSET, row, col, 'S');
        std::cout << "You shot at " << coord << ": SUNK!\n";
    }

    break;
}

print_grid("Your Fleet", YELLOW, your_board);
print_grid("Enemy Waters", RED, enemy_board);

return 0;
}