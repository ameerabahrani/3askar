#include "game.h"

void claim_box(GameState *state, int r, int c, bool *boxCompleted){
    state->box_owner[r][c] = state->current_player;
    state->scores[state->current_player - 1]++;
    state->remaining_boxes--;
    *boxCompleted = true;
    player_box(state, r, c);
}

void handle_horizontal_line(GameState *state, int r1, int c1, int c2) {
    bool boxCompleted = false;
    int min_col = (c1 < c2) ? c1 : c2;

    // Check if the box above is checked to account for two boxes being completed at once
    if (r1 > 0 && state->box_owner[r1 - 1][min_col] == 0 && check_box(state, r1 - 1, min_col)) {
        claim_box(state, r1 - 1, min_col, &boxCompleted);
    }

    // Check if the box down is checked to account for two boxes being completed at once
    if (r1 < ROWS && state->box_owner[r1][min_col] == 0 && check_box(state, r1, min_col)) {
        claim_box(state, r1, min_col, &boxCompleted);
    }

    // Switch player if no boxes were completed
    if (!boxCompleted) {
        state->current_player = (state->current_player == 1) ? 2 : 1;
    }
}

void handle_vertical_line(GameState *state, int r1, int c1, int r2) {
    bool boxCompleted = false;
    int min_row = (r1 < r2) ? r1 : r2;

    // Check if the box to the left is checked to account for two boxes being completed at once
    if (c1 > 0 && state->box_owner[min_row][c1 - 1] == 0 && check_box(state, min_row, c1 - 1)) {
        claim_box(state, min_row, c1 - 1, &boxCompleted);
    }

    // Check if the box to the right is checked to account for two boxes being completed at once
    if (c1 < COLS && state->box_owner[min_row][c1] == 0 && check_box(state, min_row, c1)) {
        claim_box(state, min_row, c1, &boxCompleted);
    }

    // Switch player if no boxes were completed
    if (!boxCompleted) {
        state->current_player = (state->current_player == 1) ? 2 : 1;
    }
}

void player_box(GameState *state, int r, int c){ // print the player's letter in the box
    if (state->current_player == 1){
        state->board[2 * r + 1][2 * c + 1] = 'A';
    }
    else if (state->current_player == 2){
        state->board[2 * r + 1][2 * c + 1] = 'B';
    }
}

void print_board(char board[9][11]){ // print the board
    printf("  0 1 2 3 4 5\n");

    for (int i = 0; i < 9; i++)
    {
        if(i % 2 == 0)
            printf("%d ", i / 2);
        else
            printf("  ");
        for (int j = 0; j < 11; j++)
        {
            
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
}

int line_type(int r1, int c1, int r2, int c2){ // check if the line is horizontal or vertical
    if (r1 == r2)
        return HORIZONTAL; // horizontal
    if (c1 == c2)
        return VERTICAL; // vertical
    return -1; // invalid
}

bool adjacent (int r1, int c1, int r2, int c2){ // check if the lines are adjacent
    if (r1 == r2 && abs(c1 - c2) == 1)
        return true;
    if (c1 == c2 && abs(r1 - r2) == 1)
        return true;
    
    return false;
}

bool check_box(GameState *state, int r, int c){ // check if the box is completed
    if (r < 0 || r >= ROWS || c < 0 || c >= COLS)
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
int process_move(GameState *state, int r1, int c1, int r2, int c2){ // process the move
    if (!adjacent(r1, c1, r2, c2))
        return -1; 
    if (line_type(r1, c1, r2, c2) == -1)
        return -2;
    if (line_type(r1, c1, r2, c2) == HORIZONTAL){ // horizontal
        int min_col = (c1 < c2) ? c1 : c2;

        if (r1 < 0 || r1 > 4 || c1 < 0 || c1 > 4)
        return -4; 


        if (state->horizontal_lines[r1][min_col])
            return -3;

        state->horizontal_lines[r1][min_col] = true;
        state->board[2 * r1 ][2 * min_col + 1] = '-';
        return 0;
    }
    if (line_type(r1, c1, r2, c2) == VERTICAL){ // vertical 
        int min_row = (r1 < r2) ? r1 : r2;

        if (min_row < 0 || min_row > ROWS || c1 < 0 || c1 > COLS)
        return -4; 

        if (state->vertical_lines[min_row][c1]) 
            return -3;

        state->vertical_lines[min_row][c1] = true;
        state->board[2 * min_row + 1][2 * c1] = '|';
        return 0; 
    }
    return -2;
}

void normalize_input(char *str) {
    // Trim leading whitespace
    char *start = str;
    while (isspace(*start)) start++;
    memmove(str, start, strlen(start) + 1);

    // Trim trailing whitespace and newline
    char *end = str + strlen(str) - 1;
    while (end >= str && (isspace(*end) || *end == '\n')) end--;
    *(end + 1) = '\0';

    // Convert to lowercase
    for (char *p = str; *p; p++) *p = tolower(*p);
}

