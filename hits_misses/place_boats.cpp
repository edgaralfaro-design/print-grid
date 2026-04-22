#include <iostream>
#include <string>
#include <cctype>
#include "boats.hpp"
#include "battleship.hpp"

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
