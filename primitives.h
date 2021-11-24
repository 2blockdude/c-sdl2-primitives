/*
 * Description:
 * Creates simple regular convex polygons and ellipses
 */

#ifndef MY_COOL_PRIMITIVES
#define MY_COOL_PRIMITIVES

#include <SDL2/SDL.h>

struct reg_convex_polygon
{
	int x, y;
	int n_sides;
	double radius;
};

struct polygon
{
	SDL_Point *points;
};

struct ellipse
{
	int x, y;
	int radius_x;
	int radius_y;
};

int draw_rcpolygon			(SDL_Renderer *renderer, const struct reg_convex_polygon *p);
int draw_rcpolygon_filled	(SDL_Renderer *renderer, const struct reg_convex_polygon *p);

int draw_polygon				(SDL_Renderer *renderer, const struct polygon *p);
int draw_polygon_filled		(SDL_Renderer *renderer, const struct polygon *p);

int draw_ellipse				(SDL_Renderer *renderer, const struct ellipse *e);
int draw_ellipse_filled		(SDL_Renderer *renderer, const struct ellipse *e);

#endif
