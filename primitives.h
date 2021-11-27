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
	float angle;
	float scale_x;
	float scale_y;
	int nsides;
	SDL_Point *vectors;
	SDL_Point *points;
};

struct ellipse
{
	int x, y;
	float angle;
	float scale_x;
	float scale_y;
	int radius_x;
	int radius_y;
};

// polygons
int draw_polygon								(SDL_Renderer *renderer, const struct polygon *p);
int draw_polygon_filled						(SDL_Renderer *renderer, const struct polygon *p);

// ellipse
int draw_ellipse								(SDL_Renderer *renderer, const struct ellipse *e);
int draw_ellipse_filled						(SDL_Renderer *renderer, const struct ellipse *e);

// build polygons
struct polygon *create_polygon			(SDL_Point *vectors, int nsides, int x, int y, float angle, float scale_x, float scale_y);
struct polygon *create_reg_polygon		(int nsides, int x, int y, float radius, float angle, float scale_x, float scale_y);
struct polygon *create_random_polygon	(int nsides, int x, int y, float max_radius, float min_radius, float angle, float scale_x, float scale_y);

// transform polygon
int polygon_translate						(struct polygon *p, int x, int y);
int polygon_set_angle						(struct polygon *p, float angle);
int polygon_set_scale						(struct polygon *p, float scale_x, float scale_y);
int polygon_rebuild							(struct polygon *p);

// free polygon
void free_polygon								(struct polygon *p);

#endif
