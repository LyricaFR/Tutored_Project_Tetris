/*
Author: QUACH Kevin
Created on the : 25-02-2022
Last modification : 25-02-2022
*/

#ifndef __ROTATION__
#define __ROTATION__

#include "Game.h"


/* Rotate the current_piece of the game given as argument to the
   right if possible. Two function are needed because aside from
   line pieces, rotating other pieces will change its "center" position   
*/
void rotate_right(Game* game);


/* Rotate the current_piece of the game given as argument to the
   left if possible. Two function are needed because aside from
   line pieces, rotating other pieces will change its "center" position   
*/
void rotate_left(Game* game);


/* This function is called when a rotation is conflicting with the board.
   It will try to find a position in which the piece can be placed 
   moving the piece to the right, to the left or up by one block (it will also
   try to move 2 blocks if one isn't enough in the case of a line piece)  */
void rotation_adjustment(Game* game, Piece piece);


#endif
