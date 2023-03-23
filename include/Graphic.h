/*
Author: QUACH Kevin
Created on the : 12-02-2022
Last modification : 29-04-2022
*/

#ifndef __GRAPHIC__
#define __GRAPHIC__

#include "Game.h"
#include "Versus.h"
#include <SDL2/SDL_image.h>

#define NB_IMAGE 23  /* The number of image used */

/* Size in pixels */

/* Menu size and coordinates */

#define MENU_SCREEN_WIDTH 1000
#define MENU_SCREEN_HEIGHT 1000

#define BUTTON_WIDTH 380
#define BUTTON_HEIGHT 136 

#define SOLO_X 310                /* X position of the upper left corner of solo button */
#define SOLO_Y 280                /* Y position of the upper left corner of solo button */

#define VERSUS_X SOLO_X           /* X position of the upper left corner of versus button */
#define VERSUS_Y 480              /* Y position of the upper left corner of versus button */

#define EXIT_X SOLO_X             /* X position of the upper left corner of exit button */
#define EXIT_Y 680                /* Y position of the upper left corner of exit button */


/* Game size and coordinates */

#define SCREEN_WIDTH 715
#define SCREEN_HEIGHT 736

#define BLOCK_SIZE 32                     /* Size of blocks */
#define BLOCK_LINE (BLOCK_SIZE + 1)       /* Size of block + separating line */
#define BORDER_SIZE 11                    /* Size of the borders */
#define TOP_BORDER 66                     /* Size of the top border */

#define HOLD_X_CORNER BORDER_SIZE           /* X position of the upper left corner of hold box */
#define HOLD_Y_CORNER TOP_BORDER            /* Y position of the upper left corner of hold box */
#define HOLD_X_SIZE 160                     /* Horizontal size of the whole hold box */
#define HOLD_Y_SIZE 96                     /* Vertical size of the whole hold box */

#define GARBAGE_X_CORNER (HOLD_X_SIZE + BORDER_SIZE * 2)    /* X position of the upper left corner of the garbage bar */
#define GARBAGE_Y_CORNER TOP_BORDER                         /* Y position of the upper left corner of the garbage bar */
#define GARBAGE_X_SIZE 11                                   /* Horizontal size of the garbage bar */
#define GARBAGE_Y_SIZE ((BLOCK_LINE * GAME_SIZE_Y) - 1)     /* Vertical size of the garbage bar */

#define BOARD_X_CORNER (HOLD_X_SIZE + GARBAGE_X_SIZE + BORDER_SIZE * 3)      /* X position of coordinate (0,0) of the board */
#define BOARD_Y_CORNER TOP_BORDER                                            /* Y position of coordinate (0,0) of the board */
#define BOARD_X_SIZE 329                                                     /* Horizontal size of the whole board */
#define BOARD_Y_SIZE ((BLOCK_LINE * GAME_SIZE_Y) - 1)                        /* Vertical size of the whole board */

#define QUEUE_X_CORNER (HOLD_X_SIZE + GARBAGE_X_SIZE + BOARD_X_SIZE + BORDER_SIZE * 4)  /* X position of the upper left corner of the queue box */
#define QUEUE_Y_CORNER TOP_BORDER                                                       /* X position of the upper left corner of the queue box */

typedef enum{
    purple_block,
    blue_block,
    pink_block,
    red_block,
    lime_block,
    green_block,
    yellow_block,
    garbage_block
} ColorNum;


/* Starts at 8 because it's the continuation of ColorNum */

typedef enum{
    main_menu = 8,
    solo = 9,
    solo_pressed = 10,
    versus = 11,
    versus_pressed = 12,
    exit_img = 13,
    exit_pressed = 14,
    versus_player = 15,
    versus_player_pressed = 16,
    versus_ai = 17,
    versus_ai_pressed = 18,
    back = 19,
    back_pressed = 20,
    interface = 21,
    garbage_bar = 22
} ImageNum;

#define NB_BUTTON 12  /* The number of buttons */

typedef enum{  /* Don't change the order ! */
   SOLO_button,
   SOLO_button_pressed,
   VERSUS_button,
   VERSUS_button_pressed,
   EXIT_button,
   EXIT_button_pressed,
   VERSUS_PLAYER_button,
   VERSUS_PLAYER_button_pressed,
   VERSUS_AI_button,
   VERSUS_AI_button_pressed,
   BACK_button,
   BACK_button_pressed
} ButtonNum;

typedef struct button {
    int x;     /* Position x of upper left corner of the button */
    int y;     /* Position y of upper left corner of the button */
    Mode mode; /* Mode the button is associated to */
    SDL_Surface* image;
} Button;


/* Draw the board in ascii */
void draw_board_ascii(Game* game);

/* Initiate the SLD window */
void init_window(SDL_Window** window, SDL_Surface** screenSurface, int width, int height);

/* Close the window given as argument */
void close_window(SDL_Window* window);

/* Free memory and close the app */
void close_app(SDL_Window* window, SDL_Surface** images, Button* buttons);

/* Load images in the Assets repertory into a list of SDL_Surface* and returns it */
SDL_Surface** load_images();

/* Free the allocated memory for the images */
void free_images(SDL_Surface** images);

/* Init the buttons and return a list of the buttons */
Button* init_buttons(SDL_Surface** images);

/* Display the image given on the window at the coordinate x,y */
void blit_image(SDL_Surface* image, SDL_Surface* surface, int x, int y);

/* Draw the menu, menu is either MENU or VERSUS. If the mouse is on a button, that button will be drawn in its pressed version */
void draw_menu(SDL_Window* window, SDL_Surface** images, Button* buttons, SDL_Surface* surface, Mode menu, Mode mode);

/* Draw the whole game interface */
void draw_game(SDL_Window* window, Game* game, SDL_Surface** images, SDL_Surface* surface);

/* Draw two game interface for a versus mode */
void draw_game_versus(SDL_Window* window, Game* game1, Game* game2, SDL_Surface** images, SDL_Surface* surface);

/* Draw the board */
void draw_board(Game* game, SDL_Surface** images, SDL_Surface* surface);

/* Draw the piece given as argument at the coordinate (pixel_x, pixel_y) */
void draw_piece(Piece piece, SDL_Surface** images, SDL_Surface* surface, int pixel_x, int pixel_y);

/* Draw the hold piece in the hold box */
void draw_hold_piece(Piece piece, SDL_Surface** images, SDL_Surface* surface, int game_nb);

/* Draw the pieces of the queue in the queue box */
void draw_queue(Game* game, SDL_Surface** images, SDL_Surface* surface);

/* Draw the garbage bar according to the number of garbage accumulated */
void draw_garbage_bar(Game* game, SDL_Surface** images, SDL_Surface* surface);


#endif