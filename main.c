#include <math.h>
#include <SDL2/SDL.h>
#include "primitives.h"

#define SCREEN_WIDTH		700
#define SCREEN_HEIGHT	700

typedef struct game_window game_window;

struct game_window
{
	// window stuff
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event event;

	// bools
	char running;
	char pause;
	char mouse_down;

	// stuff
	int mouse_pos_x;
	int mouse_pos_y;

	// settings
	int width;
	int height;

	int sides;
};

int init_window(game_window *win, char *title, int width, int height)
{
	int wflags = 0; // window flags
	int rflags = 0; // render flags

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		return -1;

	win->window = SDL_CreateWindow(title, 0, 0, width, height, wflags);

	if (!win->window)
	{
		SDL_Quit();
		return -2;
	}

	win->renderer = SDL_CreateRenderer(win->window, -1, rflags);

	if (!win->renderer)
	{
		SDL_DestroyWindow(win->window);
		SDL_Quit();
		return -3;
	}

	// init data
	SDL_GetMouseState(&win->mouse_pos_x, &win->mouse_pos_y);
	win->width = width;
	win->height = height;
	win->mouse_down = 0;
	win->running = 1;
	win->pause = 0;
	win->sides = 3;

	return 0;
}

void destroy_window(game_window *win)
{
	// clean up
	SDL_DestroyWindow(win->window);
	SDL_DestroyRenderer(win->renderer);
	SDL_Quit();
}

void handle_events(game_window *win)
{
	while (SDL_PollEvent(&(win->event)))
	{
		switch (win->event.type)
		{
			case SDL_QUIT:
				win->running = 0;
				break;

			case SDL_MOUSEMOTION:
				SDL_GetMouseState(&win->mouse_pos_x, &win->mouse_pos_y);
				if (win->pause == 1 && win->mouse_down == 1 && win->event.button.button == SDL_BUTTON_LEFT)
				{
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (win->pause == 1 && win->mouse_down == 0 && win->event.button.button == SDL_BUTTON_LEFT)
				{
					win->mouse_down = 1;
				}
				break;

			case SDL_MOUSEBUTTONUP:
				if (win->mouse_down == 1) win->mouse_down = 0;
				break;

			case SDL_KEYDOWN:
				switch (win->event.key.keysym.sym)
				{
					case SDLK_SPACE:
						win->sides = 3;
						//win->sides++;
						//win->pause = win->pause == 1 ? 0 : 1;ddko
						break;
					case SDLK_w:
						win->sides++;
						break;

					case SDLK_s:
						if (!(win->sides == 3))
							win->sides--;
						break;
				};
				break;
		};
	}
}

void render(game_window *win)
{
	SDL_RenderClear(win->renderer);

	SDL_SetRenderDrawColor(win->renderer, 0, 0, 0, 255);

	struct polygon *p;

	//p = create_reg_polygon(win->sides, 100, 100, 100, 0, 1, 1);
	//draw_polygon(win->renderer, p);
	//free_polygon(p);

	p = create_reg_polygon(win->sides, 350 / 3, 350 / 3, 100, 3 * 3.14159 / 2, 1, 1);
	SDL_RenderSetScale(win->renderer, 3, 3);
	draw_polygon(win->renderer, p);
	draw_polygon_filled(win->renderer, p);
	free_polygon(p);
	SDL_RenderSetScale(win->renderer, 1, 1);

	//SDL_Point points[6] = {
	//	{ 20, 0 },
	//	{ -10, -10 },
	//	{ -20, 50 },
	//	{ -2, 1 },
	//	{ 23, 42 },
	//	{ -10, 10 } };

	//p = create_polygon(points, 6, 350, 350, 0, 2, 2);
	//draw_polygon(win->renderer, p);
	//draw_polygon_filled(win->renderer, p);

	//p->x = 100;
	//p->y = 100;
	//p->scale_x = 3;
	//p->scale_y = 3;
	//polygon_rebuild(p);
	//draw_polygon(win->renderer, p);
	//free_polygon(p);

	//p = create_reg_polygon(3, 200, 350, 10, 3.14159 / 2, 5, 5);
	//draw_polygon(win->renderer, p);
	//draw_polygon_filled(win->renderer, p);
	//free_polygon(p);

	SDL_SetRenderDrawColor(win->renderer, 255, 255, 255, 255);
	SDL_RenderPresent(win->renderer);
}

int main()
{
	game_window win;
	int code = init_window(&win, "primitives", SCREEN_WIDTH, SCREEN_HEIGHT);
	if (code != 0) return -1;

	while (win.running)
	{
		handle_events(&win);
		if (!win.pause)
			render(&win);
		SDL_Delay(10);
	}

	destroy_window(&win);

	return 0;
}
