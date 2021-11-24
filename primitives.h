/*
 * Description:
 * Creates simple regular convex polygons and ellipses
 */

#ifndef MY_COOL_PRIMITIVES
#define MY_COOL_PRIMITIVES

#include <SDL2/SDL.h>

struct regular_polygon
{
	int x, y;
	int n_sides;
	double radius;
};

struct ellipse
{
	int x, y;
	int radius_x;
	int radius_y;
};

int draw_rpolygon				(SDL_Renderer *renderer, const struct regular_polygon *p);
int draw_rpolygon_filled	(SDL_Renderer *renderer, const struct regular_polygon *p);

int draw_ellipse				(SDL_Renderer *renderer, const struct ellipse *e);
int draw_ellipse_filled		(SDL_Renderer *renderer, const struct ellipse *e);

#endif
