/*
Author: QUACH Kevin
Created on the : 25-02-2022
Last modification : 25-02-2022
*/

#include "../include/Rotation.h"

/* Rotation function for line pieces */
static void rotate_right1(Piece* piece){
    int x, y;
    Piece tmp = *piece;
    for (y = 0; y < PIECE_SIZE; y++){
        for (x = 0; x < PIECE_SIZE; x++){
            piece->block[y][x].filled = tmp.block[PIECE_SIZE - 1 - x][y].filled;
        }
    }
}


/* Rotation function for all other pieces */
static void rotate_right2(Piece* piece){
    int x, y;
    Piece tmp = *piece;
    for (y = 1; y < PIECE_SIZE; y++){
        for (x = 0; x < PIECE_SIZE - 1; x++){
            piece->block[y][x].filled = tmp.block[PIECE_SIZE - 1 - x][y - 1].filled;
        }
    }
}


/* Rotate the current_piece of the game given as argument to the
   right if possible. Two function are needed because aside from
   line pieces, rotating other pieces will change its "center" position   
*/
void rotate_right(Game* game){
    Piece tmp = game->current_piece;
    if (tmp.type == square)
        return;
    if (tmp.type == line)
        rotate_right1(&tmp);
    else
        rotate_right2(&tmp);
    /* If there is no conflict */
    if (!check_position_conflict(game, tmp)){
        game->current_piece = tmp;
        return;
    }
    rotation_adjustment(game, tmp);
}


/* Rotation function for line pieces */
static void rotate_left1(Piece* piece){
    int x, y;
    Piece tmp = *piece;
    for (y = 0; y < PIECE_SIZE; y++){
        for (x = 0; x < PIECE_SIZE; x++){
            piece->block[y][x].filled = tmp.block[x][PIECE_SIZE - 1 - y].filled;
        }
    }
}


/* Rotation function for all other pieces */
static void rotate_left2(Piece* piece){
    int x, y;
    Piece tmp = *piece;
    for (y = 1; y < PIECE_SIZE; y++){
        for (x = 0; x < PIECE_SIZE - 1; x++){
            piece->block[y][x].filled = tmp.block[x + 1][PIECE_SIZE - 1 - y].filled;
        }
    }
}


/* Rotate the current_piece of the game given as argument to the
   left if possible. Two function are needed because aside from
   line pieces, rotating other pieces will change its "center" position   
*/
void rotate_left(Game* game){
    Piece tmp = game->current_piece;
    if (tmp.type == square)
        return;
    if (tmp.type == line)
        rotate_left1(&tmp);
    else
        rotate_left2(&tmp);
    /* If there is no conflict */
    if (!check_position_conflict(game, tmp)){
        game->current_piece = tmp;
        return;
    }
    rotation_adjustment(game, tmp);
}


/* This function is called when a rotation is conflicting with the board.
   It will try to find a position in which the piece can be placed 
   moving the piece to the right, to the left or up by one block (it will also
   try to move 2 blocks if one isn't enough in the case of a line piece).*/
void rotation_adjustment(Game* game, Piece piece){

    /* The original coordinate of the pieces */
    int original_x = game->x;
    int original_y = game->y;  

    int i, nb_pos_x = 4, nb_pos_y = 4;

    /* List of horizontal position that will be attempted */
    int lst_position_x[4] = {original_x + 1, original_x - 1,
                             original_x + 2, original_x - 2};

    /* List of vertical position that will be attempted */
    int lst_position_y[4] = {original_y + 1, original_y - 1,
                             original_y + 2, original_y - 2};

    /* Attemps to move the piece around to find a spot where it won't be in conflict */
    for (i = 0; i < nb_pos_x; i++){
        game->x = lst_position_x[i];
        if (!check_position_conflict(game, piece)){
            game->current_piece = piece;
            return;
        }
        /* if the piece isn't a line, there's no need to try to move it 2 block horizontally*/
        if (i >= 2 && piece.type != line)
            break;
    }

    /* No spot found on the horizontal axis, restoring the original x coordinate */
    game->x = original_x;
    
    for (i = 0; i < nb_pos_y; i++){
        game->y = lst_position_y[i];
        if (!check_position_conflict(game, piece)){
            game->current_piece = piece;
            return;
        }
        /* if the piece isn't a line, there's no need to try to move it 2 block horizontally*/
        if (i >= 2 && piece.type != line)
            break;
    }

    /* No spot found on the vertical axis, restoring the original y coordinate */
    game->y = original_y;
}