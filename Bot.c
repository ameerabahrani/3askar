#include "game.h"
#include <time.h>

/**
 * void easy_bot_move(GameState *state)
 *
 * Requires:
 *      The GameState pointer (state) should not be NULL.
 *      The random number generator should be seeded before calling this
 * function(in the main method).
 *
 * Effects:
 *      The function will play a random move for the bot.
 *      It will keep generating random moves until it finds a valid one.
 *      The function will update the GameState with the new move.
 *      The function will print the move played by the bot.
 *
 * Returns:
 *      Nothing
 *
 */
void easy_bot_move(GameState *state) { // easy bot that plays a random move

  int r1, c1, r2, c2;
  int result = -1;

  while (1) {

    int isHorizontal = rand() % 2; // 0 or 1 to choose if the bot will generate
                                   // a horizontal or vertical line

    if (isHorizontal == 1) {
      // Horizontal line
      r1 = rand() % (ROWS + 1);
      c1 = rand() % COLS;

      r2 = r1;
      c2 = c1 + 1;
    } else {
      // Vertical line
      r1 = rand() % ROWS;
      c1 = rand() % (COLS + 1);

      r2 = r1 + 1;
      c2 = c1;
    }

    result = process_move(state, r1, c1, r2, c2);

    if (result == 0) {
      int linetype = get_line_type(r1, c1, r2, c2);

      if (linetype == 0) { // Horizontal line
        handle_horizontal_line(state, r1, c1, c2);
      }
      if (linetype == 1) {
        handle_vertical_line(state, r1, c1, r2);
      }
      // valid move break out of loop
      printf("Bot played: %d %d %d %d\n", r1, c1, r2, c2);
      break;
    } else {
      // invalid move try again
      continue;
    }
  }
}

/**
 * void medium_bot_move(GameState *state)
 *
 * Requires:
 *      The GameState pointer (state) should not be NULL.
 *
 * Effects:
 *      The function will play a move for the bot that will complete the most
 * boxes. If no boxes can be completed, it will play a random move. The function
 * will update the GameState with the new move. The function will print the move
 * played by the bot.
 *
 * Returns:
 *      Nothing
 */
void medium_bot_move(GameState *state) { // medium bot that plays a move that will complete the most
                                         // boxes or random move if no boxes can be completed
  int most_boxes = 0;
  int r1, c1, r2, c2;

  bool move_with_box = false;

  for (int r = 0; r <= ROWS; r++) { // check horizontal lines
    for (int c = 0; c <= COLS - 1; c++) {
      if (state->horizontal_lines[r][c] == false) {
        int boxes_completed = simulate_box_completion_count(*state, r, c, r, c + 1);
        if (boxes_completed > most_boxes) { 
          most_boxes = boxes_completed;
          r1 = r;
          c1 = c;
          r2 = r;
          c2 = c + 1;
          move_with_box = true;
        }
      }
    }
  }
  for (int r = 0; r <= ROWS - 1; r++) { // check vertical lines
    for (int c = 0; c <= COLS; c++) {
      if (state->vertical_lines[r][c] == false) {
        int boxes_completed = simulate_box_completion_count(*state, r, c, r + 1, c);
        if (boxes_completed > most_boxes) {
          most_boxes = boxes_completed;
          r1 = r;
          c1 = c;
          r2 = r + 1;
          c2 = c;
          move_with_box = true;
        }
      }
    }
  }
  if (move_with_box) {
    int result = process_move(state, r1, c1, r2, c2);
    int linetype = get_line_type(r1, c1, r2, c2);

    if (linetype == HORIZONTAL) { // Horizontal line
      handle_horizontal_line(state, r1, c1, c2);
    }
    if (linetype == VERTICAL) { // Vertical line
      handle_vertical_line(state, r1, c1, r2);
    }
    printf("Bot played: %d %d %d %d\n", r1, c1, r2, c2);
  } else { // Fallback to random move if no boxes can be completed
    easy_bot_move(state);
  }
}

int simulate_box_completion_count(GameState state, int r1, int c1, int r2,
             int c2) { // simulate the move on a pass by value gamestate and
                       // only return the number of boxes completed
  int result = process_move(&state, r1, c1, r2, c2);

  if (result != 0){
    return 0;
  }

  int boxes_completed = 0;
  int linetype = get_line_type(r1, c1, r2, c2);

  if (linetype == HORIZONTAL) {
    int min_col = (c1 < c2) ? c1 : c2;

    if (r1 > 0 && state.box_owner[r1 - 1][min_col] == 0 &&
        check_box(&state, r1 - 1, min_col)) {
      boxes_completed++;
    }
    if (r1 < ROWS && state.box_owner[r1][min_col] == 0 &&
        check_box(&state, r1, min_col)) {
      boxes_completed++;
    }
  } else if (linetype == VERTICAL) { // Vertical Line
    int min_row = (r1 < r2) ? r1 : r2;

    if (c1 > 0 && state.box_owner[min_row][c1 - 1] == 0 &&
        check_box(&state, min_row, c1 - 1)) {
      boxes_completed++;
    }
    if (c1 < COLS && state.box_owner[min_row][c1] == 0 &&
        check_box(&state, min_row, c1)) {
      boxes_completed++;
    }
  }

  return boxes_completed;
}

void hard_bot_move(GameState *state) {
  if (state->remaining_moves > THRESHOLD) {
    medium_bot_move(state);
  } else {
    threaded_minimax_wrapper(state);
  }
}

void threaded_minimax_wrapper(GameState *state) {
  Move best_move = {INF_MIN, -1, -1, -1,-1}; // Initialize best move with very low score and invalid coordinates

  int max_moves = (ROWS + 1) * COLS + ROWS * (COLS + 1); // Maximum number of moves possible
  pthread_t threads[max_moves];
  MinMax_thread_args *thread_args_arr[max_moves];
  int thread_count = 0;

  // Horizontal moves loop 
  for (int r = 0; r <= ROWS; r++) {
    for (int c = 0; c <= COLS - 1; c++) {
      if (!state->horizontal_lines[r][c]) {
        MinMax_thread_args *args = malloc(
            sizeof(MinMax_thread_args)); // Allocate memory for thread arguments
        args->state_copy = deep_copy_GameState(state); // Deep copy of the game state
        args->r1 = r;
        args->c1 = c;
        args->r2 = r;
        args->c2 = c + 1;
        args->depth = MAX_DEPTH; // Set the depth for minimax
        args->alpha = INF_MIN;   // Initialize alpha
        args->beta = INF_MAX;    // Initialize beta
        thread_args_arr[thread_count] = args; // Store the arguments in the array
        pthread_create(&threads[thread_count], NULL, minimax_thread,
                       (void *)args); // Create the thread
        thread_count++;               // Increment the thread count
      }
    }
  }

  // Vertical moves loop (note: r goes from 0 to ROWS-1)
  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c <= COLS; c++) {
      if (!state->vertical_lines[r][c]) {
        MinMax_thread_args *args = malloc(sizeof(MinMax_thread_args)); // Allocate memory for thread arguments
        args->state_copy = deep_copy_GameState(state); // Deep copy of the game state
        args->r1 = r;
        args->c1 = c;
        args->r2 = r + 1;
        args->c2 = c;
        args->depth = MAX_DEPTH; // Set the depth for minimax
        args->alpha = INF_MIN;   // Initialize alpha
        args->beta = INF_MAX;    // Initialize beta
        thread_args_arr[thread_count] =
            args; // Store the arguments in the array
        pthread_create(&threads[thread_count], NULL, minimax_thread,
                       (void *)args); // Create the thread
        thread_count++;               // Increment the thread count
      }
    }
  }

  for (int i = 0; i < thread_count; i++) {
    pthread_join(threads[i], NULL);
    MinMax_thread_args *args = thread_args_arr[i]; // Get the thread arguments
    if (args->result.score > best_move.score) { // Check if the move is better than the current best move
      best_move = args->result; // Update the best move
    }
    free(args);
  }

  if (best_move.score != INF_MIN) { // If a valid move was found
    int result = process_move(state, best_move.r1, best_move.c1, best_move.r2,
                              best_move.c2);
    int lt = get_line_type(best_move.r1, best_move.c1, best_move.r2, best_move.c2);
    if (lt == HORIZONTAL) {
      handle_horizontal_line(state, best_move.r1, best_move.c1, best_move.c2);
    }
    if (lt == VERTICAL) {
      handle_vertical_line(state, best_move.r1, best_move.c1, best_move.r2);
    }
    printf("Bot played: %d %d %d %d\n", best_move.r1, best_move.c1,
           best_move.r2, best_move.c2);
  }
}

Move minimax(GameState state, int depth, bool maximizingPlayer, int alpha,
             int beta) {
  Move best_move;
  if (depth == 0 || state.remaining_boxes == 0) {
    best_move.score = evaluation_function(&state);
    best_move.r1 = best_move.c1 = best_move.r2 = best_move.c2 = -1;
    return best_move;
  }

  if (maximizingPlayer) {
    best_move.score = INF_MIN;
    // Horizontal moves
    for (int r = 0; r <= ROWS; r++) {
      for (int c = 0; c <= COLS - 1; c++) {
        if (!state.horizontal_lines[r][c]) {
          GameState temp_state;
          GET_DEEP_COPY(temp_state, &state);
          if (simulate_apply_move(&temp_state, r, c, r, c + 1) == 0) {
            Move curr_move =
                minimax(temp_state, depth - 1, (temp_state.current_player == 2),
                        alpha, beta);
            if (curr_move.score > best_move.score) {
              best_move.score = curr_move.score;
              best_move.r1 = r;
              best_move.c1 = c;
              best_move.r2 = r;
              best_move.c2 = c + 1;
            }
            alpha = max(alpha, best_move.score);

            if (alpha >= beta)
              goto prune_max_horizontal;
          }
        }
      }
    }
    prune_max_horizontal:
    // Vertical moves
    for (int r = 0; r < ROWS; r++) { // r goes from 0 to ROWS-1
      for (int c = 0; c <= COLS; c++) {
        if (!state.vertical_lines[r][c]) {
          GameState temp_state;
          GET_DEEP_COPY(temp_state, &state);
          if (simulate_apply_move(&temp_state, r, c, r + 1, c) == 0) {
            Move curr_move =
                minimax(temp_state, depth - 1, (temp_state.current_player == 2),
                        alpha, beta);
            if (curr_move.score > best_move.score) {
              best_move.score = curr_move.score;
              best_move.r1 = r;
              best_move.c1 = c;
              best_move.r2 = r + 1;
              best_move.c2 = c;
            }
            alpha = max(alpha, best_move.score);

            if (alpha >= beta)
              goto prune_max_vertical;
          }
        }
      }
    }
    prune_max_vertical:
    return best_move;
  } else {                     // Minimizing player's turn
    best_move.score = INF_MAX; 
    // Horizontal moves
    for (int r = 0; r <= ROWS; r++) {
      for (int c = 0; c <= COLS - 1; c++) {
        if (!state.horizontal_lines[r][c]) {
          GameState temp_state;
          GET_DEEP_COPY(temp_state, &state);
          if (simulate_apply_move(&temp_state, r, c, r, c + 1) == 0) {
            Move curr_move =
                minimax(temp_state, depth - 1, (temp_state.current_player == 2),
                        alpha, beta);
            if (curr_move.score < best_move.score) {
              best_move.score = curr_move.score;
              best_move.r1 = r;
              best_move.c1 = c;
              best_move.r2 = r;
              best_move.c2 = c + 1;
            }
            beta = min(beta, best_move.score);

            if (beta <= alpha)
              goto prune_min_horizontal;
          }
        }
      }
    }
    prune_min_horizontal:
    // Vertical moves
    for (int r = 0; r < ROWS; r++) {
      for (int c = 0; c <= COLS; c++) {
        if (!state.vertical_lines[r][c]) {
          GameState temp_state;
          GET_DEEP_COPY(temp_state, &state);
          if (simulate_apply_move(&temp_state, r, c, r + 1, c) == 0) {
            Move curr_move =
                minimax(temp_state, depth - 1, (temp_state.current_player == 2),
                        alpha, beta);
            if (curr_move.score < best_move.score) {
              best_move.score = curr_move.score;
              best_move.r1 = r;
              best_move.c1 = c;
              best_move.r2 = r + 1;
              best_move.c2 = c;
            }
            beta = min(beta, best_move.score);

            if (beta <= alpha)
              goto prune_min_vertical;
          }
        }
      }
    }
    prune_min_vertical:
    return best_move;
  }
}

/**
 * int simulate_apply_move(GameState *state, int r1, int c1, int r2, int c2)
 *
 * Requires:
 *      The GameState pointer (state) should not be NULL.
 *
 * Effects:
 *      The function will simulate a move on the game state.
 *      It will update the game state with the new move.
 *
 * Returns:
 *      0 if the move is valid and completed boxes
 *      -1 if the dots are not adjacent
 *      -2 if the line is invalid
 *      -3 if the line is already taken
 *      -4 if the coordinates are out of bounds
 */
int simulate_apply_move(GameState *state, int r1, int c1, int r2, int c2) {
  int result = process_move(state, r1, c1, r2, c2);
  if (result != 0)
    return result;

  int linetype = get_line_type(r1, c1, r2, c2);
  if (linetype == HORIZONTAL) { 
    handle_horizontal_line(state, r1, c1, c2);
  }
  if (linetype == VERTICAL) { 
    handle_vertical_line(state, r1, c1, r2);
  }
  return 0;
}

void *minimax_thread(void *arguments) {
  MinMax_thread_args *arg = (MinMax_thread_args *)arguments;

  simulate_apply_move(arg->state_copy, arg->r1, arg->c1, arg->r2, arg->c2);

  bool isMaximizing = (arg->state_copy->current_player == 2);

  arg->result = minimax(*arg->state_copy, arg->depth, isMaximizing, arg->alpha,
                        arg->beta);

  free(arg->state_copy);

  pthread_exit(NULL);
  return NULL;
}

/**
 * int evaluation_function(GameState *state)
 *
 * Requires:
 *      The GameState pointer (state) should not be NULL.
 *
 * Effects:
 *      The function will evaluate the current state of the game.
 *
 * Returns:
 *      The difference between the scores of player 2 and player 1.
 */
int evaluation_function(GameState *state) {
  return state->scores[1] - state->scores[0];
}