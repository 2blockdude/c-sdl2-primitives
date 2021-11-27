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
	float angle;
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
	win->angle = 0;

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
						break;
					case SDLK_UP:
						win->sides++;
						break;

					case SDLK_DOWN:
						if (!(win->sides == 3))
							win->sides--;
						break;

					case SDLK_LEFT:
						win->angle -= 3.14159f / 30.0f;
						break;

					case SDLK_RIGHT:
						win->angle += 3.14159f / 30.0f;
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

	struct fpolygon *p;
	p = create_rfpolygon(win->sides, 350, 350, 100, win->angle);
	draw_fpolygon(win->renderer, p);
	draw_fpolygon_filled(win->renderer, p);
	free_fpolygon(p);

	p = create_rfpolygon(win->sides, 100, 100, 100, win->angle);
	draw_fpolygon(win->renderer, p);
	draw_fpolygon_filled(win->renderer, p);
	free_fpolygon(p);

	p = create_rfpolygon(win->sides, 600, 100, 100, win->angle);
	draw_fpolygon(win->renderer, p);
	draw_fpolygon_filled(win->renderer, p);
	free_fpolygon(p);

	p = create_rfpolygon(win->sides, 600, 600, 100, win->angle);
	draw_fpolygon(win->renderer, p);
	draw_fpolygon_filled(win->renderer, p);
	free_fpolygon(p);

	p = create_rfpolygon(win->sides, 100, 600, 100, win->angle);
	draw_fpolygon(win->renderer, p);
	draw_fpolygon_filled(win->renderer, p);
	free_fpolygon(p);

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
		win.angle += 0.001f;
	}

	destroy_window(&win);

	return 0;
}
