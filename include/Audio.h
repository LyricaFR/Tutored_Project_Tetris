/*
Author: QUACH Kevin
Created on the : 29-04-2022
Last modification : 29-04-2022
*/

#ifndef __AUDIO__
#define __AUDIO__

#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define NB_MUSIC 1

typedef enum{
    /*FluteDance,
    GreenMemory,
    Growing,
    NighttimeFun,
    PopUpBlueSky,*/
    Wishes
} TrackNum;

void init_audio();

void close_mix();

Mix_Music** load_tracks();

void free_tracks(Mix_Music** tracks);

#endif