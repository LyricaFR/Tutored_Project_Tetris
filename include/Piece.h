/*
Author: QUACH Kevin
Created on the : 05-02-2022
Last modification : 08-06-2022
*/

#ifndef __PIECE__
#define __PIECE__

#include <stdlib.h>
#include <time.h>
#include "Block.h"

#define NB_PIECE 7
#define PIECE_SIZE 4

typedef enum{
    T,
    line,
    L,
    reverseL,
    S,
    reverseS,
    square,
    null_piece
} Type;

typedef struct{
    Block block[PIECE_SIZE][PIECE_SIZE]; /* It's [Y][X] coordinate */
    Type type;
} Piece;

/* Copy the structure of a piece of type given as argument into the piece
   given */
void copy_struct(Piece* piece, Type type);

/* Generate a piece of the type given as parameter */
Piece generate_piece(Type type);

/* Generate a random piece */
Piece generate_random_piece();

#endif