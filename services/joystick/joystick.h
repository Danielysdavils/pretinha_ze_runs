#ifndef __JOYSTICK__ 																												
#define __JOYSTICK__																												 

#include <stdbool.h>

typedef struct {								
	bool left;
	bool right;
	bool up;
	bool down;
	bool fire;

} Joystick;


Joystick* JoystickCreate();

void JoystickDestroy(Joystick *js);																							
																	
void JoystickPress(Joystick *js, int keycode);	

void JoystickRelease(Joystick *js, int keycode);																								

#endif						