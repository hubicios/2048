#pragma once
#ifndef FIELD_H
#define FIELD_H

#define _USE_MATH_DEFINES
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Ranking.h"
#include "List.h"

extern "C" {
#include"./sdl-2.0.7/include/SDL.h"
#include"./sdl-2.0.7/include/SDL_main.h"
}

class Field
{	
private:
	char message[128]="hello";
	double game_time = 0;
	bool game_ended = false;
	bool game_won = false;
	bool correct_turn = false;
	int board_size = 4;//board is NxN
	unsigned long int **tile; //tile[x][y]
	unsigned long int **previous_tile; //previous turn
	unsigned long int score = 0;
	unsigned long int previous_score = 0; //previous turn
	int size = 50;
	int space_size = 5;
	int x_start = 50;
	int y_start = 150;
	unsigned long int max_scores[10] = {};

public:

	Field(int board_size);
	~Field();

	void arrow_pressed(int direction);
	void merge_col(int col, int direction);
	void merge_row(int row, int direction);
	void move(int direction, bool modify); //modify true=real move ,returns true if moving is possible
	void update_previous();
	void copy_tiles(unsigned long int **copy_to, unsigned long int **copy_from);
	void fill_tiles(unsigned long int **tiles,int number);
	void undo();
	void new_game(int board_size);
	bool spawn_new();
	int empty_tiles();
	void end_game();
	unsigned long int get_score();
	void save_score();
	unsigned long int get_max_score();
	unsigned long int max_scores_val(int n);
	bool check_changes(unsigned long int ** tiles1, unsigned long int ** tiles2);
	void check_winning();
	void update_message(char* text);
	void update_time(double world_time);
	void get_best_scores(Ranking *Ranking);
	void save_game(char *file_name);
	void load_game(char *file_name);

	//access to private variable values (private variables /public methods)
	bool game_ended_val();
	bool game_won_val();
	double game_time_val();
	int board_size_val();
	unsigned long int tile_val(int x,int y);
	char* message_val();
};


#endif /* FIELD_H */
