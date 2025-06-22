#include <stdlib.h>
#include "gun.h"

struct Gun* GunCreate(int cooldown)
{
    struct Gun *gun = malloc(sizeof(struct Gun));												
	if (!gun) return NULL;																		
	
    gun->Timer = 0;																				
	gun->Shots = NULL;
	gun->Cooldown = cooldown;
    																		
	return gun;
}

struct Bullet* GunShot(unsigned short x, unsigned short y, unsigned char trajectory, struct Gun *gun)
{				
	struct Bullet *new_bullet = BulletCreate(x, y, trajectory, gun->Shots);									
	if (!new_bullet) return NULL;																	
	
    return new_bullet;																			
}

void GunDestroy(struct Gun *element)
{																
	struct Bullet *sentinel;																				
	for (struct Bullet *index = element->Shots; index != NULL; index = sentinel){							
		sentinel = (struct Bullet*) index->Next;															
		BulletDestroy(index);																		
	}

	free(element);																					
}