#include "../../entities/level/escene.h"
#include "../../config/config.h"

void UpdateEscene(struct Escene *escene, int player_x, int image_width, int screen_width)
{
    const int margin = 150; //PLAYER_MARGIN

    if (player_x < escene->Init_x + margin)
        escene->Init_x = player_x - margin;

    else if (player_x + PLAYER_X_RESOLUTION > escene->Init_x + screen_width - margin)
        escene->Init_x = player_x + PLAYER_X_RESOLUTION - screen_width + margin;

    if (escene->Init_x < 0)
        escene->Init_x = 0;

    if (escene->Init_x > image_width - screen_width)
        escene->Init_x = image_width - screen_width;

    escene->Fim_X = escene->Init_x + screen_width;
}

