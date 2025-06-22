#ifndef __ARM__
#define __ARM__

#include "bullet.h"

struct Gun{
    unsigned char Timer;
    struct Bullet *Shots;
    int Cooldown;
};

struct Gun* GunCreate();																									
struct Bullet* GunShot(unsigned short x, unsigned short y, unsigned char trajectory, struct Gun *gun);								
void GunDestroy(struct Gun *element);

#endif