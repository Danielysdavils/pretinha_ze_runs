#include <stdlib.h>
#include "Bullet.h"

struct Bullet* BulletCreate(unsigned short x, unsigned short y, unsigned char trajectory, struct Bullet *next)
{			
	if ((trajectory < 0) || (trajectory > 1)) return NULL;													

	struct Bullet *new_bullet = malloc(sizeof(struct Bullet));													
	if (!new_bullet) return NULL;																			
	
    new_bullet->X = x;																						
	new_bullet->Y = y;																						
	new_bullet->Trajectory = trajectory;																	
	new_bullet->Next = (struct Bullet*) next;	

	return new_bullet;																						
}

void BulletDestroy(struct Bullet *element){																		
 	free(element);																							
}