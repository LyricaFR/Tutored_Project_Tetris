/*
Author: QUACH Kevin
Created on the : 05-02-2022
Last modification : 08-06-2022
*/

#ifndef __GAME__
#define __GAME__

#include "Queue.h"
#include <SDL2/SDL.h>

#define SIZE_X 10           /* Number of blocks horizontally */  
#define SIZE_Y 22           /* Number of blocks vertically (Counting the ones out of screen) */

#define GAME_SIZE_Y 20      /* Number of blocks used vertically without the ones out of screen */
#define GAME_SIZE_X SIZE_X

#define INITIAL_X (SIZE_X / 2 - 2)  /* Coordinate x at which pieces appear */
#define INITIAL_Y (-1)              /* Coordinate y at which pieces appear */
/* Mettre INITIAL_Y à -2 et augmenter la taille de l'interface plus tard */

typedef enum{
   SOLO,
   VERSUS,
   VERSUS_PLAYER,
   VERSUS_AI,
   EXIT,
   MENU
} Mode;

typedef struct {
    int number;                  /* The number of the game */
    Block board[SIZE_Y][SIZE_X]; /* The board of the game */
    Piece queue[QUEUE_SIZE];    /* The queue of pieces */
    Piece hold;                  /* The hold piece */
    Piece current_piece;         /* The piece actually being played */
    int x;                       /* The x coordinate of the current piece on the board */
    int y;                       /* The y coordinate of the current piece on the board */
    int can_hold;                /* 1 if the player didn't use the hold yet, else 0 */
    int score;                   /* Score of the player */
    int garbage;                 /* The number of garbage lines to be added */
    int ren;                     /* Combo counter */
    int lose;                    /* 1 If the game is lost, else 0 */
} Game;


/* Initiate a game of Tetris */
Game* init_game(int number);


/* Free the memory allocated for the game */
void free_game(Game* game);


/* Returns 1 if the block at coordinate (x,y) ont the board is
   either filled or the current piece occupies it, else returns 0 */
int is_filled(Game* game, int x, int y);


/* Replace the current piece with the next one in queue */
void draw_next_piece(Game* game);


/* Clear the lines that are completely filled and
   returns the number of lines cleared */
int clear_lines(Game* game);


/* This function will clear lines that can be cleared
   and increase the score by the corresponding amount */
int manage_board(Game* game);


/* Add points to the score according to the number of lines cleared and returns the amount of points added.
   Also manage the combo counter */
int add_score(Game* game, int lines_cleared);


/* This function check if the piece given in argument can be
   placed in the board at the current coordinate without 
   conflicting with the board's current structure or without
   being out of the board.
   Returns 1 if there is conflict, else returns 0 */
int check_position_conflict(Game* game, Piece piece);

#endif
