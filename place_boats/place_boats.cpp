#include <iostream>
#include <string>
#include <cctype>
#include "boats.hpp"

const int SIZE = 10;
const std::string SHIP = "[]";

void print_grid(const std::string& title, const std::string& color, std::string grid[SIZE][SIZE]) {
    std::cout << title << std::endl;
    std::cout << BOLD << color << "  1 2 3 4 5 6 7 8 9 10" << RESET << "\n";

    for (int r = 0; r < SIZE; r++) {
        std::cout << BOLD << color << static_cast<char>('A' + r) << " " << RESET;
        for (int c = 0; c < SIZE; c++) {
            std::cout << BG_BLUE << grid[r][c] << RESET;
        }
        std::cout << std::endl;
    }
}

int row_from_letter(char row_char) {
    row_char = std::toupper(row_char);
    if (row_char < 'A' || row_char > 'J') {
        return -1;
    }
    return row_char - 'A';
}

bool parse_position(const std::string& pos, int& row, int& col) {
    if (pos.length() < 2 || pos.length() > 3) {
        return false;
    }

    row = row_from_letter(pos[0]);
    if (row == -1) {
        return false;
    }

    std::string number_part = pos.substr(1);
    for (char ch : number_part) {
        if (!std::isdigit(ch)) {
            return false;
        }
    }

    int col_num = std::stoi(number_part);
    if (col_num < 1 || col_num > 10) {
        return false;
    }

    col = col_num - 1;
    return true;
}

bool fits_on_board(char direction, int row, int col, int boat_size) {
    if (direction == 'H') {
        return col + boat_size <= SIZE;
    }
    if (direction == 'V') {
        return row + boat_size <= SIZE;
    }
    return false;
}

bool area_is_clear(std::string grid[SIZE][SIZE], char direction, int row, int col, int boat_size) {
    int row_start = row;
    int row_end = row;
    int col_start = col;
    int col_end = col;

    if (direction == 'H') {
        col_end = col + boat_size - 1;
    } else {
        row_end = row + boat_size - 1;
    }

    for (int r = row_start - 1; r <= row_end + 1; r++) {
        for (int c = col_start - 1; c <= col_end + 1; c++) {
            if (r >= 0 && r < SIZE && c >= 0 && c < SIZE) {
                if (grid[r][c] == SHIP) {
                    return false;
                }
            }
        }
    }

    return true;
}

void place_boat(std::string grid[SIZE][SIZE], char direction, int row, int col, int boat_size) {
    for (int i = 0; i < boat_size; i++) {
        if (direction == 'H') {
            grid[row][col + i] = SHIP;
        } else {
            grid[row + i][col] = SHIP;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "USAGE: ./place_boats <name>\n";
        return 1;
    }

    std::string name = argv[1];
    std::string answer;

    std::cout << "Hello " << name << ". Do you want to play a game?\n";
    std::cin >> answer;

    if (answer == "no" || answer == "No") {
        std::cout << "Run me when you are ready!\n";
        return 0;
    }

    if (answer != "yes" && answer != "Yes") {
        std::cout << "Invalid input\n";
        return 2;
    }

    std::string player_grid[SIZE][SIZE];
    std::string enemy_grid[SIZE][SIZE];

    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            player_grid[r][c] = "  ";
            enemy_grid[r][c] = "  ";
        }
    }

    std::string boat_names[5] = { "Carrier", "Battleship", "Destroyer", "Submarine", "Patrol" };
    int boat_sizes[5] = { 5, 4, 3, 3, 2 };

    std::cout << "Start by placing your 5 boats on the board.\n";
    print_grid("Your Fleet", YELLOW, player_grid);

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

            if (!area_is_clear(player_grid, direction, row, col, boat_sizes[i])) {
                std::cout << "Spot taken or too close to another ship, try again\n";
                continue;
            }

            place_boat(player_grid, direction, row, col, boat_sizes[i]);

            std::cout << "A boat of size " << boat_sizes[i]
                      << " will be placed at " << static_cast<char>('A' + row)
                      << (col + 1) << " "
                      << (direction == 'H' ? "horizontally" : "vertically") << "\n";

            print_grid("Your Fleet", YELLOW, player_grid);
            break;
        }
    }

    std::cout << "Done with board setup\n";
    std::cout << "Let the Games Begin!\n";

    print_grid("Your Fleet", YELLOW, player_grid);
    print_grid("Enemy Waters", RED, enemy_grid);

    return 0;
}