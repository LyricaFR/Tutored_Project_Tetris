/*
Author: QUACH Kevin
Created on the : 12-02-2022
Last modification : 25-02-2022
*/

#ifndef __MOVEMENT__
#define __MOVEMENT__

#include "Rotation.h"


/* This function will move game's current_piece one block to the left */
void move_piece_left(Game* game);

/* This function will move game's current_piece one block to the right */
void move_piece_right(Game* game);

/* This function will move game's current_piece one block down */
void move_piece_down(Game* game);

/* If the hold piece is NULL, put the current piece as the 
   hold piece and draw the next piece in the queue as current piece.
   If the hold piece isn't NULL, replace the current piece by the 
   one on hold and the one on hold with the current piece */
void hold_piece(Game* game);

/* This function will place the current_piece on the board at its current position
   and draw the next piece from the queue.
   If a piece is placed out of the board, change the lose field of the game to 1. */
void place_piece(Game* game);

/* This function will drop the current_piece down to the bottom of the board */
void drop_piece(Game* game);

/* Periodically make the current piece fall down one black at an interval (in milliseconds)
   given as argument, the time will be calculated by  current_time - time_flag  */
int periodic_piece_fall(Game* game, int interval, Uint32* time_flag, Uint32 current_time);

/* Check if the current piece is at the bottom, if it is, stock the current time in time_flag if
   there's no time stored already (represented by -1).
   Returns 1 if a new time_flag was set else return 0 */
int check_if_at_bottom(Game* game, Uint32* time_flag);

/* Autodrop the current piece once the time_limit is reached, if there's no time_flag (= -1) do nothing */
int autodrop(Game* game, int time_limit, Uint32* time_flag, Uint32 current_time);

#endif