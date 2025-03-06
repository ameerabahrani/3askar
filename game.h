#ifndef game_h
#define game_h

#include <stdbool.h>

#define ROWS 4
#define COLS 5

typedef struct {
    bool horizontal_lines[ROWS+1][COLS];  
    bool vertical_lines[COLS+1][ROWS];  
    int box_owner[ROWS][COLS];          // 0=neutral, 1/2=players
    

    int current_player;
    int scores[2];
    int remaining_boxes;
} GameState;

int line_type(int r1, int c1, int r2, int c2);
bool adjacent (int r1, int c1, int r2, int c2);
bool check_box(GameState *state, int r, int c);
int process_move(GameState *state, int r1, int c1, int r2, int c2);

#endif