/*
Author: QUACH Kevin
Created on the : 12-02-2022
Last modification : 13-03-2022
*/

#include "../include/Versus.h"

int nb_of_garbage_reduced(Game* current_game, int nb_cleared){
    int cmpt = 0;
    switch (nb_cleared){
        case 1:
            if (current_game->ren >= 1)
                cmpt += 1;
            if (current_game->ren >= 4)
                cmpt += 1;
            if (current_game->ren >= 7)
                cmpt += 1;
            return cmpt;
        case 2:
            if (current_game->ren >= 3)
                cmpt += 1;
            if (current_game->ren >= 6)
                cmpt += 1;
            return 1 + cmpt;
        case 3:
            if (current_game->ren >= 2)
                cmpt += 1;
            if (current_game->ren >= 4)
                cmpt += 1;
            return 2 + cmpt;
        case 4:
            if (current_game->ren > 0)
                cmpt += 1;
            return 4 + cmpt;
        default:
            return 0;
    }
}


void manage_board_versus(Game* current_game, Game* target_game){
    /* Number of lines cleared */
    int nb_cleared = clear_lines(current_game);

    /* If there is at least one line cleared */
    if (nb_cleared > 0){
        /* Adding points */
        add_score(current_game, nb_cleared);

        /* Reduce garbage count depending on the number of lines cleared and stock the difference to send them as garbage*/
        int garbage_to_send = reduce_garbage(current_game, nb_of_garbage_reduced(current_game, nb_cleared));

        /* If there is garbage to send */
        if (garbage_to_send > 0)
            /* Send garbage to the opponent */
            target_game->garbage += garbage_to_send;
    }
    /* If no line wss cleared, add the garbage to the board */
    else{
        /* Combo broken */
        current_game->ren = -1;
        if (current_game->garbage > 0)
            add_garbage(current_game);
    }
}

int reduce_garbage(Game* current_game, int nb_reduced){
    int garbage_to_send = nb_reduced - current_game->garbage; /* number of lines to send */
    current_game->garbage -= nb_reduced;

    /* If the garbage count is negative */
    if (current_game->garbage < 0){
        /* Sets it at 0 */
        current_game->garbage = 0;
    }
    return garbage_to_send;
}


/* Check if the current piece is in conflict after adding garbage lines, if it is, shift it up 
   until there is no conflict */
static void correct_current_position(Game* game){
    /* If there is conflict with the current_piece */
    while (check_position_conflict(game, game->current_piece)){
        if (game->y - 1 >= 0)
        game->y -= 1;
    }
}


void add_garbage(Game* game){
    int x, y;
    int free_spot = rand() % SIZE_X; /* x coordinates of the free spot in the garbage line */
    for (y = 0; y < SIZE_Y; y++){
        for (x = 0; x < SIZE_X; x++){
            if (game->board[y][x].filled){
                /* Check if we will push blocks out of the board, in which case the player loses.
                   We do < 2 cause the board is 22 but 2 lines are for seeing the next piece drop */ 
                if (y - game->garbage < 2){     
                    game->lose = 1;
                    return;
                }
            }

            /* Checking this value to not go out of bound of the array */
            if (y - game->garbage >= 0){ 
                /* Pushing the blocks up to give space for the garbage lines */
                game->board[y - game->garbage][x] = game->board[y][x];
            }

            if (SIZE_Y - y <= game->garbage){
                if (x != free_spot)
                    game->board[y][x] = initiate_block(1, GARBAGE); 
                else
                    game->board[y][x].filled = 0;
            }
        }
    }
    correct_current_position(game);
    /* Resets garbage count */
    game->garbage = 0;
}
