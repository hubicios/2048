#define _USE_MATH_DEFINES
#include <time.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

extern "C" 
{
#include"./sdl-2.0.7/include/SDL.h"
#include"./sdl-2.0.7/include/SDL_main.h"
}

#include "Window.h"
#include "Field.h"
#include "Ranking.h"
#include "List.h"




// main
#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char **argv) 
{
	Window window;
	/*if (window.init_window() == false)
	{
		return 1;
	}*/
	if (window.init_window())
	{
		window.run_game();
	}

	return 0;
};


