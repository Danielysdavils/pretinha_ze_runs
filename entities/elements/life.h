#ifndef __LIFE__
#define __LIFE

#include "../../config/config.h"

#define LIFE_TOTAL_FRAMES 2

struct Life
{
    ALLEGRO_BITMAP *Frames[LIFE_TOTAL_FRAMES];
    int CurrentFrame;
    bool Active;
};

struct Life CreateLife();

void LoseLife(struct Life *life, int QtdLife);

void DestroyLife(struct Life *life);

#endif