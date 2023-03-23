/*
Author: QUACH Kevin
Created on the : 12-02-2022
Last modification : 13-03-2022
*/

#ifndef __VERSUS__
#define __VERSUS__

#include "Game.h"

#define MAX(a,b) (a >= b ? a : b)

/* Returns the number of garbage lines to reduce by depending on the number of lines cleared */
int nb_of_garbage_reduced(Game* current_game, int nb_cleared);

/* Check if lines were cleared. 
   If yes, adds score, reduce garbage counter by the corresponding ammount and send the difference to the opponent.
   If no, adds the garbage the current_game has to its board and reset its garbage counter to 0 */
void manage_board_versus(Game* current_game, Game* target_game);

/* Reduce the garbage count of current_game by nb_reduced, and returns the difference */
int reduce_garbage(Game* current_game, int nb_reduced);

/* Add the garbage lines on the board and reset the garbage counter */
void add_garbage(Game* game);

#endif