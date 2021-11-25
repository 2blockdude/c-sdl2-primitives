/*
 * Description:
 * Creates simple regular convex polygons and ellipses
 */

#ifndef MY_COOL_PRIMITIVES
#define MY_COOL_PRIMITIVES

#include <SDL2/SDL.h>

struct polygon
{
	int x, y;
	double angle;
	double scale_x;
	double scale_y;
	int nsides;
	SDL_Point *vectors;
	SDL_Point *points;
};

struct ellipse
{
	int x, y;
	double angle;
	double scale_x;
	double scale_y;
	int radius_x;
	int radius_y;
};

// polygons
int draw_polygon				(SDL_Renderer *renderer, const struct polygon *p);
int draw_polygon_filled		(SDL_Renderer *renderer, const struct polygon *p);

// ellipse
int draw_ellipse				(SDL_Renderer *renderer, const struct ellipse *e);
int draw_ellipse_filled		(SDL_Renderer *renderer, const struct ellipse *e);

// build polygons
int build_polygon				(struct polygon *p, SDL_Point *vectors, int nsides, int x, int y, double angle, double scale_x, double scale_y);
int build_reg_polygon		(struct polygon *p, int nsides, int x, int y, double radius, double angle, double scale_x, double scale_y);
int build_random_polygon	(struct polygon *p, int nsides, int x, int y, double max_radius, double min_radius, double angle, double scale_x, double scale_y);

// transform polygon
int polygon_translate		(struct polygon *p, int x, int y);
int polygon_angle				(struct polygon *p, double angle);
int polygon_scale				(struct polygon *p, double scale_x, double scale_y);
int polygon_rebuild			(struct polygon *p);

// free polygon
void free_polygon				(struct polygon *p);

#endif
