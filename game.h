#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define ROWS 4
#define COLS 5
#define players 2

typedef struct {
    bool horizontal_lines[ROWS + 1][COLS];
    bool vertical_lines[ROWS][COLS + 1]; 
    int box_owner[ROWS][COLS]; // 0=neutral  1/2=players
    int current_player;
    int scores[players];
    int remaining_boxes; 
    char board[ROWS * 2 + 1][COLS * 2 + 1]; //9×11 board for printing dots and spaces
} GameState;

int line_type(int r1, int c1, int r2, int c2);
bool adjacent (int r1, int c1, int r2, int c2);
bool check_box(GameState *state, int r, int c);
int process_move(GameState *state, int r1, int c1, int r2, int c2);
void print_board(char board[ROWS * 2 + 1][COLS * 2 + 1]);
void player_box(GameState *state, int r, int c);
void normalize_input(char *str);

#endif