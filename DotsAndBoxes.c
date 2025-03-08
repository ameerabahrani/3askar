#include "game.h"

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

    while (game.remaining_boxes > 0) {
        print_board(game.board);

        if (game.current_player == 1) {
            printf("Player A's turn. Enter the row and column of the first dot (e.g., A0 -> 0 0) and second dot:\n");  
        } else if (game.current_player == 2) {
            printf("Player B's turn. Enter the row and column of the first dot (e.g., A0 -> 0 0) and second dot:\n");  
        }

        int r1, c1, r2, c2;
        scanf("%d %d %d %d", &r1, &c1, &r2, &c2);

        int result = process_move(&game, r1, c1, r2, c2);
        switch (result) {
            case 0:
                if (line_type (r1, c1, r2, c2) == 0){ 
                    bool boxCompleted = false;
                    int min_col = (c1 < c2) ? c1 : c2;

                    if (r1 > 0 && game.box_owner[r1 - 1][min_col] == 0 && check_box(&game, r1 - 1, min_col)){
                        game.box_owner[r1 - 1][min_col] = game.current_player;
                        game.scores[game.current_player - 1]++;
                        game.remaining_boxes--;
                        boxCompleted = true;
                        player_box(&game, r1 - 1, min_col);
                    }

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
                if (line_type(r1, c1, r2, c2) == 1){ 
                    bool boxCompleted = false;
                    int min_row = (r1 < r2) ? r1 : r2;

                    if (c1 > 0 && game.box_owner[min_row][c1 - 1] == 0 && check_box(&game, min_row, c1 - 1)){
                        game.box_owner[min_row][c1 - 1] = game.current_player;
                        game.scores[game.current_player - 1]++;
                        game.remaining_boxes--;
                        boxCompleted = true;
                        player_box(&game, min_row, c1 - 1);
                    }

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

        printf("********************************************************************************\n");
        printf("                            Player A score: %d\n", game.scores[0]);
        printf("                            Player B score: %d\n", game.scores[1]);
        printf("********************************************************************************\n");
    }

    int winner = (game.scores[0] > game.scores[1]) ? 1 : 2;
    print_board(game.board);
    printf("Game over! Player %c wins!\n", (winner == 1) ? 'A' : 'B');

    return 0;
}