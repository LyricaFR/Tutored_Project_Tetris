/*
Author: QUACH Kevin
Created on the : 12-02-2022
Last modification : 12-02-2022
*/

#include "../include/Input.h"



/* Old keys */
/*
const SDL_Keycode player_keys[2][NB_KEYS] = {{SDLK_LEFT, SDLK_RIGHT, SDLK_UP, SDLK_DOWN, SDLK_SPACE, SDLK_LSHIFT, SDLK_LCTRL},
                                             {SDLK_KP_1, SDLK_KP_3, SDLK_KP_5, SDLK_KP_2, SDLK_KP_ENTER, SDLK_KP_PLUS, SDLK_KP_0}};
*/


/* List of player 1 and player 2's keys */
const SDL_Keycode player_keys[2][NB_KEYS] = {{SDLK_q, SDLK_d, SDLK_z, SDLK_s, SDLK_SPACE, SDLK_LSHIFT, SDLK_f},
                                             {SDLK_j, SDLK_l, SDLK_i, SDLK_k, SDLK_RETURN, SDLK_RSHIFT, SDLK_m}};


/* This function will return the index of the key pressed in the
   key list */
KeyPress event_to_key(SDL_Event* Event, int game_nb){
    int i;
    SDL_Keycode key = Event->key.keysym.sym;
    for (i = 0; i < NB_KEYS; i++){
        if (player_keys[game_nb][i] == key){
            return i;
        }
    }
    return -1;
}

int move_piece(KeyPress key, Game* game){
    /* Select action based on the key pressed */
    switch (key){
        case LEFT:
            move_piece_left(game);
            break;
        case RIGHT:
            move_piece_right(game);
            break;
        case UP:
            rotate_right(game);
            break;
        case DOWN:
            move_piece_down(game);
            break;
        case DROP:
            drop_piece(game);
            break;
        case HOLD:
            if (game->can_hold){
                hold_piece(game);
                game->can_hold = 0;
            }
            break;
        case CTRL:
            rotate_left(game);
            break;
        default:
            return -1;
    }
    return key;
}

int handle_action(SDL_Event* Event, Game* game){
    KeyPress key = event_to_key(Event, game->number);
    return move_piece(key, game);
}

/*
Mode mode_choice(SDL_Event* Event){
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    if ((mouse_x >= SOLO_X) && (mouse_x <= SOLO_X + BUTTON_WIDTH)){
        if ((mouse_y >= SOLO_Y) && (mouse_y <= SOLO_Y + BUTTON_HEIGHT))
            return SOLO;
        if ((mouse_y >= VERSUS_Y) && (mouse_y <= VERSUS_Y + BUTTON_HEIGHT))
            return VERSUS;
        if ((mouse_y >= EXIT_Y) && (mouse_y <= EXIT_Y + BUTTON_HEIGHT))
            return EXIT;
    }
    return MENU;
}*/

Mode mode_choice(SDL_Event* Event, Button* buttons, Mode menu){
    int index = 0, i, mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);   /* Getting the mouse coordinates */
    if (menu == VERSUS)
        index = 6;
    for (i = index; i < NB_BUTTON; i++){     /* Check if the mouse is on one of the buttons */
        if ((mouse_x >= buttons[i].x) && (mouse_x <= buttons[i].x + BUTTON_WIDTH)){
            if ((mouse_y >= buttons[i].y) && (mouse_y <= buttons[i].y + BUTTON_HEIGHT))
                return buttons[i].mode;
        }
    }
    return menu;
}

void input_loop_solo(Game** games, SDL_Window* window, SDL_Surface* screen_surface, Mode* mode, int* action, Uint32* time_flag_autodrop, SDL_Surface** images, const struct timespec* tim,
                     int debug, SDL_Event* e){

    while (SDL_PollEvent(e) != 0){
        if (e->type == SDL_KEYDOWN){
            if (e->key.keysym.sym == SDLK_ESCAPE){
                *mode = EXIT;
            }
            else {

                *action = handle_action(e, games[0]);
                /* If the action changed the board */
                if (*action != -1) {
                    /* Evaluate the board whenever a piece is placed */
                    if (*action == DROP){
                        if (*mode == SOLO)
                            manage_board(games[0]);
                        else
                            manage_board_versus(games[0], games[1]);
                        *time_flag_autodrop = -1;
                    }
                    else if (*action == HOLD)
                        *time_flag_autodrop = -1;


                    if (*mode == SOLO){
                        if (debug) draw_board_ascii(games[0]);
                        draw_game(window, games[0], images, screen_surface);
                    }
                    else
                        draw_game_versus(window, games[0], games[1], images, screen_surface);
                    nanosleep(tim, NULL); /* Suspend the program */
                }
            }
        }
        nanosleep(tim, NULL); /* Suspend the program */
    }

}
