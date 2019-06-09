#include "Ranking.h"


Ranking::Ranking()
{
}


Ranking::~Ranking()
{
}

void Ranking::init() //init list
{
	Ranking::next = NULL;
}

void Ranking::add_after(int board_size1, int score1, double time1) //adding new record
{
	Ranking *x = new Ranking;

	x->board_size = board_size1;
	x->score = score1;
	x->time = time1;
	x->next = Ranking::next;
	Ranking::next = x;
}

void Ranking::remove_after() // Deletes after
{
	if (Ranking::next != NULL) {
		void* x = Ranking::next;
		Ranking::next = Ranking::next->next;
		delete x;
	}
}


void load_scores(Ranking *ranking)
{
	//int n = 0;
	FILE *file;
	file = fopen("ranking.txt", "rb");
	if (file != NULL)
	{
		int board_size = 0, score = 0;
		double time = 0;
		while (fscanf(file, "%d %d %lf", &board_size, &score, &time) != EOF)
		{
			if (board_size != 0 && score != 0 && time != 0)
			{
				ranking->add_after(board_size, score, time);
				//n++;
			}
		}
		fclose(file);
	}
}
void sort_by(Ranking *ranking, bool by_time, int board_size)
{
	//ranking = ranking->next;//head
	Ranking *ptr = ranking;
	int n = 0;
	while (ranking != NULL)
	{
		//if (ranking->board_size == board_size)
		{
			n++;
		}
		ranking = ranking->next;
	}
	ranking = ptr;

	Ranking **sorted_ranking = new Ranking*[n];

	n = 0;
	while (ranking != NULL)
	{
		//if (ranking->board_size == board_size)
		{
			sorted_ranking[n] = ranking;
			n++;
		}
		ranking = ranking->next;
	}
	ranking = ptr;

	unsigned long int buffer[2];
	double buffer_d;
	Ranking buffer_Ranking;
	for (int i = 0;i < n;i++)
	{
		for (int j = i;j < n;j++)
		{
			if ((sorted_ranking[j]->score > sorted_ranking[i]->score&& by_time == false) || (sorted_ranking[j]->time < sorted_ranking[i]->time&& by_time == true))
			{
				buffer[0] = sorted_ranking[i]->board_size;
				buffer[1] = sorted_ranking[i]->score;
				buffer_d = sorted_ranking[i]->time;

				sorted_ranking[i]->board_size = sorted_ranking[j]->board_size;
				sorted_ranking[i]->score = sorted_ranking[j]->score;
				sorted_ranking[i]->time = sorted_ranking[j]->time;

				sorted_ranking[j]->board_size = buffer[0];
				sorted_ranking[j]->score = buffer[1];
				sorted_ranking[j]->time = buffer_d;
			}
		}
	}

	n = 0;
	buffer[0]=0;
	buffer[1]=0;
	buffer_d=0;
	while (ranking != NULL)
	{
		buffer[0] = sorted_ranking[n]->board_size;
		buffer[1] = sorted_ranking[n]->score;
		buffer_d = sorted_ranking[n]->time;

		ranking->board_size = buffer[0];
		ranking->score = buffer[1];
		ranking->time = buffer_d;
		ranking = ranking->next;

		n++;
	}

	delete[] sorted_ranking;
}




