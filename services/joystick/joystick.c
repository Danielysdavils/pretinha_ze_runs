#include <stdlib.h>
#include <stdio.h>
#include "../../config/config.h"
#include "joystick.h"

Joystick* JoystickCreate()
{														
	Joystick *js = malloc(sizeof(Joystick));						
	if (!js) return NULL;

	js->down = js->fire = js->left = js->right = js->up = false;															
	return js;																	
}

void JoystickDestroy(Joystick *control)
{ 
	free(control); 
}

void JoystickPress(Joystick *js, int keycode)
{
	switch (keycode)
	{
		case ALLEGRO_KEY_LEFT:
			js->left = true;
		break;

		case ALLEGRO_KEY_RIGHT:
			js->right = true;
		break;

		case ALLEGRO_KEY_UP:
			js->up = true;
		break;

		case ALLEGRO_KEY_DOWN:
			js->down = true;
		break;

		case ALLEGRO_KEY_SPACE:
			js->fire = true;
		break;

	}
}

void JoystickRelease(Joystick *js, int keycode)
{
	switch (keycode)
	{
		case ALLEGRO_KEY_LEFT:
			js->left = false;
		break;

		case ALLEGRO_KEY_RIGHT:
			js->right = false;
		break;

		case ALLEGRO_KEY_UP:
			js->up = false;
		break;

		case ALLEGRO_KEY_DOWN:
			js->down = false;
		break;

		case ALLEGRO_KEY_SPACE:
			js->fire = false;
		break;
	}
}



