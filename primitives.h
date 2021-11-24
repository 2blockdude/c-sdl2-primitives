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
	int n_points;
	double radius;
};

struct polygon
{
	int x, y;
	double angle;
	SDL_Point *points;
	SDL_Point *t_points;
	int n_points;
};

struct ellipse
{
	int x, y;
	double angle;
	int radius_x;
	int radius_y;
};

// regular convex polygons. not sure if any good.
int draw_rcpolygon			(SDL_Renderer *renderer, const struct reg_convex_polygon *p);
int draw_rcpolygon_filled	(SDL_Renderer *renderer, const struct reg_convex_polygon *p);

// polygons
int draw_polygon				(SDL_Renderer *renderer, const struct polygon *p);
int draw_polygon_filled		(SDL_Renderer *renderer, const struct polygon *p);

// ellipse
int draw_ellipse				(SDL_Renderer *renderer, const struct ellipse *e);
int draw_ellipse_filled		(SDL_Renderer *renderer, const struct ellipse *e);

// functions to build polygons
int build_rcpolygon			(struct polygon *p, int x, int y, int n_points, double radius, double angle);
int build_random_polygon	(struct polygon *p, int x, int y, int n_points, double max_radius, double min_radius, double angle);

void free_polygon				(struct polygon *p);

#endif
