#include "game.h"


int main() {                         
    GameState game = {
        .horizontal_lines = {
            {false, false, false, false, false},
            {false, false, false, false, false},
            {false, false, false, false, false},
            {false, false, false, false, false},
            {false, false, false, false, false}
        },
        .vertical_lines = {
            {false, false, false, false, false, false},
            {false, false, false, false, false, false},
            {false, false, false, false, false, false},
            {false, false, false, false, false, false},
        },
        .box_owner = {
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0}
        },
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
        PrintBoard(game.board);

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
                if (check_box(&game, r1, c1)) {
                    game.box_owner[r1][c1] = game.current_player;
                    game.scores[game.current_player -1]++;
                    game.remaining_boxes--;
                }
                else{
                    game.current_player = (game.current_player == 1) ? 2 : 1;
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
        printf("                    Player A score: %d\n", game.scores[0]);
        printf("                    Player B score: %d\n", game.scores[1]);
        printf("********************************************************************************\n");
    }

    return 0;
}