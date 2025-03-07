// print method implemented here
#include <stdio.h>
void PrintBoard(char board[9][11])
{
    printf("  0 1 2 3 4 5\n");

    for (int i = 0; i < 9; i++)
    {
        if(i % 2 == 0)
        printf("%d ", i / 2);
        for (int j = 0; j < 11; j++)
        {
            
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
}

/*int main() // just to test the code remove the comment slashes and run
{
    char board[9][11] = {
        {'.', ' ', '.', ' ', '.', ' ', '.', ' ', '.', ' ', '.'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'.', ' ', '.', ' ', '.', ' ', '.', ' ', '.', ' ', '.'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'.', ' ', '.', ' ', '.', ' ', '.', ' ', '.', ' ', '.'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'.', ' ', '.', ' ', '.', ' ', '.', ' ', '.', ' ', '.'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'.', ' ', '.', ' ', '.', ' ', '.', ' ', '.', ' ', '.'}
    };
    PrintBoard(board);
    return 0;
} 