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
	double scale;
	int npoints;
	SDL_Point *points;
	SDL_Point *points_t;
};

struct ellipse
{
	int x, y;
	double angle;
	double scale;
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
int build_polygon				(struct polygon *p, SDL_Point *points, int npoints, int x, int y, double angle, double scale);
int build_rcpolygon			(struct polygon *p, int npoints, int x, int y, double radius, double angle, double scale);
int build_random_polygon	(struct polygon *p, int npoints, int x, int y, double max_radius, double min_radius, double angle, double scale);

// transform polygon
int set_polygon_pos			(struct polygon *p, int x, int y);
int set_polygon_rot			(struct polygon *p, double angle);
int set_polygon_scale		(struct polygon *p, double scale);

// free polygon
void free_polygon				(struct polygon *p);

#endif
