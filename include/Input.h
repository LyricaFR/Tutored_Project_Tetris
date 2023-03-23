/*
Author: QUACH Kevin
Created on the : 12-02-2022
Last modification : 12-02-2022
*/

#ifndef __INPUT__
#define __INPUT__

#include "Movement.h"
#include "Graphic.h"
#include "Audio.h"
#include <time.h>

#define NB_KEYS 7 /* Number of keys used by a player */

/* Enum of the keys used by a player */
typedef enum
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    DROP,
    HOLD,
    CTRL /* left rotation */
} KeyPress;


/* This function will return the index of the key pressed in the
   key list */
KeyPress event_to_key(SDL_Event* Event, int game_nb);

/* This function will move game's current_piece according to the key pressed 
   and return the number of the move from the KeyPress enum */
int move_piece(KeyPress key, Game* game);


int handle_action(SDL_Event* Event, Game* game);

/* This function will get the mouse's coordinate and return the game mode on which
   the mouse is pointing at */
Mode mode_choice(SDL_Event* Event, Button* buttons, Mode menu);


/* Loop used in the main to handle inputs during a game */
void input_loop_solo(Game** games, SDL_Window* window, SDL_Surface* screen_surface, Mode* mode, int* action, Uint32* time_flag_autodrop, SDL_Surface** images, const struct timespec* tim,
                     int debug, SDL_Event* e);

#endif