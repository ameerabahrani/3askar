#include "game.h"
#include <time.h> // here again to fix bug

int main() {                    
    GameState game = {
        .horizontal_lines = {[0 ... ROWS] = {[0 ... COLS - 1] = false}}, 
        .vertical_lines = {[0 ... ROWS - 1] = {[0 ... COLS] = false}},
        .box_owner = {[0 ... ROWS - 1] = {[0 ... COLS - 1] = 0}},
        .current_player = 1,
        .scores = {0, 0},
        .remaining_boxes = ROWS * COLS,
        .board = {
            {'.', ' ', '.', ' ', '.', ' ', '.', ' ', '.', ' ', '.'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'.', ' ', '.', ' ', '.', ' ', '.', ' ', '.', ' ', '.'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'.', ' ', '.', ' ', '.', ' ', '.', ' ', '.', ' ', '.'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'.', ' ', '.', ' ', '.', ' ', '.', ' ', '.', ' ', '.'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'.', ' ', '.', ' ', '.', ' ', '.', ' ', '.', ' ', '.'}
        }
    };

    int mode;
    while (1) { // Loop until valid input
        printf("Choose game mode or \"Exit\":\n");
        printf("(1) Two Players (Human vs. Human)\n");
        printf("(2) Human vs. Bot (Easy Level)\n");
        printf("(3) Human vs. Bot (Medium Level)\n");
    
        char modeInput[256];
        if (fgets(modeInput, sizeof(modeInput), stdin) == NULL) {
            // EOF or error; exit or handle however you like
            printf("Error reading input. Exiting.\n");
            return 1;
        }
        // parsing
        if (sscanf(modeInput, "%d", &mode) == 1 && (mode == 1 || mode == 2 || mode == 3)) {
            // Valid integer and it's 1, 2, or 3
            // break out of the loop
            break;
        } else {
            printf("Invalid input. Please enter 1, 2, or 3.\n");
        }
    }   // ADD EXITT HASHEM
    
    srand(time(NULL)); // Seed the random generator for bot
    while (game.remaining_boxes > 0) { // game loop
        print_board(game.board);

        if (game.current_player == 1) { //Player A always human
            printf("Player A's turn. Enter the row and column of the first dot (e.g., A0 -> 0 0) and second dot or \"exit\":\n"); 

            char input[256];
            if (fgets(input, sizeof(input), stdin) == NULL) break;

            normalize_input(input); // Handles trimming and lowercase

            if (strcmp(input, "exit") == 0) break;
            int r1, c1, r2, c2;

            if (sscanf(input, "%d %d %d %d", &r1, &c1, &r2, &c2) != 4) { // Check if the input is valid
                printf("Invalid input. Please enter 4 integers.\n");
            } else {
                int result = process_move(&game, r1, c1, r2, c2); // Check if the move is valid and update the bool arrays
                int linetype = line_type(r1, c1, r2, c2);
                switch (result) {
                    case 0:
                        if (linetype == HORIZONTAL){ // Horizontal line
                            handle_horizontal_line(&game, r1, c1, r2);
                        }
                        if (linetype == VERTICAL){ // Vertical line
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
        } 
        else {
            if(mode == 1) {
                printf("Player B's turn. Enter the row and column of the first dot (e.g., A0 -> 0 0) and second dot or \"exit\":\n");
                
                char input[256];
                if (fgets(input, sizeof(input), stdin) == NULL) break;
                normalize_input(input);
                
                if (strcmp(input, "exit") == 0) break;
                
                int r1, c1, r2, c2;
                if (sscanf(input, "%d %d %d %d", &r1, &c1, &r2, &c2) != 4) {
                    printf("Invalid input. Please enter 4 integers.\n");
                } 
                else {
                    int result = process_move(&game, r1, c1, r2, c2); // Check if the move is valid and update the bool arrays
                    int linetype = line_type(r1, c1, r2, c2);
                    switch (result) {
                        case 0:
                            if (linetype == HORIZONTAL){ // Horizontal line
                                handle_horizontal_line(&game, r1, c1, r2);
                            }
                            if (linetype == VERTICAL){  // Vertical line
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
            } 
            else if (mode == 2) { // Easy bot
                printf("Player B (Bot) is thinking...\n");
                easy_bot_move(&game);
            }
            else if (mode == 3) { // Medium bot
                printf("Player B (Bot) is thinking...\n");
                medium_bot_move(&game); 
            }
        }
        printf("********************************************************************************\n");
        printf("                            Player A score: %d\n", game.scores[0]);
        printf("                            Player B score: %d\n", game.scores[1]);
        printf("********************************************************************************\n");
    }

    int winner = (game.scores[0] > game.scores[1]) ? 1 : 2;
    print_board(game.board);
    printf("Game over! %s", (game.scores[0] == game.scores[1]) ? "Tie!\n" : ((winner == 1) ? "Player A wins!\n" : "Player B wins!\n"));

    return 0;
}