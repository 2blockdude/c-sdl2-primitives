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
	int points_c;
	double radius;
};

struct polygon
{
	int x, y;
	double angle;
	double scale;
	int points_c;
	SDL_Point *points;
	SDL_Point *points_t;
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

// build polygons
int build_rcpolygon			(struct polygon *p, int x, int y, int points_c, double radius, double angle, double scale);
int build_random_polygon	(struct polygon *p, int x, int y, int points_c, double max_radius, double min_radius, double angle, double scale);

// transform polygon
int set_polygon_pos			(struct polygon *p, int x, int y);
int set_polygon_angle		(struct polygon *p, double angle);
int set_polygon_scale		(struct polygon *p, double scale);

// free polygon
void free_polygon					(struct polygon *p);

#endif
