#include "game.h"

void claim_box(GameState *state, int r, int c, bool *boxCompleted){
    state->box_owner[r][c] = state->current_player;
    state->scores[state->current_player - 1]++;
    state->remaining_boxes--;
    *boxCompleted = true;
    player_box(state, r, c);

    int row = 2 * r + 1;
    int col = 2 * c + 1;
    int color = state->current_player;
    
    // Update adjacent lines (check bounds to avoid out-of-range errors)
    if (row - 1 >= 0)
        state->board[row - 1][col].color = color; // Top
    if (row + 1 < ROWS * 2 + 1)
        state->board[row + 1][col].color = color; // Bottom
    if (col - 1 >= 0)
        state->board[row][col - 1].color = color; // Left
    if (col + 1 < COLS * 2 + 1)
        state->board[row][col + 1].color = color; // Right
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
        state->board[2 * r + 1][2 * c + 1].symbol = 'A';
        state->board[2 * r + 1][2 * c + 1].color = 1; // set the color of the box to the current player
    }
    else if (state->current_player == 2){
        state->board[2 * r + 1][2 * c + 1].symbol = 'B';
        state->board[2 * r + 1][2 * c + 1].color = 2; // set the color of the box to the current player
    }
}

void print_board(Cell board[ROWS * 2 + 1][COLS * 2 + 1]) {
    int rows = ROWS * 2 + 1;
    int cols = COLS * 2 + 1;
    
    // Print the column header (0 to COLS)
    printf("  ");
    for (int j = 0; j <= COLS; j++) {
        printf("%d ", j);
    }
    printf("\n");
    
    // Print the board rows with row coordinates for even rows
    for (int i = 0; i < rows; i++) {
        if (i % 2 == 0)
            printf("%d ", i / 2);
        else
            printf("  ");
        
        for (int j = 0; j < cols; j++) {
            if (board[i][j].color == 1)
                printf(BLUE_COLOR "%c" RESET_COLOR, board[i][j].symbol);
            else if (board[i][j].color == 2)
                printf(RED_COLOR "%c" RESET_COLOR, board[i][j].symbol);
            else
                printf("%c", board[i][j].symbol);
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

/**
 * int process_move(GameState *state, int r1, int c1, int r2, int c2)
 *
 * Requires:
 *    0 <= r1, r2 < ROWS and 0 <= c1, c2 < COLS
 *    The 2 Coordinates needto be adjacent either vertically or horizontally.
 *
 * Effects;
 *    Updates GameState by marking the chosen horizontal / vertical line as taken.
 *    updates the board's representation with the symbol and player color.
 *    Does not update score or switch players
 *
 * Returns:
 *     0  move is valid
 *    -1  dots are not adjacent
 *    -2  line is invalid 
 *    -3  line already taken
 *    -4  coordinates are out of bounds
 */
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
        state->board[2 * r1 ][2 * min_col + 1].symbol = '-';
        state->board[2 * r1 ][2 * min_col + 1].color = state->current_player; // set the color of the line to the current player
        return 0;
    }
    if (line_type(r1, c1, r2, c2) == VERTICAL){ // vertical 
        int min_row = (r1 < r2) ? r1 : r2;

        if (min_row < 0 || min_row > ROWS || c1 < 0 || c1 > COLS)
        return -4; 

        if (state->vertical_lines[min_row][c1]) 
            return -3;

        state->vertical_lines[min_row][c1] = true;
        state->board[2 * min_row + 1][2 * c1].symbol = '|';
        state->board[2 * min_row + 1][2 * c1].color = state->current_player; // set the color of the line to the current player
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

void init_board(GameState *state){
    int rows = ROWS * 2 + 1;
    int cols = COLS * 2 + 1;

    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            state->board[i][j].color = 0; // neutral color

            if (i % 2 == 0){
                if (j % 2 == 0){
                    state->board[i][j].symbol = '.'; 
                }
                else{
                    state->board[i][j].symbol = ' '; // horizontal line
                }
            }
            else{
                if (j % 2 == 0){
                    state->board[i][j].symbol = ' '; 
                }
                else{
                    state->board[i][j].symbol = ' '; // empty space
                }
            }
        }
    }
}

GameState* deep_copy_GameState(const GameState* src) {
    if (src == NULL) return NULL;

    // Allocate memory for the new GameState
    GameState* dest = (GameState*)malloc(sizeof(GameState));
    if (dest == NULL) return NULL;

    // Copy primitive types
    dest->current_player = src->current_player;
    dest->remaining_boxes = src->remaining_boxes;

    // Copy arrays using memcpy (since they're contiguous memory blocks)
    memcpy(dest->horizontal_lines, src->horizontal_lines, sizeof(src->horizontal_lines));
    memcpy(dest->vertical_lines, src->vertical_lines, sizeof(src->vertical_lines));
    memcpy(dest->box_owner, src->box_owner, sizeof(src->box_owner));
    memcpy(dest->scores, src->scores, sizeof(src->scores));
    
    // Copy board (array of structs - safe to memcpy as Cell contains no pointers)
    memcpy(dest->board, src->board, sizeof(src->board));

    return dest;
}

