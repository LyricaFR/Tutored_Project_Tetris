/*
Author: QUACH Kevin
Created on the : 12-02-2022
Last modification : 25-02-2022
*/

#include "../include/Movement.h"


/* This function returns 1 if the current piece can move one block to the right,
   else returns 0 */
static int can_move_right(Game* game){
    int pos_y, pos_x;
    for (pos_y = 0; pos_y < PIECE_SIZE; pos_y++){
        for (pos_x = 0; pos_x < PIECE_SIZE; pos_x++){

            /* For every block of the current piece 
               we check if the block to its right on
               the board is filled
            */

            if (game->current_piece.block[pos_y][pos_x].filled == 1){
                if ((game->x + pos_x >= SIZE_X - 1) || 
                    (game->board[game->y + pos_y][game->x + pos_x + 1].filled == 1))
                    return 0;
            }
        }
    }
    return 1;
}


/* This function returns 1 if the current piece can move one block to the left,
   else returns 0 */
static int can_move_left(Game* game){
    int pos_y, pos_x;
    for (pos_y = 0; pos_y < PIECE_SIZE; pos_y++){
        for (pos_x = 0; pos_x < PIECE_SIZE; pos_x++){

            /* For every block of the current piece 
               we check if the block to its left on
               the board is filled
            */

            if (game->current_piece.block[pos_y][pos_x].filled == 1){
                if ((game->x + pos_x <= 0) || 
                    (game->board[game->y + pos_y][game->x + pos_x - 1].filled == 1))
                    return 0;
            }
        }
    }
    return 1;
}


/* This function returns 1 if the current_piece can go down one block,
   else returns 0 */
static int can_move_down(Game* game){
    int pos_x, pos_y;
    for (pos_y = 0; pos_y < PIECE_SIZE; pos_y++){
        for (pos_x = 0; pos_x < PIECE_SIZE; pos_x++){
            if (game->current_piece.block[pos_y][pos_x].filled == 1){

                /* If the piece is already at the bottom */
                if (game->y + pos_y >= SIZE_Y - 1)
                    return 0;

                /* Check if the blocks below the current_piece blocks are empty */
                else if (game->board[game->y + pos_y + 1][game->x + pos_x].filled == 1){
                    return 0;
                }
            }
        }
    }
    return 1;
}


void move_piece_left(Game* game){
    if (can_move_left(game))
        game->x -= 1;
}


void move_piece_right(Game* game){
    if (can_move_right(game))
        game->x += 1;
}


void move_piece_down(Game* game){
    if (can_move_down(game))
        game->y += 1;
}


void hold_piece(Game* game){
    Type type = game->current_piece.type;
    if (game->hold.type == null_piece){
        game->hold = generate_piece(type);
        draw_next_piece(game);
    }
    else{

        /* replace the current piece by the hold piece */
        game->current_piece = game->hold;

        /* Copy the structure of the current piece to the hold piece.
           This is so that the held piece doesn't doesn't take into 
           account the rotation angle of the current piece
        */
        copy_struct(&(game->hold), type);
        game->x = INITIAL_X;
        game->y = INITIAL_Y;
    }
}


void place_piece(Game* game){
    int pos_x, pos_y;
    for (pos_y = 0; pos_y < PIECE_SIZE; pos_y++){
        for (pos_x = 0; pos_x < PIECE_SIZE; pos_x++){
            /* Copy the current_piece to the board at its current location */
            if (game->current_piece.block[pos_y][pos_x].filled == 1){
                game->board[game->y + pos_y][game->x + pos_x].filled = 1;
                game->board[game->y + pos_y][game->x + pos_x].color = game->current_piece.block[pos_y][pos_x].color;
                /* If we place a piece out of the board, we lose */
                /* The board is size 22 but 2 lines are used to see the next piece come
                   and don't count. So the top line is 2 */
                if (game->y + pos_y < 2) 
                    game->lose = 1;
            }
        }
    }
    /* Draw the next piece and reset the position */
    draw_next_piece(game);
    game->can_hold = 1; 
}


void drop_piece(Game* game){
    while (can_move_down(game)){
        game->y += 1;
    }
    place_piece(game);
}


int periodic_piece_fall(Game* game, int interval, Uint32* time_flag, Uint32 current_time){
    if (current_time - *time_flag > interval){
        move_piece_down(game);
        *time_flag = current_time;
        return 1;
    }
    return 0;
}


int check_if_at_bottom(Game* game, Uint32* time_flag){
    int y, x;
    if (*time_flag == -1){
        for (y = 0; y < PIECE_SIZE; y++){
            for (x = 0; x < PIECE_SIZE; x++){
                /* If we're on the block of a piece, check beneath it */
                if (game->current_piece.block[y][x].filled){
                    /* If there's a block beneath it, the piece is at the bottom */
                    if (game->board[game->y + y + 1][game->x + x].filled){
                        /* Stock in time_flag the time at which the piece first hit the bottom */
                        *time_flag = SDL_GetTicks();
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

int autodrop(Game* game, int time_limit, Uint32* time_flag, Uint32 current_time){
    /* If there's no time flag, meaning if the piece didn't reach the bottom, do nothing */
    if (*time_flag == -1)
        return 0;
    /* Else compare the time at which the piece first got to the botton with the current time to see if it's past the time limit,
       if yes, place the piece down */
    if (current_time - *time_flag > time_limit){
        drop_piece(game);
        *time_flag = -1;
        return 1;
    }
    return 0;
}