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

    char modeInput[256];
    int mode;
    printf("Choose game mode:\n");
    printf("1) Two Players (Human vs. Human)\n");
    printf("2) Human vs. Bot (Easy Level)\n");
    
    fgets(modeInput, sizeof(modeInput), stdin);
    int mode;
    if (sscanf(modeInput, "%d", &mode) != 1) {
        //handle invalid input  
    }
    
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
                        if (linetype == 0){ // Horizontal line
                            bool boxCompleted = false;
                            int min_col = (c1 < c2) ? c1 : c2; 
    
                            // Check if the box above is checked to account for two boxes being completed at once
                            if (r1 > 0 && game.box_owner[r1 - 1][min_col] == 0 && check_box(&game, r1 - 1, min_col)){ 
                                game.box_owner[r1 - 1][min_col] = game.current_player;
                                game.scores[game.current_player - 1]++;
                                game.remaining_boxes--;
                                boxCompleted = true;
                                player_box(&game, r1 - 1, min_col);
                            }
    
                            // Check if the box down is checked to account for two boxes being completed at once
                            if (r1 < ROWS && game.box_owner[r1][min_col] == 0 && check_box(&game, r1, min_col)){ 
                                game.box_owner[r1][min_col] = game.current_player;
                                game.scores[game.current_player - 1]++;
                                game.remaining_boxes--;
                                boxCompleted = true;
                                player_box(&game, r1, min_col);
                            }
    
                            if (!boxCompleted){ 
                                game.current_player = (game.current_player == 1) ? 2 : 1;
                            }
                        }
                        if (linetype == 1){ 
                            bool boxCompleted = false;
                            int min_row = (r1 < r2) ? r1 : r2;
    
                            // Check if the box to the left is checked to account for two boxes being completed at once
                            if (c1 > 0 && game.box_owner[min_row][c1 - 1] == 0 && check_box(&game, min_row, c1 - 1)){ 
                                game.box_owner[min_row][c1 - 1] = game.current_player;
                                game.scores[game.current_player - 1]++;
                                game.remaining_boxes--;
                                boxCompleted = true;
                                player_box(&game, min_row, c1 - 1);
                            }
    
                            // Check if the box to the right is checked to account for two boxes being completed at once
                            if (c1 < COLS && game.box_owner[min_row][c1] == 0 && check_box(&game, min_row, c1)){ 
                                game.box_owner[min_row][c1] = game.current_player;
                                game.scores[game.current_player - 1]++;
                                game.remaining_boxes--;
                                boxCompleted = true;
                                player_box(&game, min_row, c1);
                            }
    
                            if (!boxCompleted){
                                game.current_player = (game.current_player == 1) ? 2 : 1;
                            }
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

        
        } else {
            if(mode == 1) {
                printf("Player B's turn. Enter the row and column of the first dot (e.g., A0 -> 0 0) and second dot or \"exit\":\n");
                
                char input[256];
                if (fgets(input, sizeof(input), stdin) == NULL) break;
                normalize_input(input);
                
                if (strcmp(input, "exit") == 0) break;
                
                int r1, c1, r2, c2;
                if (sscanf(input, "%d %d %d %d", &r1, &c1, &r2, &c2) != 4) {
                    printf("Invalid input. Please enter 4 integers.\n");
                } else {
                    int result = process_move(&game, r1, c1, r2, c2); // Check if the move is valid and update the bool arrays
                    int linetype = line_type(r1, c1, r2, c2);
                    switch (result) {
                        case 0:
                            if (linetype == 0){ // Horizontal line
                                bool boxCompleted = false;
                                int min_col = (c1 < c2) ? c1 : c2; 
        
                                // Check if the box above is checked to account for two boxes being completed at once
                                if (r1 > 0 && game.box_owner[r1 - 1][min_col] == 0 && check_box(&game, r1 - 1, min_col)){ 
                                    game.box_owner[r1 - 1][min_col] = game.current_player;
                                    game.scores[game.current_player - 1]++;
                                    game.remaining_boxes--;
                                    boxCompleted = true;
                                    player_box(&game, r1 - 1, min_col);
                                }
        
                                // Check if the box down is checked to account for two boxes being completed at once
                                if (r1 < ROWS && game.box_owner[r1][min_col] == 0 && check_box(&game, r1, min_col)){ 
                                    game.box_owner[r1][min_col] = game.current_player;
                                    game.scores[game.current_player - 1]++;
                                    game.remaining_boxes--;
                                    boxCompleted = true;
                                    player_box(&game, r1, min_col);
                                }
        
                                if (!boxCompleted){ 
                                    game.current_player = (game.current_player == 1) ? 2 : 1;
                                }
                            }
                            if (linetype == 1){ 
                                bool boxCompleted = false;
                                int min_row = (r1 < r2) ? r1 : r2;
        
                                // Check if the box to the left is checked to account for two boxes being completed at once
                                if (c1 > 0 && game.box_owner[min_row][c1 - 1] == 0 && check_box(&game, min_row, c1 - 1)){ 
                                    game.box_owner[min_row][c1 - 1] = game.current_player;
                                    game.scores[game.current_player - 1]++;
                                    game.remaining_boxes--;
                                    boxCompleted = true;
                                    player_box(&game, min_row, c1 - 1);
                                }
        
                                // Check if the box to the right is checked to account for two boxes being completed at once
                                if (c1 < COLS && game.box_owner[min_row][c1] == 0 && check_box(&game, min_row, c1)){ 
                                    game.box_owner[min_row][c1] = game.current_player;
                                    game.scores[game.current_player - 1]++;
                                    game.remaining_boxes--;
                                    boxCompleted = true;
                                    player_box(&game, min_row, c1);
                                }
        
                                if (!boxCompleted){
                                    game.current_player = (game.current_player == 1) ? 2 : 1;
                                }
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

            } else {
                printf("Player B (Bot) is thinking...\n");
                bot_move(&game);
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