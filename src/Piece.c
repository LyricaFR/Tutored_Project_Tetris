/*
Author: QUACH Kevin
Created on the : 05-02-2022
Last modification : 08-06-2022
*/

#include "../include/Piece.h"


/* +1 for the null_piece */
const int lst_struct[NB_PIECE + 1][PIECE_SIZE][PIECE_SIZE] = {
    {
    {0,0,0,0},
    {0,1,0,0},
    {1,1,1,0},
    {0,0,0,0}
    },

    {
    {0,0,0,0},
    {1,1,1,1},
    {0,0,0,0},
    {0,0,0,0}
    },

    {
    {0,0,0,0},
    {0,0,1,0},
    {1,1,1,0},
    {0,0,0,0}
    },

    {
    {0,0,0,0},
    {1,0,0,0},
    {1,1,1,0},
    {0,0,0,0}
    },

    {
    {0,0,0,0},
    {0,1,1,0},
    {1,1,0,0},
    {0,0,0,0}
    },

    {
    {0,0,0,0},
    {1,1,0,0},
    {0,1,1,0},
    {0,0,0,0}
    },

    {
    {0,0,0,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0}
    },

    {
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0}    
    }
};

void copy_struct(Piece* piece, Type type){
    int i, j;
    for (i = 0; i < PIECE_SIZE; i++){
        for (j = 0; j < PIECE_SIZE; j++){
            piece->block[i][j].filled = lst_struct[type][i][j];
            piece->block[i][j].color = type;
        }
    }
    piece->type = type;
}

Piece generate_piece(Type type){
    Piece piece;
    copy_struct(&piece, type);
    return piece;
}

Piece generate_random_piece(){
    Type type = rand() % NB_PIECE;
    return generate_piece(type);
}

