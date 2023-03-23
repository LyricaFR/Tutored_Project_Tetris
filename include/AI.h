/*
Author: QUACH Kevin
Created on the : 27-02-2022
Last modification : 08-06-2022
*/

#ifndef __AI__
#define __AI__

#include "Input.h"
#include <math.h>


/* This stuct will store the number of moves the AI will have to play */
typedef struct aimove {
	int left;
	int right;
	int rotate_left;
	int rotate_right;
	int hold;
} AIMoves;

/* This struct will store a configuration, it's score and the moves needed to reach said configuration */
typedef struct {
	float score;
	AIMoves moves;
	Game config;
} ConfigData;


/* This function will evaluate the board given to it and returns a score */
float evaluate_board(Game* game);

float evaluate_board2(Game* game);


/* This function will apply evaluate_board on all possible configurations with the game's
   current piece and return AIMoves struct containing the moves the AI will have to play
   to reach the board with the best score out of all the ones tested */
AIMoves search_board(Game* game, int move_interval, int fall_interval);


/* This function will make the AI make a move at each time interval given in the parameters.
   This function returns 1 if the AI placed a piece, 0 if it moved a piece, -1 if there is time left
   before the next move */
int AI_play(Game* game, AIMoves* moves, int interval, Uint32* time_flag, Uint32 current_time);

void print_nb_hole(Game* game);

#endif