#include "game.h"
#include <time.h> // required for random seed initialization

// Handles game initialization, player/bot moves, and game termination.
int main() {
  // Initialize the game state with all lines unmarked and boxes unclaimed
  GameState game = {
      .horizontal_lines = {[0 ... ROWS] = {[0 ... COLS - 1] = false}},
      .vertical_lines = {[0 ... ROWS - 1] = {[0 ... COLS] = false}},
      .box_owner = {[0 ... ROWS - 1] = {[0 ... COLS - 1] = 0}},
      .current_player = 1,
      .scores = {0, 0},
      .remaining_boxes = ROWS * COLS,
      .remaining_moves = (ROWS+1) * COLS + ROWS * (COLS+1),
  };
  init_board(&game); // Set up initial board visualization

  /** Game mode selection loop
   * loops until a valid mode is selected or user chooses to exit. 
    */ 
  int mode;
  while (1) { 
    printf("Choose game mode:\n");
    printf("(1) Two Players (Human vs. Human)\n");
    printf("(2) Human vs. Bot (Easy Level)\n");
    printf("(3) Human vs. Bot (Medium Level)\n");
    printf("(4) Human vs. Bot (Hard Level)\n");

    char modeInput[256];
    if (fgets(modeInput, sizeof(modeInput), stdin) == NULL) {
      printf("Error reading input. Exiting.\n");
      return 1;
    }
    // Parsing, valid input is integer 1-4
    if (sscanf(modeInput, "%d", &mode) == 1 &&
        (mode == 1 || mode == 2 || mode == 3 || mode == 4)) {
      break;
    } else {
      printf("Invalid input. Please enter 1, 2, 3, 4.\n");
    }
  }

  srand(time(NULL));  // Seed the random generator for bot

  if (mode > 1) {
    char first_input[16];
    int who_first = 0;
    printf("Who should play first?\n");
    printf("(1) User \n");
    printf("(2) Bot\n");

    if (fgets(first_input, sizeof(first_input), stdin) == NULL) {
      printf("Error reading input. Exiting.\n");
      return 1;
    }

    if (sscanf(first_input, "%d", &who_first) == 1 &&
        (who_first == 1 || who_first == 2)) {
      game.current_player = who_first;
    } else {
      printf("Invalid input. Please enter 1 or 2.\n");
    }

    if (who_first == 2) { 
      game.current_player = 2; 
    }

  }

  // Main game loop - continues until all boxes are claimed
  while (game.remaining_boxes > 0) { 
    print_board(game.board);

    if (game.current_player == 1) { // Human player turn, human is always player A
      printf(BLUE_COLOR "Player A's " RESET_COLOR
                        "turn. Enter the row and column of the first dot "
                        "(e.g., A0 -> 0 0) and second dot or \"exit\":\n");

      char input[256];
      if (fgets(input, sizeof(input), stdin) == NULL) {
        break;
      }

      normalize_input(input); // Standardize input format

      if (strcmp(input, "exit") == 0) {
        break;
      }

      int r1, c1, r2, c2;
      if (sscanf(input, "%d %d %d %d", &r1, &c1, &r2, &c2) != 4) { // Check if the input is valid
        printf("Invalid input. Please enter 4 integers.\n");
      } else {
        // Process the move and check if it's valid
        int result = process_move(&game, r1, c1, r2, c2); // Check if the move is valid and update the bool arrays
        int linetype = get_line_type(r1, c1, r2, c2);
        switch (result) {
        case 0: // Valid move
          if (linetype == HORIZONTAL) { 
            handle_horizontal_line(&game, r1, c1, c2);
          }
          if (linetype == VERTICAL) { 
            handle_vertical_line(&game, r1, c1, r2);
          }
          break;
        case -1:
          printf("Not adjacent\n");
          break;
        case -2:
          printf("Invalid line\n");
          break;
        case -3:
          printf("Line already taken\n");
          break;
        case -4:
          printf("Invalid coordinates\n");
          break;
        }
      }
    } else { // Player B turn (Bot or Human)
      if (mode == 1) { // Human player B
        printf(RED_COLOR "Player B's " RESET_COLOR
                         "turn. Enter the row and column of the first dot "
                         "(e.g., A0 -> 0 0) and second dot or \"exit\":\n");

        char input[256];
        if (fgets(input, sizeof(input), stdin) == NULL) {
          break;
        }

        normalize_input(input);

        if (strcmp(input, "exit") == 0) {
          break;
        }

        int r1, c1, r2, c2;
        if (sscanf(input, "%d %d %d %d", &r1, &c1, &r2, &c2) != 4) {
          printf("Invalid input. Please enter 4 integers.\n");
        } else {
          int result = process_move(&game, r1, c1, r2, c2); // Check if the move is valid and update the bool arrays
          int linetype = get_line_type(r1, c1, r2, c2);
          switch (result) {
          case 0:
            if (linetype == HORIZONTAL) { 
              handle_horizontal_line(&game, r1, c1, c2);
            }
            if (linetype == VERTICAL) { 
              handle_vertical_line(&game, r1, c1, r2);
            }
            break;
          case -1:
            printf("Not adjacent\n");
            break;
          case -2:
            printf("Invalid line\n");
            break;
          case -3:
            printf("Line already taken\n");
            break;
          case -4:
            printf("Invalid coordinates\n");
            break;
          }
        }
      } else if (mode == 2) { // Easy bot, Random moves
        printf(RED_COLOR "Player B (Bot)" RESET_COLOR " is thinking...\n");
        easy_bot_move(&game);
      } else if (mode == 3) { // Medium bot, Completes boxes when possible
        printf(RED_COLOR "Player B (Bot)" RESET_COLOR " is thinking...\n");
        medium_bot_move(&game);
      } else if (mode == 4) { // Hard bot
        int total_initial_moves = (ROWS+1)*COLS + ROWS*(COLS+1); // 49 for ROWS=4, COLS=5
        int moves_made = total_initial_moves - game.remaining_moves;
    
        if (moves_made < 20) { 
            printf(RED_COLOR "Player B (Bot)" RESET_COLOR " is thinking...\n");
            medium_bot_move(&game);
        } else {
            printf(RED_COLOR "Player B (Bot)" RESET_COLOR " is thinking...\n");
            hard_bot_move(&game);
        }
      }
    }

    // Display updated scores after each move
    printf("********************************************************************************\n");
    printf(BLUE_COLOR"                            Player A score: %d\n" RESET_COLOR, game.scores[0]);
    printf(RED_COLOR"                            Player B score: %d\n" RESET_COLOR, game.scores[1]);
    printf("********************************************************************************\n");
  }

  // Game over - display final scores and winner
  int winner = (game.scores[0] > game.scores[1]) ? 1 : 2;
  print_board(game.board);
  printf("Game over! %s", (game.scores[0] == game.scores[1])? "Tie!\n"
             : ((winner == 1) ? BLUE_COLOR "Player A wins!\n" RESET_COLOR
                              : RED_COLOR "Player B wins!\n" RESET_COLOR));

  return 0;
}