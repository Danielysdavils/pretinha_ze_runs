#ifndef __VILLIAN__
#define __VILLIAN__

#include "../../config/config.h"
#include "../../services/joystick/joystick.h"
#include "../../entities/gun/gun.h"
#include "../../entities/utilities/position.h"
#include "../utilities/difficult.h"
#include "../../entities/level/escene.h"
#include "../../entities/players/player.h"

#define VILLIAN_LIFE 1
#define VILLIAN_MARGIN 40

#define VILLIAN_FRAME_COUNT 4
#define VILLIAN_TOTAL_FRAMES (VILLIAN_FRAME_COUNT * 2)

#define FRAME_COUNT 4

typedef enum {
    BOSS,
    SUB
    
} VillianType;

struct Villian{
    char Name[50];
    ALLEGRO_BITMAP *Frames[VILLIAN_TOTAL_FRAMES];
    int Current_frame;
    float Frame_timer;
    float Frame_duration;
    int Face; // 0 -> esq, 1->dir, 2->abaixo
    struct Position Position;
    bool Fire;
    VillianType Type; //if hard? Boss : sub
    struct Gun *Gun;
    int QtdLife; //hard? x2 life
    int CurrentLife;
    struct Life *Life; //vetor de vidas
    bool Active;
};

struct Villian* CreateVillian(struct Position position, Difficult difficult, VillianType villian_type, char *Name, const char *villian_path);

void UpdateVillianFrames(struct Villian *villian, float delta_timer);

//void UpdateVillianBullet(struct Villian *villian, struct Escene background, int player_x);

void DrawVillian(struct Villian *villian, ALLEGRO_FONT *player_font, int background_x);

void DrawVillianBullet(struct Villian *villian, struct Escene background);

void MoveVillian(struct Villian *villian, int player_x, int background_x_init, int background_x_fim);

void ShotVillian(struct Villian *villian);

void DestroyVillian(struct Villian *villian);





#endif