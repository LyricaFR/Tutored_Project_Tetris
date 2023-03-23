/*
Author: QUACH Kevin
Created on the : 05-02-2022
Last modification : 08-06-2022
*/

#include "../include/Queue.h"


void generate_queue(Piece queue[QUEUE_SIZE]){
    int i;
    for (i = 0; i < QUEUE_SIZE; i++){
        queue[i] = generate_random_piece();
    }
}


Piece next_piece(Piece queue[QUEUE_SIZE]){
    int i;
    Piece piece = queue[0];
    for (i = 0; i < QUEUE_SIZE; i++){
        if (i == QUEUE_SIZE - 1)
            queue[i] = generate_random_piece();
        else
            queue[i] = queue[i + 1];
    }
    return piece;
}

