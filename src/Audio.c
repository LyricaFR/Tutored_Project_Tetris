/*
Author: QUACH Kevin
Created on the : 29-04-2022
Last modification : 29-04-2022
*/

#include "../include/Audio.h"

void init_audio(){

    if( SDL_Init(SDL_INIT_AUDIO) < 0 )
    {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        exit(1);
    }

    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
            printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
            exit(1);
    }
}

void close_mix(){
    Mix_Quit();
}

Mix_Music** load_tracks(){
    int i;
    Mix_Music** tracks = (Mix_Music**)malloc(NB_MUSIC * sizeof(Mix_Music*));
    /*tracks[FluteDance] = Mix_LoadMUS("Assets/Musics/Flute Dance.ogg");
    tracks[GreenMemory] = Mix_LoadMUS("Assets/Musics/Green Memory.mp3");
    tracks[Growing] = Mix_LoadMUS("Assets/Musics/Growing.mp3");
    tracks[NighttimeFun] = Mix_LoadMUS("Assets/Musics/Nighttime Fun.mp3");
    tracks[PopUpBlueSky] = Mix_LoadMUS("Assets/Musics/Pop Up Blue Sky.mp3");*/
    tracks[Wishes] = Mix_LoadMUS("Assets/Musics/Wishes.mp3");
    for (i = 0; i < NB_MUSIC; i++){
        if (tracks[i] == NULL){
            fprintf(stderr,"Failed to load musics. Mix_Error: %s\n", Mix_GetError());
            exit(1);
        }
    }
    return tracks;
}

void free_tracks(Mix_Music** tracks){
    int i;
    for (i = 0; i < NB_MUSIC; i++){
        Mix_FreeMusic(tracks[i]);
        tracks[i] = NULL;
    }
    free(tracks);
}   