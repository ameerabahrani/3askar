#include <stdbool.h>
#include <stdio.h>

//include all files used
#include "game.h"
#include "PrintMethod.h"
                                       // function called from another file PrintMethod.c
void PrintBoard(char board[9][11]);

typedef struct {
    bool horizontal_lines[4][5];      //horizontal lines
    bool vertical_lines[5][4];       //vertical lines
    int box_owner[4][5];            // 0=neutral  1/2=players
    int current_player;
    int scores[2];
    char board[9][11];            //9×11 board for printing dots and spaces
} GameState;

int main() {
                                
    GameState game = {
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
    // just a test to see if it runs (delete later)
    printf("Dots and Boxes Game Start\n");

    PrintBoard(game.board);

    return 0;
}

