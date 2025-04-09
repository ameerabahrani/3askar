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
#define RESET_COLOR "\033[0m"
#define BLUE_COLOR "\033[1;34m"  // Player A
#define RED_COLOR "\033[1;31m"  // Player B
#define GET_DEEP_COPY(dest, state_ptr)                        \
    do {                                                      \
        GameState* _copy_ptr = deep_copy_GameState(state_ptr);\
        if (_copy_ptr == NULL) {                              \
            fprintf(stderr, "Error: Unable to allocate memory for deep copy of GameState.\n"); \
            exit(EXIT_FAILURE);                               \
        }                                                     \
        (dest) = *_copy_ptr;                                  \
        free(_copy_ptr);                                      \
    } while (0)

typedef struct {
    char symbol;
    int color; // 0 for neutral, 1 blue for player A, 2 red for player B
} Cell;

typedef struct {
    bool horizontal_lines[ROWS + 1][COLS];
    bool vertical_lines[ROWS][COLS + 1]; 
    int box_owner[ROWS][COLS]; // 0=neutral  1/2=players
    int current_player;
    int scores[players];
    int remaining_boxes; 
    Cell board[ROWS * 2 + 1][COLS * 2 + 1]; //9×11 board for printing dots and spaces
} GameState;

typedef struct{
    int score;
    int r1, r2, c1, c2;
} Move;

enum {HORIZONTAL, VERTICAL}; // for line type

int line_type(int r1, int c1, int r2, int c2);
bool adjacent (int r1, int c1, int r2, int c2);
bool check_box(GameState *state, int r, int c);
int process_move(GameState *state, int r1, int c1, int r2, int c2);
void print_board(Cell board[ROWS * 2 + 1][COLS * 2 + 1]);
void player_box(GameState *state, int r, int c);
void normalize_input(char *str);
void easy_bot_move(GameState *state);
void medium_bot_move(GameState *state);
void hard_bot_move(GameState *state);
int simulate(GameState state, int r1, int c1, int r2, int c2);
void claim_box(GameState *state, int r, int c, bool *boxCompleted);
void handle_horizontal_line(GameState *state, int r1, int c1, int c2);
void handle_vertical_line(GameState *state, int r1, int c1, int r2);
void init_board(GameState *state);
int evaluationFunction(GameState *state);
int simulate_move(GameState *state, int r1, int c1, int r2, int c2);
Move minimax(GameState state, int depth, bool maximizingPlayer);
GameState* deep_copy_GameState(const GameState* src);

#endif