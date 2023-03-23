/*
Author: QUACH Kevin
Created on the : 05-02-2022
Last modification : 05-02-2022
*/

#include "../include/Block.h"

Block initiate_block(int filled, Color color){
    Block block;
    block.filled = filled;
    block.color = color;
    return block;
}