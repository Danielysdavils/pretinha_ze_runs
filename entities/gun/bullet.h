#ifndef __BULLET__
#define __BULLET__

#include "../../config/config.h"
#include "../players/player.h"

#define BULLET_MOVE 20

typedef enum {
    UP,
    LEFT,
    RIGHT

} Trajetory;

struct Bullet{
    unsigned short X;
    unsigned short Y;
    unsigned char Trajectory;
    struct Bullet *Next;
};

struct Bullet* BulletCreate(unsigned short x, unsigned short y, unsigned char trajectory, struct Bullet *next);

void BulletUpdate(struct Bullet *el);

void BulletDestroy(struct Bullet *el);

#endif