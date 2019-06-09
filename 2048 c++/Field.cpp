#include"Field.h"


Field::Field(int n)
{

	Field::board_size=n;
	n = 9;
	Field::tile = new unsigned long int*[n];
	Field::previous_tile = new unsigned long int*[n];
	for (int i = 0; i < n; i++)
	{
		Field::tile[i] = new unsigned long int[n]();
		Field::previous_tile[i] = new unsigned long int[n]();
	}
	new_game(Field::board_size);
}


void Field::new_game(int board_size)
{
	Field::board_size = board_size;
	update_message("New game has started");
	Field::game_ended = false;
	Field::game_won = false;
	fill_tiles(Field::tile, 0);
	spawn_new();
	spawn_new();
	Field::score = 0;
	Field::previous_score = 0;
	update_previous();
}


Field::~Field()
{

	delete[] Field::tile;
}


void Field::arrow_pressed(int direction)
{
	int n = Field::board_size;
	unsigned long int **starting_tile;
	starting_tile=new unsigned long int*[n];
	
	int starting_score = Field::score;
	Field::correct_turn = false;
	
	for (int j = 0;j < n;j++)
	{
		starting_tile[j] = new unsigned long int[n];
		for (int i = 0; i < n; i++)
		{
			starting_tile[j][i] = Field::tile[j][i];
		}
	}

	move(direction,true);
	
	if (check_changes(Field::tile,starting_tile))
	{
		update_message("");
		/*if (score > max_scores[Field::boar)
		{
			max_score = score;
		}*/
		copy_tiles(Field::previous_tile, starting_tile);
		Field::previous_score=starting_score;
		spawn_new();
		if (empty_tiles() ==0)
		{
			copy_tiles(starting_tile,Field::tile);
			starting_score = Field::score;
			int possible_move = 0;
			for (int i = 0;i < 4;i++)//starts form right and does it for 4 directions
			{
				move(SDLK_RIGHT + i, false);
				if (check_changes(Field::tile, starting_tile))
				{
					copy_tiles(Field::tile, starting_tile);
					Field::score=starting_score;
					break;
				}
				if (i == 3)
				{
					Field::end_game();
				}
			}
		}

		check_winning();

	}


	for (int j = 0;j < n;j++)
	{
		delete[] starting_tile[j];
	}
	delete[] starting_tile;
}


void Field::merge_col(int j, int direction)
{
	int n = Field::board_size;
	int jump = 0;
	if (direction == SDLK_UP)
	{
		for (int i = 0;i < n - 1;i++)
		{
			jump = 0;
			while (i + (jump + 1) <n && Field::tile[j][i + (++jump)] == 0);

			if (i + jump < n&&Field::tile[j][i] == Field::tile[j][i+jump])
			{
				Field::tile[j][i] *= 2;
				score += Field::tile[j][i];
				Field::tile[j][i+jump] = 0;
				Field::correct_turn = true;
				i+=jump;
			}
		}
	}
	else if (direction == SDLK_DOWN)
	{
		for (int i = n-1;i >=1;i--)
		{
			jump = 0;
			while (i - (jump + 1) >= 0 &&Field::tile[j][i - (++jump)] == 0);

			if (i - jump >= 0&&Field::tile[j][i] == Field::tile[j][i - jump])
			{
				Field::tile[j][i] *= 2;
				score += Field::tile[j][i];
				Field::tile[j][i - jump] = 0;
				Field::correct_turn = true;
				i -= jump;
			}
		}
	}
}


void Field::merge_row(int i, int direction)
{
	int n = Field::board_size;
	int jump = 0;
	if (direction == SDLK_LEFT)
	{
		for (int j = 0;j < n - 1;j++)
		{
			jump = 0;
			while (j+(jump+1)<n&&Field::tile[j + (++jump)][i] == 0);

			if (j + jump <n&&Field::tile[j][i] == Field::tile[j +jump][i])
			{
				Field::tile[j][i] *= 2;
				score += Field::tile[j][i];
				Field::tile[j + jump][i] = 0;
				Field::correct_turn = true;
				j += jump;
			}
		}
	}
	else if (direction == SDLK_RIGHT)
	{
		for (int j = n-1;j >=1;j--)
		{
			jump = 0;
			while (j-(jump+1)>=0&&Field::tile[j - (++jump)][i] == 0);

			if (j - jump >=0 && Field::tile[j][i] == Field::tile[j -jump][i])
			{
				Field::tile[j][i] *= 2;
				score += Field::tile[j][i];
				Field::tile[j - jump][i] = 0;
				Field::correct_turn = true;
				j -= jump;
			}
		}
	}

}


bool Field::check_changes(unsigned long int ** tiles1, unsigned long int ** tiles2)
{
	int n = Field::board_size;
	for(int i=0;i<n;i++)
	{
		for (int j = 0; j < n; j++)
		{

			if (tiles1[j][i] != tiles2[j][i])
			{
				return true;
			}
		}
	}
	return false;
}


bool Field::spawn_new()
{
	srand(time(NULL)); 
	int n = Field::board_size;
	int empty = 0;//amount of fields without number
	empty = empty_tiles();
	int new_field = 0;
	if (empty > 0)
	{
		new_field = rand() % empty + 1;
		int new_digit=2;
		if (rand() % 10 == 0)
		{
			new_digit = 4;
		}
		empty = 0;
		for (int i = 0;i < n;i++)
		{
			for (int j = 0; j < n; j++)
			{

				if (Field::tile[j][i] == 0)
				{
					empty++;
					if (new_field == empty)
					{
						Field::tile[j][i] = new_digit;
					}
				}
			}
		}
	}
	else
	{
		return false;
	}
	return true;
}


int Field::empty_tiles()
{
	int empty = 0;
	int n = Field::board_size;
	for (int i = 0;i < n;i++)
	{
		for (int j = 0; j < n; j++)
		{

			if (Field::tile[j][i] == 0)
			{
				empty++;
			}
		}
	}
	return empty;
}


void Field::copy_tiles(unsigned long int **copy_to, unsigned long int **copy_from)
{
	int n = Field::board_size;
	for (int i = 0;i < n;i++)
	{
		for (int j = 0; j < n; j++)
		{
			copy_to[j][i] = copy_from[j][i];
		}
	}

}


void Field::update_previous()
{
	int n = Field::board_size;
	for (int i = 0;i < n;i++)
	{
		for (int j = 0; j < n; j++)
		{
			Field::previous_tile[j][i] = Field::tile[j][i];
		}
	}

}


void Field::fill_tiles(unsigned long int **tiles, int number)
{
	int n = Field::board_size;
	for (int i = 0;i < n;i++)
	{
		for (int j = 0; j < n; j++)
		{
			Field::tile[j][i] = number;
		}
	}

}


void Field::end_game()
{
	Field::game_ended = true;
	//make proper end game
	char text[128];
	save_score();

	sprintf(text,"game over, your score is %lu , time: %.1lf", Field::score,Field::game_time);
	update_message(text);
	//Field::score=0;
	
}


void Field::check_winning()
{
	int n = Field::board_size;
	for (int i = 0;i < n;i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (Field::tile[j][i] == 2048)
			{
				Field::game_won = true;
				j = n;
				i = n;
			}
			
		}
	}
}


unsigned long int Field::get_score()
{
	return Field::score;
}


unsigned long int Field::get_max_score()
{
	return Field::max_scores[Field::board_size];
}


void Field::undo()
{
	int n = Field::board_size;
	if (check_changes(Field::tile, Field::previous_tile)==true && Field::game_ended==false)
	{
		update_message("Previous turn has been restored");
			for (int i = 0;i < n;i++)
			{
				for (int j = 0; j < n; j++)
				{
					Field::tile[j][i] = Field::previous_tile[j][i];
				}
			}
		Field::score = Field::previous_score;
	}
	else if(empty_tiles()<n*n-2 && Field::game_ended == false)
	{
		update_message("Previous turn had already been restored");
	}
	else if(Field::game_ended == false)
	{
		update_message("");
	}

}


void Field::move(int direction, bool modify)
{
	int n = Field::board_size;
	if (direction == SDLK_UP)
	{
		for (int j = 0;j < n;j++)
		{
			Field::merge_col(j, direction);
			for (int i = 0;i < n;i++)
			{

				int up = i;
				while (up > 0)
				{
					if (Field::tile[j][--up] != 0)
					{
						up++;
						break;
					}
				}
				if (up >= 0 && up != i && Field::tile[j][i] != 0)
				{
					Field::tile[j][up] = Field::tile[j][i];
					Field::tile[j][i] = 0;
					Field::correct_turn = true;
				}
			}
		}
	}
	else if (direction == SDLK_DOWN)
	{
		for (int j = 0;j < n;j++)
		{
			Field::merge_col(j, direction);
			for (int i = n - 1;i >= 0;i--)
			{

				int down = i;
				while (down < n - 1)
				{
					if (Field::tile[j][++down] != 0)
					{
						down--;
						break;
					}
				}
				if (down < n && down != i && Field::tile[j][i] != 0)
				{
					Field::tile[j][down] = Field::tile[j][i];
					Field::tile[j][i] = 0;
					Field::correct_turn = true;
				}
			}
		}
	}
	else if (direction == SDLK_LEFT)
	{
		for (int i = 0;i < n;i++)
		{
			Field::merge_row(i, direction);
			for (int j = 0;j < n;j++)
			{
				int left = j;
				while (left > 0)
				{
					if (Field::tile[--left][i] != 0)
					{
						left++;
						break;
					}
				}
				if (left >= 0 && left != j && Field::tile[j][i] != 0)
				{
					Field::tile[left][i] = Field::tile[j][i];
					Field::tile[j][i] = 0;
					Field::correct_turn = true;
				}
			}
		}
	}
	else if (direction == SDLK_RIGHT)
	{
		for (int i = 0;i < n;i++)
		{
			Field::merge_row(i, direction);
			for (int j = n - 1;j >= 0;j--)
			{
				int right = j;
				while (right < n - 1)
				{
					if (Field::tile[++right][i] != 0)
					{
						right--;
						break;
					}
				}
				if (right < n && right != j && Field::tile[j][i] != 0)
				{
					//(j, i, right, i);
					Field::tile[right][i] = Field::tile[j][i];
					Field::tile[j][i] = 0;
					Field::correct_turn = true;
				}
			}
		}
	}

}


void Field::update_message(char* text)
{
	strcpy(Field::message,text);
}


void Field::update_time(double world_time)
{
	Field::game_time= world_time;
}


void Field::save_score()
{
	FILE *file;
	file = fopen("ranking.txt", "a");
	if (file != NULL)
	{
		fprintf(file, "\n%d %lu %.1lf", Field::board_size, Field::score, Field::game_time);
		fclose(file);
	}
}


void Field::get_best_scores(Ranking *Ranking)
{
	while (Ranking != NULL)
	{
		if (Ranking->score > Field::max_scores[Ranking->board_size])
		{
			Field::max_scores[Ranking->board_size] = Ranking->score;
		}
		Ranking = Ranking->next;
	}
}


void Field::save_game(char *file_name)
{
	all_file_names(file_name);
	strcat(file_name, ".txt");
	FILE *file;
	file = fopen(file_name, "wb");
	if (file != NULL)
	{
		fprintf(file, "%d ", Field::board_size);
		fprintf(file, "%lu ", Field::score);
		fprintf(file, "%lf ", Field::game_time);
		for (int j = 0;j < Field::board_size;j++)
		{
			for (int i = 0;i < Field::board_size;i++)
			{
				fprintf(file, "%lu ", Field::tile[i][j]);
			}
		}
		//fwrite((void*)field, sizeof(Field), 1, file);
		fclose(file);
	}
}

void Field::load_game(char *file_name)
{
	strcat(file_name, ".txt");
	FILE *file;
	file = fopen(file_name, "rb");
	if (file != NULL)
	{
		fscanf(file, "%d ", &board_size);
		fscanf(file, "%lu ", &score);
		fscanf(file, "%lf ", &game_time);
		for (int j = 0;j < Field::board_size;j++)
		{
			for (int i = 0;i < Field::board_size;i++)
			{
				fscanf(file, "%lu ", &tile[i][j]);
			}
		}
		update_message("Game loaded successfully");
		fclose(file);
	}
}


//access to private variable values (private variables /public methods)
bool Field::game_ended_val()
{
	return game_ended;
}

bool Field::game_won_val()
{
	return game_won;
}

double Field::game_time_val()
{
	return game_time;
}

int Field::board_size_val()
{
	return board_size;
}

unsigned long int Field::tile_val(int x, int y)
{
	if (x < board_size&&y < board_size)
	{
		return Field::tile[x][y];
	}
	return 0;
}

char* Field::message_val()
{
	return Field::message;
}

unsigned long int Field::max_scores_val(int n)
{
	if (n <= 9 && n >= 2)
	{
		return Field::max_scores[n];
	}
	return 0;
}