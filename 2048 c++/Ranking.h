#pragma once
#ifndef RANKING_H
#define RANKING_H

#include<stdio.h>

//#define _USE_MATH_DEFINES
//#include<math.h>
//#include<stdlib.h>
//#include<string.h>
//#include"Field.h"
//#include"Window.h"

class Ranking
{
public:
	int board_size = 0;
	long unsigned int score = 0;
	double time = 0;
	class Ranking *next;

	Ranking();
	~Ranking();
	void init();
	void add_after(int board_size1, int score1, double time1);
	void remove_after();

	
};

void load_scores(Ranking *ranking);
//void show(Ranking *Ranking, SDL_Surface *screen, SDL_Surface *charset, int board_size);
//void show(Ranking *Ranking, Window *window, int board_size);


void sort_by(Ranking *ranking, bool by_time, int board_size);

#endif /* RANKING_H */