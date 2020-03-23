//
//  main.cpp
//  LearnCPP
//
//  Created by Domenic SanGiovanni on 2/21/20.
//  Copyright © 2020 Domenic SanGiovanni. All rights reserved.
//

#include <iostream>
#include <map>
#include <iterator>
#include <vector>
#include <set>
using namespace std;

class SudokuBoard {
private:
    int size;
    int board_dim;
    int** board;
    
public:
    SudokuBoard () {
        size = 9;
        board_dim = 3;
        board = new int*[9];
        
        for (int i = 0; i < size; i++) {
            board[i] = new int[9];
        }
        
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                board[i][j] = 0;
            }
        }
    }
        
    SudokuBoard (int board_dimension) {
        board_dim = board_dimension;
        size = board_dim * board_dim;
        
        board = new int*[size];
        
        for (int i = 0; i < size; i++) {
            board[i] = new int[size];
        }
    }
    
    /*
     Method to test if a number
     */
    bool is_in_square(int row, int col, int num) {
        int tempRow = row / board_dim;
        tempRow *= board_dim;
        
        int tempCol = col / board_dim;
        tempCol *= board_dim;
        
        for (int i = 0; i < board_dim; i++) {
            for (int j = 0; j < board_dim; j++) {
                if (board[row][col] == num) {
                    return true;
                }
            }
        }
        
        return false;
    }
    
    int get_size() {
        return size;
    }
    

    /*
        Method to check if a number is in the given row
     */
    bool is_in_row(int row, int num) {
        for (int i = 0; i < size; i++) {
            if (board[row][i] == num) {
                return true;
            }
        }
        
        return false;
    }
    
    
    /*
     Method to check if a number is in the column
     */
    bool is_in_col(int col, int num) {
        for (int i = 0; i < size; i++) {
            if (board[i][col] == num) {
                return true;
            }
        }
        
        return false;
    }

    /*
     Method to set cell in a sudoku board
     */
    bool set_cell(int row, int col, int num) {
        if (row >= size || row < 0 || col >= size || col < 0) {
            return false;
        } else {
            board[row][col] = num;
            return true;
        }
    }
    
    /*
     Method to check if the move is valid
     */
    bool is_valid_move(int row, int col, int num) {
        if (is_in_col(col, num) || is_in_row(row, num) || is_in_square(row, col, num)) {
            return false;
        } else {
            return true;
        }
    }
    
    /*
     Method to check if the Board is still valid
     */
    bool is_valid_board() {
        
        /* Check valid row */
        for (int i = 0; i < size; i++) {
            if(!is_valid_col(i) || !is_valid_row(i) || !is_valid_square(i)) {
                return false;
            }
        }
        
        return true;
    }
    
    bool is_valid_row(int row) {
        bool* numbers = new bool[size];
        
        for (int i = 0; i < size; i++) {
            if (numbers[board[row][i] - 1] == true) {
                return false;
            } else {
                numbers[board[row][i] - 1] = true;
            }
        }
        
        return true;
    }
    
    bool is_valid_col(int col) {
        bool* numbers = new bool[size];
        
        for (int i = 0; i < size; i++) {
            if (board[i][col] == 0) {
                continue;
            } else if (numbers[board[i][col] - 1] == true) {
                return false;
            } else {
                numbers[board[i][col] - 1] = true;
            }
        }
        
        return true;
    }
    
    bool is_valid_square(int square) {
        bool* numbers = new bool[size];
        
        for (int row = square / board_dim; row < square/board_dim + board_dim; row ++) {
            for (int col = square % board_dim; col < square % board_dim + board_dim; col++) {
                if (board[row][col] == 0) {
                    continue;
                } else if (numbers[board[row][col] - 1] == true) {
                    return false;
                } else {
                    numbers[board[row][col] - 1] = true;
                }
            }
        }
        
        return true;
    }
    
    void parse_text(const char* text) {
        int i = 0;
        
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                board[row][col] = (int)text[i++] - 48;
            }
        }
    }
    
    void print() {
        print_horizontal_bar();
        
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                cout << '|' + to_string(board[i][j]);
            }
            cout << "|\n";
            
            print_horizontal_bar();
            
        }
    }
    
    int** get_board() {
        return board;
    }
    
private:
    void print_horizontal_bar() {
        for (int i = 0; i < (2 * size) + 1; i++) {
            cout << "-";
        }
        cout << "\n";
    }
};

class TrivialSolver {
    
public:
    SudokuBoard trivial_solve(SudokuBoard sb) {
        map<int, set<int>> final_options;
        
        for (int cell = 0; cell < sb.get_size() * sb.get_size(); cell ++) {
            int row = cell / sb.get_size();
            int col = cell % sb.get_size();
            set<int> s;
            
            if (sb.get_board()[row][col] == 0) {
                set<int> options;
                for (int nums = 0; nums < sb.get_size(); nums++) {
                    options.insert(nums + 1);
                }
                
                // First we check the Row
                for (int x = 1; x < sb.get_size() + 1; x++) {
                    if (sb.is_in_row(row, x)) {
                        options.erase(x);
                    }
                }
                
                // Then We check the column
                for (int i = 0; i < sb.get_size(); i++) {
                    for (int x = 1; x < sb.get_size() + 1; x++) {
                        if (sb.is_in_col(col, x)) {
                            options.erase(x);
                        }
                    }
                }
                
                // Then we check the Box
                for (int x = 1; x < sb.get_size() + 1; x++) {
                    if (sb.is_in_square(row, col, x)) {
                        options.erase(x);
                    }
                }
            }
            final_options.insert(pair<int, set<int>>(cell, s));
        }
    }
};

int main(int argc, const char * argv[]) {
    SudokuBoard b = SudokuBoard();

    b.parse_text("004300209005009001070060043006002087190007400050083000600000105003508690042910300");
    b.print();
}
