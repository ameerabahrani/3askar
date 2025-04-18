#ifndef GAME_H
#define GAME_H

#include <ctype.h>
#include <limits.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { HORIZONTAL, VERTICAL }; // for line type

#define ROWS 4
#define COLS 5
#define MAX_DEPTH 5             // Max depth for minimax algorithm
#define PLAYERS 2
#define THRESHOLD 20            // Threshold for switching to hard mode
#define RESET_COLOR "\033[0m"   // Reset color
#define BLUE_COLOR "\033[1;34m" // Player A
#define RED_COLOR "\033[1;31m"  // Player B
#define INF_MAX INT_MAX         // Infinity
#define INF_MIN INT_MIN         // Negative Infinity
#define GET_DEEP_COPY(dest, state_ptr)                                        \
  do {                                                                         \
    GameState *_copy_ptr = deep_copy_GameState(state_ptr);                     \
    if (_copy_ptr == NULL) {                                                   \
      fprintf(                                                                 \
          stderr,                                                              \
          "Error: Unable to allocate memory for deep copy of GameState.\n");   \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
    (dest) = *_copy_ptr;                                                       \
    free(_copy_ptr);                                                           \
  } while (0)
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

typedef struct {
  char symbol;
  int color; // 0 for neutral, 1 blue for player A, 2 red for player B
} Cell;

typedef struct {
  bool horizontal_lines[ROWS + 1][COLS];
  bool vertical_lines[ROWS][COLS + 1];
  int box_owner[ROWS][COLS]; // 0=neutral  1/2=players
  int current_player;
  int scores[PLAYERS];
  int remaining_boxes;
  int  remaining_moves;
  Cell board[ROWS * 2 + 1]
            [COLS * 2 + 1]; // 9×11 board for printing dots and spaces
} GameState;

typedef struct {
  int score;
  int r1, r2, c1, c2;
} Move;

typedef struct {
  GameState *state_copy;
  int r1, c1, r2, c2;
  int depth;
  bool maximizingPlayer;
  int alpha, beta;
  Move result;
} MinMax_thread_args;

// ------------------------ Logic.c ------------------------
int get_line_type(int r1, int c1, int r2, int c2);
bool adjacent(int r1, int c1, int r2, int c2);
bool check_box(GameState *state, int r, int c);
int process_move(GameState *state, int r1, int c1, int r2, int c2);
void print_board(Cell board[ROWS * 2 + 1][COLS * 2 + 1]);
void print_box(GameState *state, int r, int c);
void normalize_input(char *str);
void claim_box(GameState *state, int r, int c, bool *boxCompleted);
void handle_horizontal_line(GameState *state, int r1, int c1, int c2);
void handle_vertical_line(GameState *state, int r1, int c1, int r2);

// ------------------------ Bot.c ---------------------------
void easy_bot_move(GameState *state);
void medium_bot_move(GameState *state);
void hard_bot_move(GameState *state);
void threaded_minimax_wrapper(GameState *state);
int simulate_box_completion_count(GameState state, int r1, int c1, int r2, int c2);
void init_board(GameState *state);
int evaluation_function(GameState *state);
int simulate_apply_move(GameState *state, int r1, int c1, int r2, int c2);
Move minimax(GameState state, int depth, bool maximizingPlayer, int alpha,
             int beta);
GameState *deep_copy_GameState(const GameState *src);
void *minimax_thread(void *arguments);

#endif