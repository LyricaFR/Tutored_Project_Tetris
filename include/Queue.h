/*
Author: QUACH Kevin
Created on the : 05-02-2022
Last modification : 08-06-2022
*/

#ifndef __QUEUE__
#define __QUEUE__

#include "Piece.h"
#include <string.h>
#define QUEUE_SIZE 5


/* Generate a list of 5 pieces and returns it */
void generate_queue(Piece queue[QUEUE_SIZE]);

/* Return the pieces at the front of the linked list,
 shift the pieces to the left, then adds a new one at the end */
Piece next_piece(Piece queue[QUEUE_SIZE]);




#endif