#include "game.h"
#include <time.h>

void bot_move(GameState *state) {
   
    int r1, c1, r2, c2;
    int result = -1;

    while(1) {

        int isHorizontal = rand() % 2; // 0 or 1 to choose if the bot will generate a horizontal or vertical line

        if(isHorizontal == 1) {
            //Horizontal line
            r1 = rand() % (ROWS + 1);
            c1 = rand() % COLS;

            r2 = r1;
            c2 = c1 + 1;
        } else {
            //Vertical line
            r1 = rand() % ROWS;
            c1 = rand() % (COLS + 1);

            r2 = r1 + 1;
            c2 = c1;
        }

        /*// --- DEBUG PRINTS ---
        printf("[BOT DEBUG] Trying move: (%d, %d) -> (%d, %d), isHorizontal=%d\n",
               r1, c1, r2, c2, isHorizontal);

        // Process the move
        result = process_move(state, r1, c1, r2, c2);

        // --- DEBUG PRINTS ---
        printf("[BOT DEBUG] process_move returned: %d\n", result); */

        result = process_move(state, r1, c1, r2, c2);

        if(result == 0) {
            // int lineType = line_type(r1, c1, r2, c2); could have used this for consistency with logic but we already know from isHorizontal the line type
            bool boxCompleted = false;

            if( isHorizontal == 1) { //Horizontal Line
                int min_col = (c1 < c2) ? c1 : c2;

                //same check logic above box
                if(r1 > 0 && state->box_owner[r1 - 1][min_col] == 0 && check_box(state, r1 - 1, min_col)){
                    state->box_owner[r1 - 1][min_col] = state->current_player;
                    state->scores[state->current_player - 1]++;
                    state->remaining_boxes--;
                    boxCompleted = true;
                    player_box(state, r1 - 1, min_col);
                }

                //below box
                if (r1 < ROWS && state->box_owner[r1][min_col] == 0 && check_box(state, r1, min_col)){
                    state->box_owner[r1][min_col] = state->current_player;
                    state->scores[state->current_player - 1]++;
                    state->remaining_boxes--;
                    boxCompleted = true;
                    player_box(state, r1, min_col);
                }

                if(!boxCompleted) {
                    state->current_player = (state->current_player == 1 ? 2 : 1);
                }

            } else {//Vertical line
                int min_row = (r1 < r2) ? r1 : r2;

                //check left box
                if(c1 > 0 && state->box_owner[min_row][c1 - 1] == 0 && check_box(state, min_row, c1 - 1)) {
                    state->box_owner[min_row][c1 - 1] = state->current_player;
                    state->scores[state->current_player - 1]++;
                    state->remaining_boxes--;
                    boxCompleted = true;
                    player_box(state, min_row, c1 - 1);
                }

                //check right box
                if(c1 < COLS && state->box_owner[min_row][c1] == 0 && check_box(state, min_row, c1)) {
                    state->box_owner[min_row][c1] = state->current_player;
                    state->scores[state->current_player - 1]++;
                    state->remaining_boxes--;
                    boxCompleted = true;
                    player_box(state, min_row, c1);
                }

                if(!boxCompleted) {
                    state->current_player = (state->current_player == 1 ? 2 : 1);
                }
            }
            
            //valid move break out of loop
            printf("Bot played: %d %d %d %d\n", r1, c1, r2, c2);
            break;
        } else {
            //invalid move try again
            continue;
        }

    }
}

