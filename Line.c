#include "game.h"
void printLine(char board[ROWS * 2 + 1][COLS * 2 + 1], int x1, int y1, int x2, int y2){
    if(x1 == x2){
        board[2 * x1 + 1][2 * y1 + 1] = '_';
    }else if(y1 == y2){
        board[2 * x1 + 1][2 * y1 + 1] = '|';
    }   
}
