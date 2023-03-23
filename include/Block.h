/*
Author: QUACH Kevin
Created on the : 05-02-2022
Last modification : 05-02-2022
*/
#ifndef __BLOCK__
#define __BLOCK__

#include <stdio.h>
#include "Color.h"


typedef struct {
    int filled;
    Color color;
} Block;

/* Initiate a block and returns it */
Block initiate_block(int filled, Color);

#endif