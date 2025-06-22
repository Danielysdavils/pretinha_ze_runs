#include <stdbool.h>
#include <stdlib.h>    
#include <time.h>

#include "position.h"

bool IsValidPosition(struct Position position)
{
    return true;
}

int GenerateAleatValue(int min_num, int max_num)
{
    int low_num = min_num < max_num ? min_num : max_num;
    int hi_num = min_num < max_num ? max_num : min_num;
    return (rand() % (hi_num - low_num + 1)) + low_num;
}

