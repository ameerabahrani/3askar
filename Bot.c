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
 *      boxes. If no boxes can be completed, it will play a random move. The function
 *      will update the GameState with the new move. The function will print the move
 *      played by the bot.
 *
 * Returns:
 *      Nothing
 */
void medium_bot_move(GameState *state) { // medium bot that plays a move that will complete the most
                                         // boxes or random move if no boxes can be completed

  int most_boxes = 0;
  int r1, c1, r2, c2;

  bool move_with_box = false;

  pthread_mutex_t lock;
  pthread_mutex_init(&lock, NULL); // Initialize the mutex lock

  pthread_t threads[2];
  MediumBotThreadArgs args[2];

  args[0] = (MediumBotThreadArgs) {
      .state = *state,
      .best_boxes = &most_boxes,
      .best_r1 = &r1,
      .best_c1 = &c1,
      .best_r2 = &r2,
      .best_c2 = &c2,
      .found_move = &move_with_box,
      .lock = &lock,
      .is_horizontal = true
    }; // Horizontal move

  args[1] = (MediumBotThreadArgs) {
      .state = *state,
      .best_boxes = &most_boxes,
      .best_r1 = &r1,
      .best_c1 = &c1,
      .best_r2 = &r2,
      .best_c2 = &c2,
      .found_move = &move_with_box,
      .lock = &lock,
      .is_horizontal = false
    }; // Vertical move

  pthread_create(&threads[0], NULL, medium_bot_thread, (void *)&args[0]);
  pthread_create(&threads[1], NULL, medium_bot_thread, (void *)&args[1]);

  pthread_join(threads[0], NULL);
  pthread_join(threads[1], NULL);

  pthread_mutex_destroy(&lock);

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

/**
 * void* medium_bot_thread(void* args)
 *
 * Requires:
 *      The GameState pointer (state) should not be NULL.
 *
 * Effects:
 *      The function will play a move for the bot that will complete the most
 *      boxes. If no boxes can be completed, it will play a random move. The function
 *      will update the GameState with the new move. The function will print the move
 *      played by the bot.
 *
 * Returns:
 *      Nothing
 */
void* medium_bot_thread(void* args){
  MediumBotThreadArgs* arg = (MediumBotThreadArgs*) args;
  int local_most_boxes = 0;
  int local_r1, local_c1, local_r2, local_c2;
  bool local_move_with_box = false;

  if (arg->is_horizontal){
    for (int r = 0; r <= ROWS; r++) {
      for (int c = 0; c <= COLS - 1; c++) {
        if (arg->state.horizontal_lines[r][c] == false) {
          int boxes_completed = simulate_box_completion_count(arg->state, r, c, r, c + 1);
          if (boxes_completed > local_most_boxes) {
            local_most_boxes = boxes_completed;
            local_r1 = r;
            local_c1 = c;
            local_r2 = r;
            local_c2 = c + 1;
            local_move_with_box = true;
          }
        }
      }
    }
  } else {
    for (int r = 0; r <= ROWS - 1; r++) {
      for (int c = 0; c <= COLS; c++) {
        if (arg->state.vertical_lines[r][c] == false) {
          int boxes_completed = simulate_box_completion_count(arg->state, r, c, r + 1, c);
          if (boxes_completed > local_most_boxes) {
            local_most_boxes = boxes_completed;
            local_r1 = r;
            local_c1 = c;
            local_r2 = r + 1;
            local_c2 = c;
            local_move_with_box = true;
          }
        }
      }
    }
  }

  pthread_mutex_lock(arg->lock); // Lock the mutex to update shared variables
  if (local_most_boxes > *arg->best_boxes) {
    *arg->best_boxes = local_most_boxes;
    *arg->best_r1 = local_r1;
    *arg->best_c1 = local_c1;
    *arg->best_r2 = local_r2;
    *arg->best_c2 = local_c2;
    *arg->found_move = local_move_with_box; // Update the found move status
  }
  pthread_mutex_unlock(arg->lock); // Unlock the mutex
  return NULL; 
}

/**
 * int simulate_box_completion_count(GameState state, int r1, int c1, int r2,
 * int c2)
 *
 * Requires:
 *      The GameState pointer (state) should not be NULL.
 *
 * Effects:
 *      The function will simulate a move on the game state.
 *      It will check how many boxes are completed by this move.
 *
 * Returns:
 *      The number of boxes completed by the move.
 */
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
  Move best_move = {INF_MIN, -1, -1, -1, -1}; // Initialize best move with very low score and invalid coordinates

  // Horizontal moves loop
  for (int r = 0; r <= ROWS; r++) { 
      for (int c = 0; c <= COLS - 1; c++) {
          if (!state->horizontal_lines[r][c]) {
              GameState temp_state = *state;
              bool box_completed;
              if (simulate_apply_move(&temp_state, r, c, r, c + 1, &box_completed) == 0) {
                  Move curr_move = minimax(temp_state, MAX_DEPTH, (temp_state.current_player == 2), INF_MIN, INF_MAX); 
                  if (curr_move.score > best_move.score) {
                      best_move.score = curr_move.score;
                      best_move.r1 = r;
                      best_move.c1 = c;
                      best_move.r2 = r;
                      best_move.c2 = c + 1;
                  }
              }
          }
      }
  }

  // Vertical moves loop 
  for (int r = 0; r < ROWS; r++) {
      for (int c = 0; c <= COLS; c++) {
          if (!state->vertical_lines[r][c]) {
            GameState temp_state = *state;
              bool box_completed;
              if (simulate_apply_move(&temp_state, r, c, r + 1, c, &box_completed) == 0) {
                  Move curr_move = minimax(temp_state, MAX_DEPTH, (temp_state.current_player == 2), INF_MIN, INF_MAX); 
                  if (curr_move.score > best_move.score) {
                      best_move.score = curr_move.score;
                      best_move.r1 = r;
                      best_move.c1 = c;
                      best_move.r2 = r + 1;
                      best_move.c2 = c;
                  }
              }
          }
      }
  }

  if (best_move.score != INF_MIN) { // If a valid move was found
      int result = process_move(state, best_move.r1, best_move.c1, best_move.r2, best_move.c2);
      int lt = get_line_type(best_move.r1, best_move.c1, best_move.r2, best_move.c2);
      if (lt == HORIZONTAL) {
          handle_horizontal_line(state, best_move.r1, best_move.c1, best_move.c2);
      }
      if (lt == VERTICAL) {
          handle_vertical_line(state, best_move.r1, best_move.c1, best_move.r2);
      }
      printf("Bot played: %d %d %d %d\n", best_move.r1, best_move.c1, best_move.r2, best_move.c2);
  } 
}

/**
 * int simulate_apply_move(GameState *state, int r1, int c1, int r2, int c2, bool *box_completed)
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
int simulate_apply_move(GameState *state, int r1, int c1, int r2, int c2, bool *box_completed) {
  int original_score = state->scores[state->current_player - 1];
  int result = process_move(state, r1, c1, r2, c2);
  if (result != 0) { 
    *box_completed = false;
    return result;
  }

  int lt = get_line_type(r1, c1, r2, c2);
  if (lt == HORIZONTAL) { 
    handle_horizontal_line(state, r1, c1, c2);
  }
  if (lt == VERTICAL) { 
    handle_vertical_line(state, r1, c1, r2);
  }

  *box_completed = (state->scores[state->current_player - 1] > original_score);
  return 0;
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

/**
 * Move minimax(GameState state, int depth, bool maximizingPlayer, int alpha,
 * int beta)
 *
 * Requires:
 *      The GameState pointer (state) should not be NULL.
 *
 * Effects:
 *      The function will evaluate the current state of the game using the
 *      minimax algorithm with alpha-beta pruning.
 *
 * Returns:
 *      The best move for the current player.
 */
Move minimax(GameState state, int depth, bool maximizingPlayer, int alpha, int beta) {
  if (depth == 0 || state.remaining_boxes == 0) {
      Move move;
      move.score = evaluation_function(&state);
      move.r1 = move.c1 = move.r2 = move.c2 = -1;
      return move;
  }

  Move best_move;
  best_move.score = maximizingPlayer ? INF_MIN : INF_MAX;
  best_move.r1 = best_move.c1 = best_move.r2 = best_move.c2 = -1;

  // Horizontal moves
  for (int r = 0; r <= ROWS; r++) {
      for (int c = 0; c <= COLS - 1; c++) {
          if (state.horizontal_lines[r][c]) continue;

          GameState temp_state = state;

          bool box_completed;
          int result = simulate_apply_move(&temp_state, r, c, r, c+1, &box_completed);
          
          if (result == 0) {
              bool new_maximizing = maximizingPlayer;
              if (!box_completed) {
                  new_maximizing = !maximizingPlayer; // Switch player
              }
              
              Move curr_move = minimax(temp_state, depth - 1, new_maximizing, alpha, beta);

              if ((maximizingPlayer && curr_move.score > best_move.score) ||
                  (!maximizingPlayer && curr_move.score < best_move.score)) {
                  best_move = curr_move;
                  best_move.r1 = r;
                  best_move.c1 = c;
                  best_move.r2 = r;
                  best_move.c2 = c + 1;
              }

              if (maximizingPlayer) {
                  alpha = max(alpha, best_move.score);
              } else {
                  beta = min(beta, best_move.score);
              }
          }

          if (alpha >= beta) goto prune;
      }
  }
  
  for (int r = 0; r < ROWS; r++) {
      for (int c = 0; c <= COLS; c++) {
          if (state.vertical_lines[r][c]) continue;

          GameState temp_state = state;

          bool box_completed;
          int result = simulate_apply_move(&temp_state, r, c, r + 1, c, &box_completed);
          
          if (result == 0) {
              bool new_maximizing = maximizingPlayer;
              if (!box_completed) {
                  new_maximizing = !maximizingPlayer; // Switch player
              }

              Move curr_move = minimax(temp_state, depth - 1, new_maximizing, alpha, beta);

              if ((maximizingPlayer && curr_move.score > best_move.score) ||
                  (!maximizingPlayer && curr_move.score < best_move.score)) {
                  best_move = curr_move;
                  best_move.r1 = r;
                  best_move.c1 = c;
                  best_move.r2 = r + 1;
                  best_move.c2 = c;
              }

              if (maximizingPlayer) {
                  alpha = max(alpha, best_move.score);
              } else {
                  beta = min(beta, best_move.score);
              }
          }
          if (alpha >= beta) goto prune;
      }
  }
prune:
  return best_move;
}