#ifndef __LEVEL__
#define __LEVEL__

#include "../villains/villian.h"
#include "../progress/progress.h"
#include "../utilities/difficult.h"
#include "escene.h"

struct Fase{
    char Name[50];
    Difficult Level_difficult;
   
    int QtdVilhoes;
    int CurrentActiveVillians; //qtd viloes ativos na fase
    struct Villian **Villian; //vetor de villanos
    struct Player *Player;
    struct Progress Progress; 

    //STYLE PROPERTIES
    struct Escene Background; //rolling background    
    const char *Background_image; //background path
    const char *Music; //music path
    const char *SubVillianPath;
    const char *BossVillianPath;
};

struct Fase* CreateFase(Difficult level_dificult, int level, const char *player_basePath, char *player_name);

FaseState PlayFase(struct Fase *fase, GameState *state, ALLEGRO_EVENT_QUEUE *event_queue);

void DeleteFase(struct Fase *fase, ALLEGRO_TIMER *timer, ALLEGRO_BITMAP *bg , ALLEGRO_AUDIO_STREAM *music, ALLEGRO_FONT* font, ALLEGRO_FONT* player_font);

void DestroyFase(struct Fase *fase);

#endif