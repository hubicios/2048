#include"Window.h"


Window::Window()
{

}


Window::~Window()
{

}
void Window::init_colors()
{

	black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	white = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);
	board_color = SDL_MapRGB(screen->format, 0xBB, 0xAD, 0xA0);
	board_border_color = SDL_MapRGB(screen->format, 0xA8, 0x9B, 0x90);
	numbers_color[0] = SDL_MapRGB(screen->format, 205, 193, 180);//everything else
	numbers_color[1] = SDL_MapRGB(screen->format, 238, 228, 218);//2
	numbers_color[2] = SDL_MapRGB(screen->format, 237, 224, 200);//4
	numbers_color[3] = SDL_MapRGB(screen->format, 242, 177, 121);//8
	numbers_color[4] = SDL_MapRGB(screen->format, 245, 149, 99);//16
	numbers_color[5] = SDL_MapRGB(screen->format, 246, 124, 95);//32
	numbers_color[6] = SDL_MapRGB(screen->format, 246, 94, 59);//64
	numbers_color[7] = SDL_MapRGB(screen->format, 237, 207, 114);//128
	numbers_color[8] = SDL_MapRGB(screen->format, 237, 204, 97);//256
	numbers_color[9] = SDL_MapRGB(screen->format, 237, 200, 80);//512
	numbers_color[10] = SDL_MapRGB(screen->format, 237, 197, 63);//1024
	numbers_color[11] = SDL_MapRGB(screen->format, 237, 194, 46);//2048
}
bool Window::init_window()
{
	int rc;

	printf("printf output goes here\n");

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return false;
	}

	// fullscreen mode
	//	rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
	//	                                 &window, &renderer);
	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		&(Window::window), &(Window::renderer));
	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return false;
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(Window::renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(Window::renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "2048");


	Window::screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	Window::scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);


	// disable showing cursor
	SDL_ShowCursor(SDL_DISABLE);

	// load cs8x8.bmp
	Window::charset[WHITE_TEXT] = SDL_LoadBMP("./cs8x8.bmp");
	if (Window::charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(Window::screen);
		SDL_DestroyTexture(Window::scrtex);
		SDL_DestroyWindow(Window::window);
		SDL_DestroyRenderer(Window::renderer);
		SDL_Quit();
		return false;
	};
	SDL_SetColorKey(Window::charset[WHITE_TEXT], true, 0x000000);

	// load cs8x8_black.bmp
	Window::charset[BLACK_TEXT] = SDL_LoadBMP("./cs8x8_black.bmp");
	if (Window::charset[BLACK_TEXT] == NULL) {
		printf("SDL_LoadBMP(cs8x8_black.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(Window::screen);
		SDL_DestroyTexture(Window::scrtex);
		SDL_DestroyWindow(Window::window);
		SDL_DestroyRenderer(Window::renderer);
		SDL_Quit();
		return false;
	};
	SDL_SetColorKey(Window::charset[BLACK_TEXT], true, 0xFFFFFF);

	return true;
}
void Window::run_game()
{
	int t1, t2, quit=0, frames=0,user_board_size = 4,line = 0;
	double delta, worldTime=0, fpsTimer=0, fps=0;
	SDL_Event event;
	char text[128];
	bool save_once = true, won_once = true,first_move = true,visible_ranking_r = false,visible_ranking_t = false,visible_ranking_p = false,sort_by_time = false;

	SDL_FillRect(Window::screen, NULL, black);
	init_colors();
	choose_board_size(&user_board_size, &event);
	Field field(user_board_size);
	Ranking ranking;
	ranking.init();
	
	load_scores(&ranking);
	field.get_best_scores(&ranking);

	//*************************************************************************************************************
	while (!quit) {
		user_board_size = field.board_size_val();
		// here t2-t1 is the time in milliseconds since
		// the last screen was drawn
		// delta is the same time in seconds
		if (first_move == true)
		{
			t1 = SDL_GetTicks();
		}
		else if (field.game_ended_val() == false)
		{
			t2 = SDL_GetTicks();
			delta = (t2 - t1) * 0.001;
			t1 = t2;

			worldTime += delta;
			if (field.game_ended_val() == false)
			{
				field.update_time(worldTime);
			}

			fpsTimer += delta;
			if (fpsTimer > 0.5) {
				fps = frames * 2;
				frames = 0;
				fpsTimer -= 0.5;
			};
		}
		SDL_FillRect(Window::screen, NULL, black);


		if (visible_ranking_r) //ranking when pressed 'r'
		{
			int n = 1;
			char text[128] = "";
			int start_x = 100, start_y = 100;
			sprintf(text, "Best scores for every board ");
			Window::draw_string(start_x, start_y, text, WHITE_TEXT);
			for (int i = 2;i < 10;i++)
			{
				if (field.max_scores_val(i) > 0)
				{
					sprintf(text, "%dx%d best score: %lu ", i, i, field.max_scores_val(i));
					Window::draw_string(start_x, start_y+12*n, text, WHITE_TEXT);
					n++;
				}
			}
			if (n == 1)
			{
				sprintf(text, "no records");
				Window::draw_string(start_x, start_y + 12 * n, text, WHITE_TEXT);
			}
		}
		if (visible_ranking_t || visible_ranking_p) //ranking when pressed 'p' or 't'
		{
			show_ranking(&ranking, field.board_size_val());
		}


		//board content
		int field_size = 55;
		if (Window::screen->w >= field.board_size_val() * field_size + 5 && Window::screen->h >= field.board_size_val() * field_size + 5 + 10)
		{
			char text[128];
			int x = Window::screen->w / 2 - field.board_size_val() * field_size / 2;
			int y = Window::screen->h / 2 - field.board_size_val() * field_size / 2;

			line = 1;
			sprintf(text, "previous best: %lu", field.get_max_score());
			draw_string(x, y - 5 - 10 * line++, text, WHITE_TEXT);
			sprintf(text, "time:  %02d:%04.1lf", (int)worldTime/60, fmod(worldTime,60.0));
			draw_string(x, y - 5 - 10 * line++, text, WHITE_TEXT);
			sprintf(text, "score: %lu ", field.get_score());
			draw_string(x, y - 5 - 10 * line++, text, WHITE_TEXT);

			draw_legend(Window::screen->w - 350, 100, field.board_size_val());
			

			sprintf(text, field.message_val());
			draw_string(x, y + field.board_size_val() * field_size + 5, text, WHITE_TEXT);
			Window::draw_rectangle(x - 5, y - 5, field_size * field.board_size_val() + 5, field_size * field.board_size_val() + 5, board_border_color, board_color);
			int field_color;
			for (int i = 0;i < field.board_size_val();i++)
			{
				for (int j = 0;j < field.board_size_val();j++)
				{
					if (field.tile_val(j,i) > 0)
					{
						if (field.tile_val(j,i) <= 2048)
						{
							field_color = (int)log2(field.tile_val(j,i));
						}
						else
						{
							field_color = sizeof(numbers_color) / sizeof(numbers_color[0]) - 1;
						}

						Window::draw_rectangle(x + field_size * j, y + field_size * i, field_size - 5, field_size - 5, Window::numbers_color[field_color], Window::numbers_color[field_color]);
						sprintf(text, "%lu", field.tile_val(j, i));
						if (field.tile_val(j,i) < 8)
						{
							Window::draw_string(x + field_size * j + (field_size - 5 - strlen(text) * 8) / 2, y + field_size * i + (field_size - 5 - 8) / 2, text, BLACK_TEXT);
						}
						else
						{
							Window::draw_string(x + field_size * j + (field_size - 5 - strlen(text) * 8) / 2, y + field_size * i + (field_size - 5 - 8) / 2, text, WHITE_TEXT);
						}
					}
					else
					{
						Window::draw_rectangle(x + field_size * j, y + field_size * i, field_size - 5, field_size - 5, Window::numbers_color[0], Window::numbers_color[0]);
					}
				}
			}

			if (field.game_ended_val() == true && save_once == true) 
			{
				int quit_save_rank = 0;
				sprintf(text, "Do you want to save your score (ENTER/ESC) ?");
				draw_string(x, y + field.board_size_val() * field_size + 5 + 10, text, WHITE_TEXT);

				SDL_UpdateTexture(Window::scrtex, NULL, Window::screen->pixels, Window::screen->pitch);
				SDL_RenderCopy(Window::renderer, Window::scrtex, NULL, NULL);
				SDL_RenderPresent(Window::renderer);

				while (!quit_save_rank)
				{
					while (SDL_PollEvent(&event))
					{
						switch (event.type)
						{
						case SDL_KEYDOWN:
							if (event.key.keysym.sym == SDLK_RETURN)
							{
								ranking.add_after(field.board_size_val(), field.get_score(), field.game_time_val());
								field.get_best_scores(&ranking);
								sort_by(&ranking, sort_by_time, field.board_size_val());
								quit_save_rank = 1;
							}
							else if (event.key.keysym.sym == SDLK_ESCAPE)
							{
								quit_save_rank = 1;
							}
							break;
						case SDL_QUIT:
							quit_save_rank = 1;
							break;
						}
					}
				}
				save_once = false;
			}

			if (field.game_won_val()== true && won_once == true)
			{
				int quit_2 = 0;
				sprintf(text, "You won!");
				draw_string(x, y + field.board_size_val() * field_size + 5 + 10, text, WHITE_TEXT);

				sprintf(text, "Do you want to continue the game (ENTER/ESC) ?");
				draw_string(x, y + field.board_size_val() * field_size + 5 + 10 * 2, text, WHITE_TEXT);

				SDL_UpdateTexture(Window::scrtex, NULL, Window::screen->pixels, Window::screen->pitch);
				SDL_RenderCopy(Window::renderer, Window::scrtex, NULL, NULL);
				SDL_RenderPresent(Window::renderer);

				while (!quit_2)
				{
					while (SDL_PollEvent(&event))
					{
						switch (event.type)
						{
						case SDL_KEYDOWN:
							if (event.key.keysym.sym == SDLK_RETURN)
							{
								quit_2 = 1;
								t1 = SDL_GetTicks();
								//field.game_won_val(false);
							}
							else if (event.key.keysym.sym == SDLK_ESCAPE)
							{
								field.end_game();
								quit_2 = 1;
							}
							break;
						case SDL_QUIT:
							quit_2 = 1;
							break;
						}
					}
				}
				won_once = false;
			}
		}

		SDL_UpdateTexture(Window::scrtex, NULL, Window::screen->pixels, Window::screen->pitch);
		//		SDL_RenderClear(renderer);
		SDL_RenderCopy(Window::renderer, Window::scrtex, NULL, NULL);
		SDL_RenderPresent(Window::renderer);

		// handling of events (if there were any)
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:

				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					quit = 1;
				}
				else if (event.key.keysym.sym == SDLK_u)
				{
					field.undo();
				}
				else if (event.key.keysym.sym == SDLK_r)
				{

					if (visible_ranking_r == true)
					{
						visible_ranking_r = false;
					}
					else
					{
						visible_ranking_r = true;
					}

					visible_ranking_t = false;//disable other rankings
					visible_ranking_p = false;//disable other rankings

				}
				else if (event.key.keysym.sym == SDLK_t)
				{
					sort_by_time = true;
					if (visible_ranking_t == true)
					{
						visible_ranking_t = false;
					}
					else
					{
						visible_ranking_t = true;
						sort_by(&ranking, sort_by_time, field.board_size_val());
					}


					visible_ranking_r = false;//disable other rankings
					visible_ranking_p = false;//disable other rankings

				}
				else if (event.key.keysym.sym == SDLK_p)
				{
					sort_by_time = false;
					if (visible_ranking_p == true)
					{
						visible_ranking_p = false;
					}
					else
					{
						visible_ranking_p = true;
						sort_by(&ranking, sort_by_time, field.board_size_val());
					}

					visible_ranking_r = false;//disable other rankings
					visible_ranking_t = false;//disable other rankings
				}
				else if (event.key.keysym.sym == SDLK_n)
				{
					choose_board_size(&user_board_size, &event);
					field.new_game(user_board_size);
					worldTime = 0;
					save_once = true;
					won_once = true;
					first_move = true;
					visible_ranking_r = false;
					visible_ranking_t = false;
					visible_ranking_p = false;
				}
				else if (event.key.keysym.sym == SDLK_s)
				{
					sprintf(text, "%.0lf", worldTime);
					field.save_game(text);
					field.update_message("Game has been saved");
				}
				else if (event.key.keysym.sym == SDLK_l)
				{
					choose_save(&field, &event);
					worldTime=field.game_time_val();
					first_move = true;
				}
				else if ((field.game_ended_val() == false) && (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT))
				{
					field.arrow_pressed(event.key.keysym.sym);
					first_move = false;
				}
				break;
			case SDL_QUIT:
				quit = 1;
				break;
			};
		};
		frames++;
		if (quit == 1)
		{
			while (ranking.next != NULL)
			{
				ranking.remove_after();
			}
		}
	};
	//*************************************************************************************************************

	while (ranking.next != NULL)
	{
		
	}


	// freeing all surfaces
	SDL_FreeSurface(Window::charset[WHITE_TEXT]);
	SDL_FreeSurface(Window::charset[BLACK_TEXT]);
	SDL_FreeSurface(Window::screen);
	SDL_DestroyTexture(Window::scrtex);
	SDL_DestroyRenderer(Window::renderer);
	SDL_DestroyWindow(Window::window);

	SDL_Quit();
}


void Window::draw_string(int x, int y, const char *text, int color) //draw a string
{
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(Window::charset[color], &s, Window::screen, &d);
		x += 8;
		text++;
	}
}

void Window::draw_pixel(int x, int y, Uint32 color)// draw a single pixel
{
	int bpp = screen->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;
	*(Uint32 *)p = color;
};

void Window::draw_line(int x, int y, int l, int dx, int dy, Uint32 color)// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
{
	for (int i = 0; i < l; i++) {
		draw_pixel(x, y, color);
		x += dx;
		y += dy;
	};
};

void Window::draw_rectangle(int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor)// draw a rectangle of size l by k
{
	int i;
	draw_line(x, y, k, 0, 1, outlineColor);
	draw_line(x + l - 1, y, k, 0, 1, outlineColor);
	draw_line(x, y, l, 1, 0, outlineColor);
	draw_line(x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		draw_line(x + 1, i, l - 2, 1, 0, fillColor);
};

void Window::draw_empty_board(int field_size, int size)
{
	if (Window::screen->w >= size * field_size + 5 && Window::screen->h >= size * field_size + 5)
	{
		int x = Window::screen->w / 2 - size * field_size / 2;
		int y = Window::screen->h / 2 - size * field_size / 2;
		draw_rectangle(x - 5, y - 5, field_size * size + 5, field_size * size + 5, board_border_color, board_color);

		for (int i = 0;i < size;i++)
		{
			for (int j = 0;j < size;j++)
			{
				draw_rectangle(x + field_size * j, y + field_size * i, field_size - 5, field_size - 5, numbers_color[0], numbers_color[0]);
			}
		}
	}
}

void Window::choose_board_size(int *user_board_size, SDL_Event *event)
{
	int quit = 0;
	char text[128];

	while (!quit)
	{
		SDL_FillRect(Window::screen, NULL, black);
		sprintf(text, "Choose board size or press enter to load defalut");
		draw_string(Window::screen->w / 2 - strlen(text) * 8 / 2, 70, text, WHITE_TEXT);
		sprintf(text, "Board size: %dx%d", *user_board_size, *user_board_size);
		draw_string(Window::screen->w / 2 - strlen(text) * 8 / 2, 82, text, WHITE_TEXT);

		int field_size = 55;
		draw_empty_board(field_size, (*user_board_size));


		while (SDL_PollEvent(event))
		{
			switch (event->type)
			{
			case SDL_KEYDOWN:
				if (event->key.keysym.sym == SDLK_RETURN)
				{
					quit = 1;
				}
				else if (event->key.keysym.sym >= SDLK_2 && event->key.keysym.sym <= SDLK_9)
				{
					(*user_board_size) = event->key.keysym.sym - SDLK_0;
				}
				else if (event->key.keysym.sym == SDLK_RIGHT && (*user_board_size) <= 8)
				{
					(*user_board_size)++;
				}
				else if (event->key.keysym.sym == SDLK_LEFT && (*user_board_size) >= 3)
				{
					(*user_board_size)--;
				}
				break;
			case SDL_QUIT:
				quit = 1;
				break;
			}
		}
		SDL_UpdateTexture(Window::scrtex, NULL, Window::screen->pixels, Window::screen->pitch);
		SDL_RenderCopy(Window::renderer, Window::scrtex, NULL, NULL);
		SDL_RenderPresent(Window::renderer);
	}
}
void Window::choose_save(Field *field, SDL_Event *event)
{
	int quit = 0;
	char text[128];
	char chosen[128];
	int n = 0;
	List list;
	list.init();
	int list_size=get_existing_saves(&list);
	while (!quit)
	{
		SDL_FillRect(Window::screen, NULL, black);
		sprintf(text, "Choose game to load (ENTER/ESC)");
		draw_string(Window::screen->w / 2 - strlen(text) * 8 / 2, 70, text, WHITE_TEXT);
		
		//***
		int x = 100, y = 100;
		//Ranking = Ranking->next;
		int q = 1;
		
		char text[128];
		show_saves(&list);
		strcpy(chosen,get_nth_val(&list, n));
		sprintf(text, "Chosen save nr %d: %s", n+1, chosen);
		draw_string(Window::screen->w / 2 - strlen(text) * 8 / 2, 82, text, WHITE_TEXT);
		
		while (SDL_PollEvent(event))
		{
			switch (event->type)
			{
			case SDL_KEYDOWN:
				if (event->key.keysym.sym == SDLK_RETURN)
				{
					field->load_game(chosen);
					quit = 1;
				}
				else if (event->key.keysym.sym == SDLK_ESCAPE)
				{
					quit = 1;
				}
				else if (event->key.keysym.sym == SDLK_DOWN && n < list_size-1)
				{
					n++;
				}
				else if (event->key.keysym.sym == SDLK_UP && n>0)
				{
					n--;
				}
				break;
			case SDL_QUIT:
				quit = 1;
				break;
			}
		}
		SDL_UpdateTexture(Window::scrtex, NULL, Window::screen->pixels, Window::screen->pitch);
		SDL_RenderCopy(Window::renderer, Window::scrtex, NULL, NULL);
		SDL_RenderPresent(Window::renderer);
	}
}
void Window::show_saves(List *saves)
{
	int x = 100, y = 100;
	int q = 1;
	char text[128];
	saves = saves->next;
	while (saves != NULL)
	{
		if (2 * y + 12 * q < screen->h)
		{
			//if (strlen(saves->value) > 0)
			{
				sprintf(text, "%2d. name: %s", q, saves->value);
				Window::draw_string(x, y + 12 * q, text, WHITE_TEXT);
				q++;
			}
		}
		saves = saves->next;
	}
	if (q == 1)
	{
		sprintf(text, "no records");
		Window::draw_string(x, y + 12 * q, text, WHITE_TEXT);
	}
}
void Window::draw_legend(int x,int y,int board_size)
{
	int line = 0;
	char text[128];
	
	draw_rectangle(4, 4, SCREEN_WIDTH - 8, 36, numbers_color[0], numbers_color[5]);
	sprintf(text, "Hubert Solecki s175823");
	draw_string(Window::screen->w / 2 - strlen(text) * 8 / 2, 10, text, WHITE_TEXT);
	sprintf(text, "2048");
	draw_string(Window::screen->w / 2 - strlen(text) * 8 / 2, 26, text, WHITE_TEXT);

	sprintf(text, "\32,\030,\031,\33 - moving tiles");
	draw_string(x , y + 12 * line++, text, WHITE_TEXT);
	sprintf(text, "ESC - quit");
	draw_string(x , y + 12 * line++, text, WHITE_TEXT);
	sprintf(text, "n - new game");
	draw_string(x , y + 12 * line++, text, WHITE_TEXT);
	sprintf(text, "s - save game");
	draw_string(x, y + 12 * line++, text, WHITE_TEXT);
	sprintf(text, "l - load game");
	draw_string(x, y + 12 * line++, text, WHITE_TEXT);
	sprintf(text, "u - previous move");
	draw_string(x , y + 12 * line++, text, WHITE_TEXT);
	sprintf(text, "r - ranking (all best scores)");
	draw_string(x , y + 12 * line++, text, WHITE_TEXT);
	sprintf(text, "t - %dx%d ranking sorted by time",board_size, board_size);
	draw_string(x , y + 12 * line++, text, WHITE_TEXT);
	sprintf(text, "p - %dx%d ranking sorted by points", board_size, board_size);
	draw_string(x , y + 12 * line++, text, WHITE_TEXT);
}

void Window::show_ranking(Ranking *ranking,int board_size)
{
	int ranking_x = 100, ranking_y = 100;
	//Ranking = Ranking->next;
	int q = 1;
	char text[128];
	sprintf(text, "Board %dx%d", board_size, board_size);
	Window::draw_string(ranking_x, ranking_y, text, WHITE_TEXT);
	while (ranking != NULL)
	{
		if (ranking->board_size == board_size && 2 * ranking_y + 12 * q < screen->h)
		{
			sprintf(text, "%2d. score:%6lu  time: %02d:%04.1lf", q, ranking->score, (int)ranking->time / 60, fmod(ranking->time,60.0));
			Window::draw_string(ranking_x, ranking_y + 12 * q, text, WHITE_TEXT);
			q++;
		}
		ranking = ranking->next;
	}
	if (q == 1)
	{
		sprintf(text, "no records");
		Window::draw_string(ranking_x, ranking_y + 12 * q, text, WHITE_TEXT);
	}
}


int Window::window_width()
{
	return Window::screen->w;
}
int Window::window_height()
{
	return Window::screen->h;
}
