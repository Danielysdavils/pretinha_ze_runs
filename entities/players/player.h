#ifndef __PLAYER__
#define __PLAYER__

#include <stdbool.h>
#include "../../config/config.h"
#include "../../services/joystick/joystick.h"
#include "../../entities/gun/gun.h"
#include "../../entities/utilities/position.h"
#include "../../entities/level/level.h"
#include "../../entities/level/escene.h"
#include "../../entities/elements/life.h"

#define FRAME_COUNT 4
#define TOTAL_FRAMES (FRAME_COUNT * 4)

#define PLAYER_LIFE 4

struct Player{
    char Name[50];
    ALLEGRO_BITMAP *Frames[TOTAL_FRAMES];
    int Current_frame;
    float Frame_timer;
    float Frame_duration;
    unsigned char Face;
    struct Position Position;
    int QtdLife;
    int CurrentLife;
    struct Life *Life; //vetor de vidas
    bool IsJumping;
    bool IsCrouching;
    float VelJump;
    Joystick *Control;
    struct Gun *Gun; 
};

struct Player* CreatePlayer(struct Position position, const char *player_path,  const char *name);

void UpdatePlayerFrames(struct Player *player, float delta_timer);

//void UpdatePlayerBullet(struct Player *player, struct Escene background);

void DrawPlayer(struct Player *player, int background1_x, ALLEGRO_FONT *player_font);

void DrawPlayerBullet(struct Player *player, struct Escene Background);

void MovePlayer(struct Player *player);

void ShotPlayer(struct Player *player);

void DestroyPlayer(struct Player *player);


#endif