#ifndef __POSITION__
#define __POSITION__

struct Position
{
    unsigned short X;
    unsigned short Y;
    unsigned short W;
    unsigned short Z; 
};

bool IsValidPosition(struct Position position);

int GenerateAleatValue(int min_num, int max_num);

#endif