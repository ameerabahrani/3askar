#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"

int line_type(int r1, int c1, int r2, int c2){
    if (r1 == r2)
        return 0; // horizontal
    if (c1 == c2)
        return 1; // vertical
    return -1; // invalid
}

bool adjacent (int r1, int c1, int r2, int c2){
    if (r1 == r2 && abs(c1 - c2) == 1)
        return true;
    if (c1 == c2 && abs(r1 - r2) == 1)
        return true;
    
    return false;
}

bool check_box(GameState *state, int r, int c){
    if (r < 0 || r >= 4 || c < 0 || c >= 5)
        return false;
    if (state->horizontal_lines[r][c] && state->horizontal_lines[r+1][c] && state->vertical_lines[r][c] && state->vertical_lines[r][c+1]){
        return true;
    }
    return false;
}

//0 success
//-1 not adjacent
//-2 invalid line
//-3 line already taken
//-4 invalid coordinates
int process_move(GameState *state, int r1, int c1, int r2, int c2){
    if (!adjacent(r1, c1, r2, c2))
        return -1; 
    if (line_type(r1, c1, r2, c2) == -1)
        return -2;
    if (line_type(r1, c1, r2, c2) == 0){ // horizontal
        int min_col = (c1 < c2) ? c1 : c2;

        if (state->horizontal_lines[r1][min_col])
            return -3;

        if (r1 < 0 || r1 > 4 || c1 < 0 || c1 > 4)
            return -4; 

        state->horizontal_lines[r1][min_col] = true;
        state->board[2 * r1 ][2 * min_col + 1] = '_';
        return 0;
    }
    if (line_type(r1, c1, r2, c2) == 1){ // vertical 
        int min_row = (r1 < r2) ? r1 : r2;

        if (state->vertical_lines[min_row][c1]) 
            return -3;

        if (min_row < 0 || min_row >= ROWS || c1 < 0 || c1 >= COLS)
            return -4; 

        state->vertical_lines[min_row][c1] = true;
        state->board[2 * min_row + 1][2 * c1 + 1] = '|';
        return 0; 
    }
    return -2;
}