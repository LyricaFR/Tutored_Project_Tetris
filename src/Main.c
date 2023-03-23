/*
Author: QUACH Kevin
Created on the : 05-02-2022
Last modification : 31-02-2022
*/


#include "../include/Input.h"
#include "../include/AI.h"
#include "../include/Graphic.h"
#include "../include/Audio.h"


int main(int argc, char* argv[]){

    int debug = 0, i;
    for (i = 0; i < argc; i++){
        if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--debug") == 0)
            debug = 1;
    }

    Mode mode = MENU; /* Game mode */

    srand(time(NULL));

    /* The window */
    SDL_Window* window = NULL;
    
    /* The window surface */
    SDL_Surface* screen_surface = NULL;

    /* The list of images */
    SDL_Surface** images = NULL;

    /* Event handler */
    SDL_Event e;

    /* Loading images */
    images = load_images();

    /* List of the menu buttons */
    Button* buttons = init_buttons(images);

/*    Mix_Music** gMusic = load_tracks(); */

    /* Time frame in milliseconds at which the pieces falls */
    int fall_interval = 1000; /* 1 sec */

    /* Time limit before the piece is autodropped when it's at the bottom */
    int autodrop_interval = 3000; /* 4 sec */

    /* Values used for nanosleep */
    struct timespec tim;
    tim.tv_sec = 0;
    tim.tv_nsec = 5000000; /* 0.005 sec */

    while (1){
        /* Initiation of menu window */
        if (mode == MENU){
            init_window(&window, &screen_surface, MENU_SCREEN_WIDTH, MENU_SCREEN_HEIGHT);
            draw_menu(window, images, buttons, screen_surface, mode, MENU);
        }

        /* Menu loop */
        while (mode == MENU || mode == VERSUS){
            while (SDL_PollEvent(&e) != 0){
                if (e.type == SDL_MOUSEMOTION){
                    /* Update the menu display */
                    draw_menu(window, images, buttons, screen_surface, mode, mode_choice(&e, buttons, mode));
                    nanosleep(&tim, NULL); /* Suspend the program */
                }
                /* If a mouse button is pressed */
                if (e.type == SDL_MOUSEBUTTONDOWN){
                    /* Choice of the game mode */
                    mode = mode_choice(&e, buttons, mode);
                    draw_menu(window, images, buttons, screen_surface, mode, mode_choice(&e, buttons, mode));
                    nanosleep(&tim, NULL); /* Suspend the program */
                }
            }
        }

        /* Close the menu window */
        close_window(window);


        /* Solo mode loop */
        if (mode == SOLO){

            /* Initiation of the game window */
            init_window(&window, &screen_surface, SCREEN_WIDTH, SCREEN_HEIGHT);

            Game* game = init_game(0);

            draw_game(window, game, images, screen_surface);

            if (debug) draw_board_ascii(game);

            Uint32 time_flag = SDL_GetTicks(); /* For the periodic drop */
            Uint32 time_flag_autodrop = -1;    /* For the autodrop */

            int action; /* Will store the last action */

            /* Main loop */
            while (mode == SOLO){

                /* Handles inputs and its effects on the game */
                input_loop_solo(&game, window, screen_surface, &mode, &action, &time_flag_autodrop, images, &tim, debug, &e);

                /* Make the block fall at a time interval */
                if (periodic_piece_fall(game, fall_interval, &time_flag, SDL_GetTicks())){
                    if (debug) draw_board_ascii(game);
                    draw_game(window, game, images, screen_surface);
                    nanosleep(&tim, NULL); /* Suspend the program */
                }

                /* Check if the piece is at the botton */
                check_if_at_bottom(game, &time_flag_autodrop);
                /* Autodrop it after the time limit */
                if (autodrop(game, autodrop_interval, &time_flag_autodrop, SDL_GetTicks())){
                    if (debug) draw_board_ascii(game);
                    manage_board(game);
                    draw_game(window, game, images, screen_surface);
                    nanosleep(&tim, NULL); /* Suspend the program */
                }

                if (game->lose){
                    mode = MENU;
                }
                
            }

            free(game);
            close_window(window);

        }


        /* Versus Player mode loop */
        else if (mode == VERSUS_PLAYER){

            /* Initiation of the game window */
            init_window(&window, &screen_surface, SCREEN_WIDTH * 2, SCREEN_HEIGHT);

            Game* games[2];
            games[0] = init_game(0);
            games[1] = init_game(1);

            draw_game_versus(window, games[0], games[1], images, screen_surface);

            /* For the periodic fall */
            Uint32 time_flags[2];
            time_flags[0] = SDL_GetTicks();
            time_flags[1] = SDL_GetTicks();

            /* For the autodrop */
            Uint32 time_flags_autodrop[2];
            time_flags_autodrop[0] = -1;
            time_flags_autodrop[1] = -1;

            int actions[2];

            while (mode == VERSUS_PLAYER){


                while (SDL_PollEvent(&e) != 0){
                    if (e.type == SDL_KEYDOWN){
                        if (e.key.keysym.sym == SDLK_ESCAPE){
                            mode = EXIT;
                        }
                        else {

                            actions[0] = handle_action(&e, games[0]);
                            actions[1] = handle_action(&e, games[1]);

                            /* Si l'action a changer le plateau */
                            if ((actions[0] != -1) || (actions[1] != -1)){

                                if (actions[0] == DROP){
                                    manage_board_versus(games[0], games[1]);
                                    time_flags_autodrop[0] = -1;
                                }
                                else if (actions[0] == HOLD)
                                    time_flags_autodrop[0] = -1;

                                if (actions[1] == DROP){
                                    manage_board_versus(games[1], games[0]);
                                    time_flags_autodrop[1] = -1;
                                }
                                else if (actions[1] == HOLD)
                                    time_flags_autodrop[1] = -1;

                                draw_game_versus(window, games[0], games[1], images, screen_surface);
                                nanosleep(&tim, NULL); /* Suspend the program */
                            }
                        }
                    }
                    nanosleep(&tim, NULL); /* Suspend the program */
                }

                /* Make the block fall at a time interval */
                if (periodic_piece_fall(games[0], fall_interval, &time_flags[0], SDL_GetTicks()) || 
                    periodic_piece_fall(games[1], fall_interval, &time_flags[1], SDL_GetTicks())){
                    draw_game_versus(window, games[0], games[1], images, screen_surface);
                    nanosleep(&tim, NULL); /* Suspend the program */
                }

                /* Check if the piece is at the botton */
                check_if_at_bottom(games[0], &time_flags_autodrop[0]);
                check_if_at_bottom(games[1], &time_flags_autodrop[1]);
                /* Autodrop it after the time limit */
                if (autodrop(games[0], autodrop_interval, &time_flags_autodrop[0], SDL_GetTicks())){
                    manage_board_versus(games[0], games[1]);
                    draw_game_versus(window, games[0], games[1], images, screen_surface);
                    nanosleep(&tim, NULL); /* Suspend the program */
                }

                if (autodrop(games[1], autodrop_interval, &time_flags_autodrop[1], SDL_GetTicks())) {
                    manage_board_versus(games[1], games[0]);
                    draw_game_versus(window, games[0], games[1], images, screen_surface);
                    nanosleep(&tim, NULL); /* Suspend the program */
                }

                if (games[0]->lose || games[1]->lose){
                    mode = MENU;
                }
            }

            free(games[0]);
            free(games[1]);
            close_window(window);
        }


        /* Versus AI mode loop */
        else if (mode == VERSUS_AI){

            /* Time between each action of the AI */
            int AI_interval = 150;

            /* Initiation of the game window */
            init_window(&window, &screen_surface, SCREEN_WIDTH * 2, SCREEN_HEIGHT);

            Game* games[2];
            games[0] = init_game(0);
            games[1] = init_game(1);

            draw_game_versus(window, games[0], games[1], images, screen_surface);

            Uint32 time_flag1 = SDL_GetTicks();
            Uint32 time_flag2 = SDL_GetTicks();
            Uint32 time_flag_AI = SDL_GetTicks();

            /* For the autodrop */
            Uint32 time_flag_autodrop1 = -1;
            Uint32 time_flag_autodrop2 = -1;

            int action1;
            int AI_action; /* Store the type of action of the AI */

            /* Generating AI_moves */
            AIMoves AI_moves = search_board(games[1], AI_interval, fall_interval);

            while (mode == VERSUS_AI){

                /* Handles inputs for the player */
                input_loop_solo(games, window, screen_surface, &mode, &action1, &time_flag_autodrop1, images, &tim, debug, &e);

                AI_action = AI_play(games[1], &AI_moves, AI_interval, &time_flag_AI, SDL_GetTicks());
                if (AI_action != -1){
                    /* If the AI has placed its pieces, generate its next moves */
                    if (AI_action == DROP){
                        AI_moves = search_board(games[1], AI_interval, fall_interval);
                        manage_board_versus(games[1], games[0]);
                        time_flag_autodrop2 = -1;
                    }
                    else if (AI_action == HOLD)
                        time_flag_autodrop2 = -1;
                    draw_game_versus(window, games[0], games[1], images, screen_surface);
                    nanosleep(&tim, NULL); /* Suspend the program */
                }

                /* Make the block fall at a time interval */
                if (periodic_piece_fall(games[0], fall_interval, &time_flag1, SDL_GetTicks()) || 
                    periodic_piece_fall(games[1], fall_interval, &time_flag2, SDL_GetTicks())){
                    draw_game_versus(window, games[0], games[1], images, screen_surface);
                    /* AI_moves = search_board(games[1]); */
                    nanosleep(&tim, NULL); /* Suspend the program */
                }

                /* Check if the piece is at the botton */
                check_if_at_bottom(games[0], &time_flag_autodrop1);
                check_if_at_bottom(games[1], &time_flag_autodrop2);
                /* Autodrop it after the time limit */
                if (autodrop(games[0], autodrop_interval, &time_flag_autodrop1, SDL_GetTicks())){
                    manage_board_versus(games[0], games[1]);
                    draw_game_versus(window, games[0], games[1], images, screen_surface);
                    nanosleep(&tim, NULL); /* Suspend the program */
                }

                if (autodrop(games[1], autodrop_interval, &time_flag_autodrop2, SDL_GetTicks())) {
                    manage_board_versus(games[1], games[0]);
                    draw_game_versus(window, games[0], games[1], images, screen_surface);
                    nanosleep(&tim, NULL); /* Suspend the program */
                }

                if (games[0]->lose || games[1]->lose){
                    mode = MENU;
                }
            }

            free(games[0]);
            free(games[1]);
            close_window(window);

        }

        if (mode == EXIT){
            close_app(window, images, buttons);
            return 1;
        }
    }
}