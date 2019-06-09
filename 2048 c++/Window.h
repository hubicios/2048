#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include"Ranking.h"
#include"Field.h"
#include"List.h"

extern "C" {
#include"./sdl-2.0.7/include/SDL.h"
#include"./sdl-2.0.7/include/SDL_main.h"
}

#define SCREEN_WIDTH	1280
#define SCREEN_HEIGHT	720
#define BLACK_TEXT	0
#define WHITE_TEXT	1

class Window
{
	SDL_Surface *screen, *charset[2];
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;

	int black;
	int white;
	int numbers_color[12];
	int board_color;
	int board_border_color;

public:
	Window();
	~Window();

	bool init_window();
	void init_colors();
	void draw_string(int x, int y, const char *text, int color);
	void draw_pixel(int x, int y, Uint32 color);
	void draw_line(int x, int y, int l, int dx, int dy, Uint32 color);
	void draw_rectangle(int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);
	void draw_empty_board(int field_size, int size);
	void choose_board_size(int *user_board_size, SDL_Event *event);
	void choose_save(Field *field, SDL_Event *event);
	void draw_legend(int x, int y, int board_size);
	void run_game();
	void show_ranking(Ranking *ranking, int board_size);
	void show_saves(List *saves);
	int window_width();
	int window_height();
	
};
#endif /* WINDOW_H */