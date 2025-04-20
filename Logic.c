#include "game.h"

/**
 * void claim_box(GameState *state, int r, int c, bool *box_completed)
 *
 * Requires
 *    (r, c) must be a valid box coordinate
 *    (state) needs to be initialized and cant be NULL
 *    (box_completed) must point to a valid boolean

 * Effects
 *    Checks whether all four lines around box have been drawn
 *    1)If box is completed ( closed from 4 sides):
 *    Assigns the box to the current player
 *    change the visual board and update the player score
 *    Sets the pointer *box_completed = true
 *    2)If box is not completed:
 *    Does not update the visual board and does not update player score
 *    sets the pointer *box_completed = false
 * Returns
 *   Nothing
 */
void claim_box(GameState *state, int r, int c, bool *box_completed) {
  state->box_owner[r][c] = state->current_player;
  state->scores[state->current_player - 1]++;
  state->remaining_boxes--;
  *box_completed = true;
  print_box(state, r, c);

  // Update adjacent line colors
  int row = 2 * r + 1;
  int col = 2 * c + 1;
  int color = state->current_player;

  // Update adjacent lines (check bounds to avoid out-of-range errors)
  if (row - 1 >= 0) {
    state->board[row - 1][col].color = color; // Top
  }
  if (row + 1 < ROWS * 2 + 1) {
    state->board[row + 1][col].color = color; // Bottom
  }
  if (col - 1 >= 0) {
    state->board[row][col - 1].color = color; // Left
  }
  if (col + 1 < COLS * 2 + 1) {
    state->board[row][col + 1].color = color; // Right
  }
}
/**
 * void handle_horizontal_line(GameState *state, int r1, int c1, int c2)
 *
 * Requires:
 *    The coordinates (r1, c1) and (r1, c2) must be a valid horizontal line
 *     The move must already be validated and not already drawn
 *
 * Effects:
 *    Handles horizontal line placement.
 *    Checks if one or two boxes were completed by this move
 *    Assigns completed boxes to the current player and adds it to their score
 *    Switches to the other player if no box was taken
 *
 * Returns:
 *   Nothing
 */
void handle_horizontal_line(GameState *state, int r1, int c1, int c2) {
  bool box_completed = false;
  int min_col = (c1 < c2) ? c1 : c2;

  // Check adjacent boxes to the left and right of the horizontal line
  if (r1 > 0 && state->box_owner[r1 - 1][min_col] == 0 &&
      check_box(state, r1 - 1, min_col)) {
    claim_box(state, r1 - 1, min_col, &box_completed);
  }

  // Check if the box down is checked to account for two boxes being completed at once
  if (r1 < ROWS && state->box_owner[r1][min_col] == 0 &&
      check_box(state, r1, min_col)) {
    claim_box(state, r1, min_col, &box_completed);
  }

  // Switch player if no boxes were completed
  if (!box_completed) {
    state->current_player = (state->current_player == 1) ? 2 : 1;
  }
}
/**
 * void handle_vertical_line(GameState *state, int r1, int c1, int r2)
 *
 * Requires:
 *    The coordinates (r1, c1) and (r2, c1) must form a valid vertical line
 *    The move must already be validated and not already drawn
 *
 * Effects
 *    Handles vertical line placement.
 *    Checks if one or at maximum two boxes were completed by this move
 *    Assigns completed boxes to the current player and changes their score
 *    Switches to the other player if no box was completed
 * Returns
 *  Nothing.
 */
void handle_vertical_line(GameState *state, int r1, int c1, int r2) {
  bool box_completed = false;
  int min_row = (r1 < r2) ? r1 : r2;

  // Check if the box to the left is checked to account for two boxes being
  // completed at once
  if (c1 > 0 && state->box_owner[min_row][c1 - 1] == 0 &&
      check_box(state, min_row, c1 - 1)) {
    claim_box(state, min_row, c1 - 1, &box_completed);
  }

  // Check if the box to the right is checked to account for two boxes being
  // completed at once
  if (c1 < COLS && state->box_owner[min_row][c1] == 0 &&
      check_box(state, min_row, c1)) {
    claim_box(state, min_row, c1, &box_completed);
  }

  // Switch player if no boxes were completed
  if (!box_completed) {
    state->current_player = (state->current_player == 1) ? 2 : 1;
  }
}
/**
 * void print_box(GameState *state, int r, int c)

 * Requires
 *   state must be initialized (should not be null)
 *   (r, c) must refer to a valid box on the board
 *
 * Effects
 *   Updates visual representation of a claimed box.
 *
 * Returns
 *    Nothing
 */
void print_box(GameState *state, int r, int c) {
  if (state->current_player == 1) {
    state->board[2 * r + 1][2 * c + 1].symbol = 'A'; // print the player's letter in the box
    state->board[2 * r + 1][2 * c + 1].color = 1; // Set the color of the box to the current player
  } else if (state->current_player == 2) {
    state->board[2 * r + 1][2 * c + 1].symbol = 'B';
    state->board[2 * r + 1][2 * c + 1].color = 2;
  }
}

/**
 * void print_board(Cell board[ROWS * 2 + 1][COLS * 2 + 1])
 *
 * Requires:
 *   The board must be fully initialized with Cell objects
 *
 * Effects
 *    Updates current game board with coordinates, horizontal/vertical lines and box ownership
 *    Applies color formatting.
 *
 * Returns
 *    Nothing
 */
void print_board(Cell board[ROWS * 2 + 1][COLS * 2 + 1]) {
  int rows = ROWS * 2 + 1;
  int cols = COLS * 2 + 1;

  // Print the column header 
  printf("  ");
  for (int j = 0; j <= COLS; j++) {
    printf("%d ", j);
  }
  printf("\n");

  // Print each row
  for (int i = 0; i < rows; i++) {
    if (i % 2 == 0) {
      printf("%d ", i / 2);
    } else {
      printf("  ");
    }

    for (int j = 0; j < cols; j++) {
      if (board[i][j].color == 1) {
        printf(BLUE_COLOR "%c" RESET_COLOR, board[i][j].symbol);
      } else if (board[i][j].color == 2) {
        printf(RED_COLOR "%c" RESET_COLOR, board[i][j].symbol);
      } else {
        printf("%c", board[i][j].symbol);
      }
    }
    printf("\n");
  }
}

/**
 * int get_line_type(int r1, int c1, int r2, int c2)
 *
 * Requires:
 *   All coordinates must be within the board bounds
 *
 * Effects:
 *   Analyzes the position of the two dots
 *   Determines if the move is horizontal or vertical
 *
 * Returns:
 *    HORIZONTAL  the move is a valid horizontal line
 *    VERTICAL    if the move is a valid vertical line
 *    -1           move is invalid
 */
int get_line_type(int r1, int c1, int r2, int c2) { 
  if (r1 == r2) {
    return HORIZONTAL;
  }
  if (c1 == c2) {
    return VERTICAL;
  }
  return -1;
}

/**
 * bool adjacent(int r1, int c1, int r2, int c2)
 *
 * Requires:
 *   All coordinates must be within the board bounds
 *
 * Effects:
 *   Checks if the two coordinates are adjacent either vertically or horizontally
 *
 * Returns:
 *    true  if the coordinates are adjacent
 *    false if they are not adjacent
 */
bool adjacent(int r1, int c1, int r2, int c2) { 
  if (r1 == r2 && abs(c1 - c2) == 1) {
    return true;
  }
  if (c1 == c2 && abs(r1 - r2) == 1) {
    return true;
  }

  return false;
}

/**
 * bool check_box(GameState *state, int r, int c)
 *
 * Requires:
 *   The coordinates (r, c) must be a valid box coordinate
 *   The GameState pointer (state) should not be NULL.
 *
 * Effects:
 *   Checks if the box at (r, c) is completed by checking the lines around it
 *
 * Returns:
 *    true  if the box is completed
 *    false if the box is not completed
 */
bool check_box(GameState *state, int r, int c) { 
  if (r < 0 || r >= ROWS || c < 0 || c >= COLS) {
    return false;
  }
  if (state->horizontal_lines[r][c] && state->horizontal_lines[r + 1][c] &&
      state->vertical_lines[r][c] && state->vertical_lines[r][c + 1]) {
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
 *    Processes the move by checking if the line is valid and not already taken, updates game state.
 *
 * Returns:
 *     0  move is valid
 *    -1  dots are not adjacent
 *    -2  line is invalid
 *    -3  line already taken
 *    -4  coordinates are out of bounds
 */
int process_move(GameState *state, int r1, int c1, int r2, int c2) { // process the move
  if (!adjacent(r1, c1, r2, c2)) {
    return -1;
  }
  int lt = get_line_type(r1, c1, r2, c2);
  if (lt == -1) {
    return -2;
  }
  if (lt == HORIZONTAL) { 
    int min_col = (c1 < c2) ? c1 : c2;

    if (r1 < 0 || r1 > ROWS || r2 < 0 || r2 > ROWS || c1 < 0 || c1 > COLS ||
        c2 < 0 || c2 > COLS) {
      return -4;
    }

    if (state->horizontal_lines[r1][min_col]) {
      return -3;
    }

    state->horizontal_lines[r1][min_col] = true;
    state->remaining_moves--;
    state->board[2 * r1][2 * min_col + 1].symbol = '-';
    state->board[2 * r1][2 * min_col + 1].color = state->current_player; // set the color of the line to the current player
    return 0;
  }
  if (lt == VERTICAL) { 
    int min_row = (r1 < r2) ? r1 : r2;

    if (r1 < 0 || r1 > ROWS || r2 < 0 || r2 > ROWS || c1 < 0 || c1 > COLS ||
        c2 < 0 || c2 > COLS) {
      return -4;
    }

    if (state->vertical_lines[min_row][c1])
      return -3;

    state->vertical_lines[min_row][c1] = true;
    state->remaining_moves--;
    state->board[2 * min_row + 1][2 * c1].symbol = '|';
    state->board[2 * min_row + 1][2 * c1].color = state->current_player; // set the color of the line to the current player
    return 0;
  }
  return -2;
}

/**
 * void normalize_input(char *str)
 *
 * Requires:
 *   str must point to a null character array
 *
 * Effects
 *    Trims newline characters from the input.
 *    Converts all uppercase letters to lowercase.
 *    Option to  trim leading/trailing spaces.
 *
 * Returns:
 *  Nothing
 */
void normalize_input(char *str) {
  // Trim leading whitespace
  char *start = str;
  while (isspace(*start)) {
    start++;
  }
  memmove(str, start, strlen(start) + 1);

  // Trim trailing whitespace and newline
  char *end = str + strlen(str) - 1;
  while (end >= str && (isspace(*end) || *end == '\n')) {
    end--;
  }
  *(end + 1) = '\0';

  // Convert to lowercase
  for (char *p = str; *p; p++) {
    *p = tolower(*p);
  }
}

/**
 * void init_board(GameState *state)
 *
 * Requires:
 *    The gamestate pointer(state) should not be NULL.
 *    Every field inside (state) is assumed to be allocated properly.
 *
 * Effects:
 *    Initializes the visual board with dots in their positions
 *    Clears any previous line symbols or box ownerships from the board for new round
 *
 * Returns:
 *   Nothing
 */
void init_board(GameState *state) {
  int rows = ROWS * 2 + 1;
  int cols = COLS * 2 + 1;

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      state->board[i][j].color = 0; // neutral color

      if (i % 2 == 0) {
        if (j % 2 == 0) {
          state->board[i][j].symbol = '.';
        } else {
          state->board[i][j].symbol = ' '; // horizontal line
        }
      } else {
        if (j % 2 == 0) {
          state->board[i][j].symbol = ' ';
        } else {
          state->board[i][j].symbol = ' '; // empty space
        }
      }
    }
  }
}