#include <stdbool.h>

typedef struct {
    // Grid state
    bool horizontal_lines[4][5];  // 4 rows, 5 cols of horizontal lines
    bool vertical_lines[5][4];    // 5 rows, 4 cols of vertical lines
    int box_owner[4][5];          // 0=neutral, 1/2=players
    
    // Game state
    int current_player;
    int scores[2];
    int remaining_boxes;
} GameState;

int main() {
    //main function
    printf("test");
    return 0;
}
