#include <stdio.h>
#include "life.h"

bool UploadLifeFrames(struct Life *life)
{
    char filename[256];
    for(int i = 0; i < LIFE_TOTAL_FRAMES; i++)
    {
        snprintf(filename, sizeof(filename), "%s/%d.png", LIFE_PATH, i);
        life->Frames[i] = al_load_bitmap(filename);
        if(!life->Frames[i])
        {
            printf("[ERRO]: UPLOAD LIFE FRAMES %s\n", filename);
            return false;
        }
    }
    return true;
}

struct Life CreateLife()
{
    struct Life life;

    if(!UploadLifeFrames(&life))
        printf("[ERROR]: UPLOAD FRAMES LIFE");
        
    life.CurrentFrame = 1;
    life.Active = true;
    return life;
}

void LoseLife(struct Life *life, int QtdLife)
{
    for(int i = 0; i < QtdLife; i++)
    {
        if(life[i].Active)
        {
            life[i].Active = false;
            life[i].CurrentFrame = 0;
            break;
        }
        
    }
}

void DestroyLife(struct Life *life)
{
    for(int i = 0; i < LIFE_TOTAL_FRAMES; i++)
    {
        if(life->Frames[i])
            al_destroy_bitmap(life->Frames[i]);
    }
}