/*
Author: QUACH Kevin
Created on the : 05-02-2022
Last modification : 08-06-2022
*/

#include "../include/Game.h"

Game* init_game(int number){
    int i, j;
    Game* game = (Game*)malloc(sizeof(Game));
    if (game == NULL){
        fprintf(stderr, "Couldn't allocate memory for the game\n");
        return NULL;
    }
    game->number = number;
    for (i = 0; i < SIZE_Y; i++){
        for (j = 0; j < SIZE_X; j++){
            game->board[i][j] = initiate_block(0, COLORLESS);
        }
    }
    generate_queue(game->queue);
    game->hold = generate_piece(null_piece);
    game->current_piece = next_piece(game->queue);
    game->x = INITIAL_X; 
    game->y = INITIAL_Y;
    game->can_hold = 1;
    game->score = 0;
    game->garbage = 0;
    game->ren = -1;
    game->lose = 0;
    return game;
}


void free_game(Game* game){
    free(game);
}


int is_filled(Game* game, int x, int y){
    /* If the(x,y) block is filled */
    if (game->board[y][x].filled){
        return 1;
    }
    /* If the coordinate correspond to the current_piece
       returns if that block in the current piece is filled or not */
    if ((x >= game->x) && (x < game->x + 4) && (y >= game->y) && (y < game->y + 4)){
        return (game->current_piece.block[y - game->y][x - game->x].filled);
    }
    return 0;
}


void draw_next_piece(Game* game){
    /* Replace the current piece with the next one in queue */
    game->current_piece = next_piece(game->queue);
    /* Reset the position of the piece back to the initial position */
    game->x = INITIAL_X;
    game->y = INITIAL_Y;
}


/* Check the line y and return 1 if it is filled, else return 0 */
static int check_line(Game* game, int y){
    int x;
    for (x = 0; x < SIZE_X; x++){
        if (!(game->board[y][x].filled))
            return 0;
    }
    return 1;
}


/* Shift down all the lines of the board above line */
static void shift_lines_down(Game* game, int line){
    int x, y;
    for (y = line; y > 0; y--){
        for (x = 0; x < SIZE_X; x++){
            game->board[y][x] = game->board[y - 1][x];
        }
    }
}


int clear_lines(Game* game){
    int y;
    int nb_cleared = 0;
    for (y = 0; y < SIZE_Y; y++){
        if (check_line(game, y)){
            shift_lines_down(game, y);
            nb_cleared += 1;
        }
    }
    return nb_cleared;
}


int add_score(Game* game, int lines_cleared){
    int score = 0;
    switch (lines_cleared){
        case 1:
            game->ren += 1;
            score += 100; break;
        case 2:
            game->ren += 1;
            score += 300; break;
        case 3:
            game->ren += 1;
            score += 500; break;
        case 4:
            game->ren += 1;
            score += 800; break;
        default:
            /* No line cleared, combo broken */
            game->ren = -1;
            break;
    }
    if (game->ren > 0){
        game->score += game->ren * 50;
    }
    game->score += score;
    return score;
}


int manage_board(Game* game){
    int nb_cleared = clear_lines(game);
    return add_score(game, nb_cleared);
}


int check_position_conflict(Game* game, Piece piece){
    int x, y;
    for (y = 0; y < PIECE_SIZE; y++){
        for (x = 0; x < PIECE_SIZE; x++){
            if (piece.block[y][x].filled == 1){
                /* Check if the piece will be out of the board */
                if ((game->x + x < 0) || (game->x + x >= SIZE_X) ||
                    (game->y + y < 0) || (game->y + y >= SIZE_Y))
                    return 1;
                
                /* Check if the piece will conflict with a block of the board */
                else if ((game->board[game->y + y][game->x + x].filled == 1))
                    return 1;
            }
        }
    }
    return 0;
}
