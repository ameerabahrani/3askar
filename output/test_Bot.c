#include <assert.h>
#include <stdlib.h>



static void test_medium_bot_move(void) {
    GameState state = {0};
    init_board(&state); // Initialize the board
    state.current_player = 2;
    state.scores[1] = 0;
    state.remaining_moves = 2;
    state.remaining_boxes = ROWS * COLS;

    // --- two‑box scenario at (0,0)&(0,1), missing vertical [0][1] ---
    state.horizontal_lines[0][0] = true;
    state.horizontal_lines[1][0] = true;
    state.vertical_lines[0][0] = true;

    
    state.horizontal_lines[0][1] = true;
    state.horizontal_lines[1][1] = true;
    state.vertical_lines[0][2] = true;

    // --- one‑box scenario at (2,1), missing horizontal [2][1]→[3][1] ---
    state.horizontal_lines[2][1] = true;
    state.horizontal_lines[3][1] = true;
    state.vertical_lines[2][1] = true;
    
    

    medium_bot_move(&state);

    // FIRST MOVE: bot should take the 2‑box line
    // check the move made by the bot
    assert(state.remaining_moves == 1);
    assert(state.box_owner[0][0] == 2);
    assert(state.box_owner[0][1] == 2);
    assert(state.scores[1]      == 2);
    assert(state.remaining_boxes == ROWS*COLS - 2);   
    
    medium_bot_move(&state);
    // SECOND MOVE: bot should take the 1‑box line
    assert(state.remaining_moves == 0);
    assert(state.box_owner[2][1] == 2);
    assert(state.scores[1]      == 3);

    printf("Medium bot move test passed!\n");
}

// Test function for the hard bot move
static void test_hard_bot_move(void) {
    GameState state = {0};
    init_board(&state); // Initialize the board
    state.current_player = 2;
    state.scores[1] = 0;
    state.remaining_moves = 2;
    state.remaining_boxes = ROWS * COLS;

    // --- two‑box scenario at (0,0)&(0,1), missing vertical [0][1] ---
    state.horizontal_lines[0][0] = true;
    state.horizontal_lines[1][0] = true;
    state.vertical_lines[0][0] = true;

    
    state.horizontal_lines[0][1] = true;
    state.horizontal_lines[1][1] = true;
    state.vertical_lines[0][2] = true;

    // --- one‑box scenario at (2,1), missing horizontal [2][1]→[3][1] ---
    state.horizontal_lines[2][1] = true;
    state.horizontal_lines[3][1] = true;
    state.vertical_lines[2][1] = true;
    
    

    hard_bot_move(&state);

    // FIRST MOVE: bot should take the 2‑box line
    // check the move made by the bot
    assert(state.remaining_moves == 1);
    assert(state.box_owner[0][0] == 2);
    assert(state.box_owner[0][1] == 2);
    assert(state.scores[1]      == 2);
    assert(state.remaining_boxes == ROWS*COLS - 2);   
    
    hard_bot_move(&state);
    // SECOND MOVE: bot should take the 1‑box line
    assert(state.remaining_moves == 0);
    assert(state.box_owner[2][1] == 2);
    assert(state.scores[1]      == 3);

    printf("Hard bot move test passed!\n");
}

int main() {
    test_medium_bot_move();
    test_hard_bot_move();
    printf("All tests passed!\n");
    return 0;
}

