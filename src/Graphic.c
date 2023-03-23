/*
Author: QUACH Kevin
Created on the : 12-02-2022
Last modification : 29-04-2022
*/

#include "../include/Graphic.h"

void draw_board_ascii(Game* game){
    int i, j;
    for (i = 0; i < SIZE_Y + 1; i++){
        for (j = 0; j < SIZE_X + 2; j++){
            if ((j == 0 || j == SIZE_X + 1 || i == SIZE_Y) && (i > 1))
                printf("X");
            else if(is_filled(game, j-1, i))
                printf("O");
            else
                printf(" ");
        }
        printf("\n");
    }
}


void init_window(SDL_Window** window, SDL_Surface** screen_surface, int width, int height){

    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS ) < 0 )
    {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        exit(1);
    }
    
    else
    {
        *window = SDL_CreateWindow( "Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN );
        if( *window == NULL )
        {
            fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            exit(1);
        }
        else
        {

            /* Initiate SLD Image */
            if (!IMG_Init(IMG_INIT_PNG)){
                fprintf(stderr, "Couldn't initialize SDL_image! SDL_Error: %s\n", IMG_GetError() );
                exit(1);
            }
            else{
                *screen_surface = SDL_GetWindowSurface( *window );

                SDL_FillRect( *screen_surface, NULL, SDL_MapRGB( (*screen_surface)->format, 200, 200, 200 ) );
                
                SDL_UpdateWindowSurface( *window );
            }
            
        }

    }

    /* Test display */
    /*
    SDL_Surface* image = IMG_Load("Assets/board.png");
    SDL_BlitSurface( image, NULL, screen_surface, NULL );
    SDL_UpdateWindowSurface( window );
    */
}


void close_window(SDL_Window* window){
    SDL_DestroyWindow( window );
}


void close_app(SDL_Window* window, SDL_Surface** images, Button* buttons){

    free_images(images);

    free(buttons);

    /* Destroy window to free memory */
    SDL_DestroyWindow( window );

    /* Quit SDL subsystems */
    IMG_Quit();
    SDL_Quit();
}


SDL_Surface** load_images(SDL_Surface* surface){
    int i;
    SDL_Surface** images = (SDL_Surface**)malloc(NB_IMAGE * sizeof(SDL_Surface*));
    if (images == NULL){
        fprintf(stderr, "Failed to load images.\n");
        exit(1);
    }
    images[main_menu] = IMG_Load("Assets/Images/main.png");
    images[solo] = IMG_Load("Assets/Images/solo.png");
    images[solo_pressed] = IMG_Load("Assets/Images/solo_pressed.png");
    images[versus] = IMG_Load("Assets/Images/versus.png");
    images[versus_pressed] = IMG_Load("Assets/Images/versus_pressed.png");
    images[versus_ai] = IMG_Load("Assets/Images/versus_AI.png");
    images[versus_ai_pressed] = IMG_Load("Assets/Images/versus_AI_pressed.png");
    images[versus_player] = IMG_Load("Assets/Images/versus_player.png");
    images[versus_player_pressed] = IMG_Load("Assets/Images/versus_player_pressed.png");
    images[back] = IMG_Load("Assets/Images/back.png");
    images[back_pressed] = IMG_Load("Assets/Images/back_pressed.png");
    images[exit_img] = IMG_Load("Assets/Images/exit.png");
    images[exit_pressed] = IMG_Load("Assets/Images/exit_pressed.png");
    images[interface] = IMG_Load("Assets/Images/interface.png");
    images[garbage_bar] = IMG_Load("Assets/Images/garbage_bar.png");
    images[purple_block] = IMG_Load("Assets/Images/block1.png");
    images[blue_block] = IMG_Load("Assets/Images/block2.png");
    images[pink_block] = IMG_Load("Assets/Images/block3.png");
    images[red_block] = IMG_Load("Assets/Images/block4.png");
    images[lime_block] = IMG_Load("Assets/Images/block5.png");
    images[green_block] = IMG_Load("Assets/Images/block6.png");
    images[yellow_block] = IMG_Load("Assets/Images/block7.png");
    images[garbage_block] = IMG_Load("Assets/Images/block8.png");
    for (i = 0; i < NB_IMAGE; i++){
        if (images[i] == NULL){
            fprintf(stderr,"Failed to load image. SDL_Error: %s\n", SDL_GetError());
            exit(1);
        }
    }
    return images;
}


void free_images(SDL_Surface** images){
    int i;
    for (i = 0; i < NB_IMAGE; i++){
        SDL_FreeSurface(images[i]);
        images[i] = NULL;
    }
    free(images);
}


static Button init_button(int x, int y, Mode mode, SDL_Surface* image){
    Button button;
    button.x = x;
    button.y = y;
    button.mode = mode;
    button.image = image;
    return button;
}


Button* init_buttons(SDL_Surface** images){
    Button* buttons = (Button*)malloc(NB_BUTTON * sizeof(Button));
    if (buttons == NULL){
        fprintf(stderr, "Failed to init buttons.\n");
        exit(1);
    }
    buttons[SOLO_button] = init_button(SOLO_X, SOLO_Y, SOLO, images[solo]);
    buttons[SOLO_button_pressed] = init_button(SOLO_X, SOLO_Y, SOLO, images[solo_pressed]);

    buttons[VERSUS_button] = init_button(VERSUS_X, VERSUS_Y, VERSUS, images[versus]);
    buttons[VERSUS_button_pressed] = init_button(VERSUS_X, VERSUS_Y, VERSUS, images[versus_pressed]);

    buttons[VERSUS_PLAYER_button] = init_button(SOLO_X, SOLO_Y, VERSUS_PLAYER, images[versus_player]);
    buttons[VERSUS_PLAYER_button_pressed] = init_button(SOLO_X, SOLO_Y, VERSUS_PLAYER, images[versus_player_pressed]);

    buttons[VERSUS_AI_button] = init_button(VERSUS_X, VERSUS_Y, VERSUS_AI, images[versus_ai]);
    buttons[VERSUS_AI_button_pressed] = init_button(VERSUS_X, VERSUS_Y, VERSUS_AI, images[versus_ai_pressed]);

    buttons[BACK_button] = init_button(EXIT_X, EXIT_Y, MENU, images[back]);
    buttons[BACK_button_pressed] = init_button(EXIT_X, EXIT_Y, MENU, images[back_pressed]);

    buttons[EXIT_button] = init_button(EXIT_X, EXIT_Y, EXIT, images[exit_img]);
    buttons[EXIT_button_pressed] = init_button(EXIT_X, EXIT_Y, EXIT, images[exit_pressed]);

    return buttons;
}


void blit_image(SDL_Surface* image, SDL_Surface* surface, int x, int y){

    /* Coordinate where the image will be drawn */
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;

    SDL_BlitSurface(image, NULL, surface, &dest);
}


void draw_menu(SDL_Window* window, SDL_Surface** images, Button* buttons, SDL_Surface* surface, Mode menu, Mode mode){
    blit_image(images[main_menu], surface, 0, 0);

    if (menu == MENU){ /* If Main Menu */
        blit_image(buttons[SOLO_button].image, surface, buttons[SOLO_button].x, buttons[SOLO_button].y);
        blit_image(buttons[VERSUS_button].image, surface, buttons[VERSUS_button].x, buttons[VERSUS_button].y);
        blit_image(buttons[EXIT_button].image, surface, buttons[EXIT_button].x, buttons[EXIT_button].y);
        if (mode == SOLO)
            blit_image(buttons[SOLO_button_pressed].image, surface, buttons[SOLO_button_pressed].x, buttons[SOLO_button_pressed].y);
        if (mode == VERSUS)
            blit_image(buttons[VERSUS_button_pressed].image, surface, buttons[VERSUS_button_pressed].x, buttons[VERSUS_button_pressed].y);
        if (mode == EXIT)
            blit_image(buttons[EXIT_button_pressed].image, surface, buttons[EXIT_button_pressed].x, buttons[EXIT_button_pressed].y);
    }
    if (menu == VERSUS){ /* If Versus menu */
        blit_image(buttons[VERSUS_PLAYER_button].image, surface, buttons[VERSUS_PLAYER_button].x, buttons[VERSUS_PLAYER_button].y);
        blit_image(buttons[VERSUS_AI_button].image, surface, buttons[VERSUS_AI_button].x, buttons[VERSUS_AI_button].y);
        blit_image(buttons[BACK_button].image, surface, buttons[BACK_button].x, buttons[BACK_button].y);
        if (mode == VERSUS_PLAYER)
            blit_image(buttons[VERSUS_PLAYER_button_pressed].image, surface, buttons[VERSUS_PLAYER_button_pressed].x, buttons[VERSUS_PLAYER_button_pressed].y);
        if (mode == VERSUS_AI)
            blit_image(buttons[VERSUS_AI_button_pressed].image, surface, buttons[VERSUS_AI_button_pressed].x, buttons[VERSUS_AI_button_pressed].y);
        if (mode == MENU)
            blit_image(buttons[BACK_button_pressed].image, surface, buttons[BACK_button_pressed].x, buttons[BACK_button_pressed].y);
    }
    SDL_UpdateWindowSurface( window );
}


void draw_game(SDL_Window* window, Game* game, SDL_Surface** images, SDL_Surface* surface){

    /* Draw the game interface */
    blit_image(images[interface], surface, (game->number * SCREEN_WIDTH), 0);

    draw_board(game, images, surface);

    /* Draw the current piece */
    draw_piece(game->current_piece, images, surface, 
              (game->number * SCREEN_WIDTH) + BOARD_X_CORNER + (game->x * BLOCK_LINE), game->y * BLOCK_LINE);

    /* Draw the hold piece */
    draw_hold_piece(game->hold, images, surface, game->number);

    /* Draw the queue */
    draw_queue(game, images, surface);

    /* Draw the garbage bar if there is any garbage */
    draw_garbage_bar(game, images, surface);

    SDL_UpdateWindowSurface( window );
}


void draw_game_versus(SDL_Window* window, Game* game1, Game* game2, SDL_Surface** images, SDL_Surface* surface){
    draw_game(window, game1, images, surface);
    draw_game(window, game2, images, surface);
}


void draw_board(Game* game, SDL_Surface** images, SDL_Surface* surface){

    int x, y;              /* Positions of the block on the board */
    int pixel_x, pixel_y;  /* Positions in pixels */

    for (y = 0; y < SIZE_Y; y++){
        for (x = 0; x < SIZE_X; x++){
            if (game->board[y][x].filled){
                pixel_x = (game->number * SCREEN_WIDTH) + BOARD_X_CORNER + (x * BLOCK_LINE);
                pixel_y = y * BLOCK_LINE;
                /* Draw block */
                blit_image(images[game->board[y][x].color], surface, pixel_x, pixel_y);
            }
        }
    }
}


void draw_piece(Piece piece, SDL_Surface** images, SDL_Surface* surface, int pixel_x, int pixel_y){
    int x, y;              /* Positions of the block on the board */
    for (y = 0; y < PIECE_SIZE; y++){
        for (x = 0; x < PIECE_SIZE; x++){
            if (piece.block[y][x].filled){
                blit_image(images[piece.type], surface, pixel_x + (x * BLOCK_LINE), pixel_y + (y * BLOCK_LINE));
            }
        }
    }
}


/* This function will put in x and y adjusted coordinates to center the
   hold and queue pieces depending on its type for the hold and queue pieces */
static void get_adjust_coordinate(Piece piece, int* x, int* y){
    switch (piece.type){
        case line:
            *x = BLOCK_SIZE / 2;
            *y = 0;
            break;
        case square:
            *x = (BLOCK_SIZE / 2);
            *y = -(BLOCK_SIZE / 2);
            break;
        default:
            *x = BLOCK_SIZE;
            *y = -(BLOCK_SIZE / 2);
    }
}


void draw_hold_piece(Piece piece, SDL_Surface** images, SDL_Surface* surface, int game_nb){
    int adjust_x, adjust_y; /* Values added to center the pieces */
    if (piece.type != null_piece){

        /* The coordinate will be adjusted depending the type of the piece */
        get_adjust_coordinate(piece, &adjust_x, &adjust_y);

        draw_piece(piece, images, surface, (game_nb * SCREEN_WIDTH) + HOLD_X_CORNER + adjust_x, HOLD_Y_CORNER + adjust_y); 
    }
}


void draw_queue(Game* game, SDL_Surface** images, SDL_Surface* surface){
    int adjust_x, adjust_y, i;

    for (i = 0; i < QUEUE_SIZE; i++){
        get_adjust_coordinate(game->queue[i], &adjust_x, &adjust_y);
        draw_piece(game->queue[i], images, surface, (game->number * SCREEN_WIDTH) + QUEUE_X_CORNER + adjust_x,
                   QUEUE_Y_CORNER + adjust_y + (BLOCK_SIZE * 3 * i));
    }
}


void draw_garbage_bar(Game* game, SDL_Surface** images, SDL_Surface* surface){
    int i;

    for (i = 0; i < game->garbage; i++){
        blit_image(images[garbage_bar], surface, (SCREEN_WIDTH * game->number) + GARBAGE_X_CORNER,
                     GARBAGE_Y_CORNER + GARBAGE_Y_SIZE - ((i + 1) * BLOCK_LINE));
    }
}