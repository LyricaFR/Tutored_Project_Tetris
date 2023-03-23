/*
Author: QUACH Kevin
Created on the : 27-02-2022
Last modification : 08-06-2022
*/

#include "../include/AI.h"

const int dir_coordinate [4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

#define NB_CONFIG 5 /* The number of configs we keep for the game tree search */


/* Check if there's a block above (x, y) */
static int block_above(Game* game, int y, int x){
    /* Check if there's a block above it */
    if (y > 0)
        return game->board[y - 1][x].filled;
    else
        return 1;
}


/* Check if there's a block below (x, y) */
static int block_below(Game* game, int y, int x){
    /* Check if there's a block below it */
    if (y < SIZE_Y - 1)
        return game->board[y + 1][x].filled;
    else
        return 1;
}


/* Check if there's a block at the left of (x, y) */
static int block_on_left(Game* game, int y, int x){
    /* Check if there's a block at its left */
    if (x > 0)
        return game->board[y][x - 1].filled;
    else
        return 1;
}


/* Check if there's a block at the right of (x, y) */
static int block_on_right(Game* game, int y, int x){
    if (x < SIZE_X - 1)
        return game->board[y][x + 1].filled;
    else
        return 1;

}


/* Returns the direction opposite to the one given to the function */
static int opposite_direction(int direction){
    switch (direction){
        case UP:
            return DOWN;
        case DOWN:
            return UP;
        case LEFT:
            return RIGHT;
        case RIGHT:
            return LEFT;
        default:
            return -1;
    }
}


/* Look for the closing of a hole with direction being the direction of the opening */
static int find_hole_corner(Game* game, int y, int x, int direction){
    int corners, i;
    int filled[4];
    filled[UP] = block_above(game, y, x);
    filled[DOWN] = block_below(game, y, x);
    filled[LEFT] = block_on_left(game, y, x);
    filled[RIGHT] = block_on_right(game, y, x);
    corners = filled[UP] + filled[DOWN] + filled[LEFT] + filled[RIGHT];

    /* If there's 3 borders, then the hole is closed */
    if (corners == 3)
        return 1;

    /* If there's 2 borders, then it's still unsure if it's a hole or not */
    else if (corners == 2){
        for (i = 0; i < 4; i++){
            if (filled[i] == 0 && i != opposite_direction(direction)){
                return find_hole_corner(game, y + dir_coordinate[i][0], x + dir_coordinate[i][1], i);
            }
        }
    }

    /* If there is 1 border, the chance of there being a hole is very slim,
       for now, I'm assuming 1 border means no hole. Of course 0 borders means
       no hole too */ 
    return 0;
}


/* Check if position (x, y) in the game is surrounded by blocks */
static int check_hole(Game* game, int y, int x){
    int corners, i;
    int filled[4];
    filled[UP] = block_above(game, y, x);
    filled[DOWN] = block_below(game, y, x);
    filled[LEFT] = block_on_left(game, y, x);
    filled[RIGHT] = block_on_right(game, y, x);
    corners = filled[UP] + filled[DOWN] + filled[LEFT] + filled[RIGHT];

    /* There's blocks on all 4 corners, there's a hole */
    if (corners == 4){
        return 1;
    }

    /* The position is surrounded by 3 blocks, we need to find out if the hole closes somewhere */
    else if (corners == 3){
        for (i = 0; i < 4; i++){
            if (filled[i] == 0){
                return find_hole_corner(game, y + dir_coordinate[i][0], x + dir_coordinate[i][1], i);
            }
        }
    }

    /* The position is surrounded by 2 blocks, we search for a hole edge on both sides */
    /*
    else if (corners == 2){
        int hole = 1;
        for (i = 0; i < 4; i++){
            if (filled[i] == 0){
                hole = hole && find_hole_corner(game, y + dir_coordinate[i][0], x + dir_coordinate[i][1], i);
            }
        }
        return hole;
    }
    */

    /* Only 2 or less borders */
    return 0;
}

static int positive(int nb){
    return (nb < 0 ? -nb : nb);
}

/* Evaluate the landscape of the game. The smoother and lower it is, the higher is the score */
static float evaluate_landscape(Game* game){
    float score = 0;
    int x, y, i, average_height = 0, max_height = SIZE_Y, aggregate_height = 0;
    int first_block_y[SIZE_X];

    /* Initiating the list */
    for (x = 0; x < SIZE_X; x++){
        first_block_y[x] = SIZE_Y - 1;
    }

    for (x = 0; x < SIZE_X; x++){
        for (y = 0; y < SIZE_Y; y++){

            if (game->board[y][x].filled == 1){

                /* Check if there is an empty space under a block */
                if (y < SIZE_Y - 1){
                    if (game->board[y + 1][x].filled == 0)
                        score -= 300; 
                    else
                        score += 200;
                }
                /* Check if there is an empty space at the right of a block */
                if (x < SIZE_X - 1){
                    if (game->board[y][x + 1].filled == 0)
                        score -= 300;
                    else
                        score += 200;
                }
                /* Check if there is an empty space at the left of a block */ 
                if (x > 0){
                    if (game->board[y][x - 1].filled == 0)
                        score -= 300;
                    else
                        score += 200;
                }

                if (y < first_block_y[x]){
                    first_block_y[x] = y; /* Stock the vertical position of every first block on each column */
                    aggregate_height += y;     /* Accumulator of the height of every column */
                }

            }

            if (game->board[y][x].filled == 0){

            }

        }
    }

    for (i = 0; i < SIZE_X - 1; i++){
        /* Give points if the offset between each column is below 3, else reduce it */
        score += ((1 - positive(first_block_y[i] - first_block_y[i + 1])) * 150);

        if (first_block_y[i] < max_height)
            max_height = first_block_y[i];

    }
    /* Reduce score by 800 the placement leads to loosing the game */
    if (max_height < 2){
        score -= 1000;
    }

    average_height = aggregate_height / SIZE_X;
    score += average_height * 150;
    score += max_height * 150; /* Increase the score, the lower the height is (reminder that the height is reversed) */
    return score;
}

static float evaluate_landscape2(Game* game){
    float score = 0;
    /* Aggregate height is the sum of the height of all the column */
    int x, y, aggregate_height = 0, bumpiness = 0, holes = 0;
    int first_block_y[SIZE_X];

    /* Initiating the list */
    for (x = 0; x < SIZE_X; x++){
        first_block_y[x] = SIZE_Y;
    }

    for (x = 0; x < SIZE_X; x++){
        for (y = 0; y < SIZE_Y; y++){
            if (game->board[y][x].filled == 1){
                if (first_block_y[x] > y){
                    first_block_y[x] = y;
                    /* Big score decrease if it's a losing move */
                    if (first_block_y[x] < 2){
                        score += -10.563221;
                    }
                    aggregate_height += (SIZE_Y - 1) - y + 1;
                }
            }

            else{
                if (y > first_block_y[x]){
                    holes++;
                }
            }
        }
    }

    for (x = 0; x < SIZE_X - 1; x++){
        bumpiness += positive(first_block_y[x] - first_block_y[x + 1]);
    }
    score += ((-0.510066) * aggregate_height) + ((-0.55663) * holes) + ((-0.184483) * bumpiness);

    /* For debugging */
    /*
    draw_board_ascii(game);
    printf("aggregate_height = %d\nholes = %d\nbumpiness = %d\nscore = %f\n", aggregate_height, holes, bumpiness, score);
    */
    
    return score;
}

float evaluate_board(Game* game){
    int x, y, score = 0;
    score += manage_board(game) * 10; /* Points from lines cleared */
    score += evaluate_landscape(game); /* Points from the landscape */
    /* Looking for holes in the configuration */
    for (y = 1; y < SIZE_Y; y++){
        for (x = 0; x < SIZE_X; x++){
            if (!(game->board[y][x].filled)){
                if (check_hole(game, y, x)){
                    score -= 350; /* Minus 350 points if there's an empty slot surrounded by blocks */
                }
            }
        }
    }
    return score;
}

float evaluate_board2(Game* game){
    float score = 0;
    score += clear_lines(game) * 0.76066; /* Points from lines cleared */
    score += evaluate_landscape2(game); /* Points from the landscape */
    return score;
}

/* Test for check_hole */
void print_nb_hole(Game* game){
    int x, y;   
    int nb_hole = 0;
    for (y = 1; y < SIZE_Y; y++){
        for (x = 0; x < SIZE_X; x++){
            if (!(game->board[y][x].filled)){
                if (check_hole(game, y, x)){
                    nb_hole++;
                }
            }
        }
    }
    printf("nb_hole = %d\n", nb_hole);
}


/* Initiate a AIMove with all its field to 0 */
static AIMoves initiate_iamoves(){
    AIMoves moves;
    moves.left = 0;
    moves.right = 0;
    moves.rotate_left = 0;
    moves.rotate_right = 0;
    moves.hold = 0;
    return moves;
}

/* Simulate the fall of the piece depending on the number of moves */
static void simulate_fall(Game* game, int move_interval, int fall_interval, int nb_move){
    game->y += (nb_move * move_interval) / fall_interval;
}


/* The function that will handle shifting the piece to the direction given in the arguments using shift_value */
static void aux_evaluate_position(Game* original, ConfigData** data, int nbrotations, int shift_value, int move_interval, int fall_interval, int nb_moves, int hold){
    Game tmp_game = *original; /* Temporary board to test out configs */
    float tmp_score;
    int i, tmp_x = original->x;
    int nb_shift = 0;

    simulate_fall(&tmp_game, move_interval, fall_interval, nb_moves); /* Simulate the fall of the piece */

    /* Loop to score each config for the piece */
    while (!check_position_conflict(&tmp_game, tmp_game.current_piece)){

        /* tmp_game.queue = *(copy_queue2(&(original->queue), &(tmp_game.queue))); */
        drop_piece(&tmp_game);
        tmp_score = evaluate_board2(&tmp_game);

        /* If the score is big enough to be stored in the list (the data are by scores, from best to worse) */
        if (tmp_score > (*data)[NB_CONFIG - 1].score){

            /* Check if the next config in the list has a higher score */
            for (i = NB_CONFIG - 1; i >= 0; i--){
                /* If yes then, i is the index of the previous config, else check the next one */
                if (tmp_score < (*data)[i].score){
                    i++;
                    break;
                }
                if (i == 0)
                    break; /* Break, else the loop will decrement i to -1 */
            }

            /* Storing new data here */
            (*data)[i].score = tmp_score;
            (*data)[i].config = tmp_game;
            (*data)[i].moves = initiate_iamoves(); /* resetting the moves */
            (*data)[i].moves.hold = hold;
            if (nb_shift < 0)
                (*data)[i].moves.left = -(nb_shift);
            else
                (*data)[i].moves.right = nb_shift;
            if (nbrotations < 3)
                (*data)[i].moves.rotate_left = nbrotations;
            else
                (*data)[i].moves.rotate_right = 1;

        }

        /* Resetting the game to the config before the shift */
        tmp_game = *original;

        /* Shifting the piece to the next position */
        tmp_x += shift_value;
        nb_shift = tmp_x - original->x ;
        nb_moves++;
        tmp_game.x = tmp_x;
        simulate_fall(&tmp_game, move_interval, fall_interval, nb_moves); /* Simulate the fall of the piece */
        /* free_queue(tmp_game.queue); */
    }
}


/* Evaluate each board obtained by shifting the current piece to the left as much as possible,
   then to the right, as much as possible. If a board has a better score than the one stored 
   in the score given in parameters, replace it and store the moves needed to reach that board */ 
static void evaluate_position(Game* original, ConfigData** data, int nbrotations, int move_interval, int fall_interval, int nb_moves, int hold){

    /* Evaluation of all position on the left */
    aux_evaluate_position(original, data, nbrotations, -1, move_interval, fall_interval, nb_moves, hold);

    /* Evaluation of all position on the right */
    aux_evaluate_position(original, data, nbrotations, 1, move_interval, fall_interval, nb_moves, hold);
}


/* Create and return a list of ConfigData of size given as parameter */
static ConfigData* initiate_config_list(int size){
    int i;
    ConfigData* data = (ConfigData*)malloc(sizeof(ConfigData) * size);
    for (i = 0; i < size; i++){
        /* Arbitrairy value for score here, I set it to -100000 so that the first evaluation
           will always result in a higher score */
        data[i].score = -100000;
        data[i].moves = initiate_iamoves();  /* Will store the movements the AI will have to make */
    }
    return data;
}


AIMoves search_board(Game* game, int move_interval, int fall_interval){
    /* Arbitrairy value for score here, I set it to -100000 so that the first evaluation
       will always result in a higher score */
    int i, j, hold = 0;
    ConfigData* data = initiate_config_list(NB_CONFIG);
    Game tmp_game = *game;              /* Temporary board to test out configs */
    int move_cmpt = 0;                  /* Counter for the number of moves needed before a drop */

    /* FILE* file = fopen("./score_test.txt", "ab"); */

    for (j = 0; j < 2; j++){

        if (j == 1){
            hold_piece(&tmp_game);
            hold = 1;
        }

        /* Rotate the piece and evaluate each possible horizontal position */
        for (i = 0; i < 4; i++){

            if (i < 3)
                move_cmpt = i;
            else
                move_cmpt = 1; /* One rotation to the right is enough */

            evaluate_position(&tmp_game, &data, i, move_interval, fall_interval, move_cmpt, hold);
            /* fprintf(file, "rotation %d = %f\n", i, score); */
            tmp_game.y = game->y; /* Restoring vertical position */
            rotate_left(&tmp_game);
        }
    }
    /*fprintf(file, "current score = %f\n", score);
    fprintf(file, "\n");
    fclose(file); */
    return data[0].moves;
}

/*
AIMoves search_board_level_2(Game* game, int move_interval, int fall_interval){
    int i, j, hold = 0;
    float scores[NB_CONFIG];
    AI_moves moves[NB_CONFIG];
    for (i = 0; i < NB_CONFIG; i++){
        moves[i] = initiate_iamoves();
        scores[i] = -100000;  Arbitrary value 
    }

}*/


/*
static void print_moves(AIMoves moves){
    printf("Left: %d\nRight: %d\nRotate left: %d\nRotate right: %d\n", moves.left, moves.right, moves.rotate_left, moves.rotate_right);
}
*/

int AI_play(Game* game, AIMoves* moves, int interval, Uint32* time_flag, Uint32 current_time){

    /* Do nothing if there is still time left before the next move */
    if (interval > current_time - *time_flag)
        return -1;

    /* Update the time_flag when the AI make a move */
    *time_flag = current_time;
    if (moves->hold > 0){
        hold_piece(game);
        (moves->hold)--;
        return HOLD;
    }
    if (moves->rotate_left > 0){
        rotate_left(game);
        (moves->rotate_left)--;
        return CTRL;
    }
    else if (moves->rotate_right > 0){
        rotate_right(game);
        (moves->rotate_right)--;
        return UP;
    }
    else if (moves->left > 0){
        move_piece_left(game);
        (moves->left)--;
        return LEFT;
    }
    else if (moves->right > 0){
        move_piece_right(game);
        (moves->right)--;
        return RIGHT;
    }
    else{
        drop_piece(game);
        return DROP;
    }
}
