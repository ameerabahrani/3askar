#include <assert.h>
#include <stdbool.h>
 

static void test_line_type(void) {
    assert(get_line_type(0,0, 0,1) == HORIZONTAL);
    assert(get_line_type(0,0, 1,0) == VERTICAL);
    assert(get_line_type(0,0, 1,1) == -1);
}

static void test_adjacent(void) {
    assert(adjacent(0,0, 0,1) == true); //valid horizontal adjacency
    assert(adjacent(0,0, 1,0) == true); // valid vertical adjacency
    assert(adjacent(0,0, 1,1) == false); // diagonal non-adjacent
    assert(adjacent(ROWS - 1, COLS - 1, ROWS - 2, COLS - 1) == true); // valid vertical adjacency at the edge of the board
    assert(adjacent(0, 0, 0, 0) == false); // point cannot be adjacent to itself
    assert(adjacent(0, 0, 0, 2) == false); // not adjacent
    assert(adjacent(0, 0, 2, 0) == false); // not adjacent
}


static void test_check_box(void) {
    GameState s = {0};
    // no lines => no box
    assert(!check_box(&s, 0,0));

    // set the four lines around box (0,0):
    s.horizontal_lines[0][0] = true;
    s.horizontal_lines[1][0] = true;
    s.vertical_lines  [0][0] = true;
    s.vertical_lines  [0][1] = true;
    assert( check_box(&s, 0,0) );
    // out of bounds
    assert( !check_box(&s, -1,0) );
}

static void test_process_move(void) {
    GameState s = {0};
    s.current_player = 1;
    s.remaining_moves = 1;

    // draw a horizontal line
    assert(process_move(&s, 0,0, 0,1) == 0);
    assert(s.horizontal_lines[0][0]);
    assert(s.remaining_moves == 0);
    // invalid coords
    assert(process_move(&s, -1,0, -1,1) == -4);
}

static void test_claim_box(void) {
    GameState s = {0};
    s.current_player  = 2;
    s.remaining_boxes = 1;

    bool done = false;
    claim_box(&s, 0,0, &done);
    assert(done);
    assert(s.box_owner[0][0] == 2);
    assert(s.scores[1]      == 1);
    assert(s.remaining_boxes== 0);
}

static void test_handle_horizontal_line(void) {
    GameState s = {0};
    s.current_player = 1;
    // prepare three sides of box (0,0):
    s.horizontal_lines[1][0] = true;
    s.vertical_lines  [0][0] = true;
    s.vertical_lines  [0][1] = true;
    // now draw the top side and handle it
    process_move(&s, 0,0, 0,1);
    handle_horizontal_line(&s, 0,0, 1);
    assert(s.box_owner[0][0] == 1);
    // since a box was claimed, player does NOT switch
    assert(s.current_player == 1);
}

static void test_handle_vertical_line(void) {
    GameState s = {0};
    s.current_player = 2;
    // prepare three sides of box (0,0):
    s.horizontal_lines[0][0] = true;
    s.horizontal_lines[1][0] = true;
    s.vertical_lines  [0][1] = true;
    // now draw the left side and handle it
    process_move(&s, 0,0, 1,0);
    handle_vertical_line(&s, 0,0, 1);
    assert(s.box_owner[0][0] == 2);
    assert(s.current_player == 2);
}

static void test_print_box(void) {
    GameState s = {0};
    s.current_player = 1;
    print_box(&s, 0,0);
    assert(s.board[1][1].symbol == 'A');
    assert(s.board[1][1].color  == 1);
}

void test_normalize_input() {
    char s1[] = "  HELLO ";  // leading/trailing spaces
    normalize_input(s1);
    assert(strcmp(s1, "hello") == 0);

    char s2[] = "NO\n";      // trailing newline
    normalize_input(s2);
    assert(strcmp(s2, "no") == 0);

    char s3[] = "";         
    normalize_input(s3);
    assert(strcmp(s3, "") == 0);

    char s4[] = "  A  B  ";  //internal spaces are preserved 
    normalize_input(s4);
    assert(strcmp(s4, "a  b") == 0);
}

int main(void) {
    test_line_type();
    test_adjacent();
    test_check_box();
    test_process_move();
    test_claim_box();
    test_handle_horizontal_line();
    test_handle_vertical_line();
    test_print_box();
    test_normalize_input();
    printf("all tests passed\n");
    return 0;
}
