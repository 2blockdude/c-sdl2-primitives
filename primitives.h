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
	int nsides;
	SDL_FPoint scale;
	SDL_Point *vectors;
	SDL_Point *points;
};

struct fpolygon
{
	float x, y;
	float angle;
	int nsides;
	SDL_FPoint scale;
	SDL_FPoint *vectors;
	SDL_FPoint *points;
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

// build polygons
struct polygon *create_polygon			(SDL_Point *vectors, int nsides, int x, int y, float angle);
struct polygon *create_rpolygon			(int nsides, int x, int y, float radius, float angle);
struct polygon *create_polygon_rand		(int nsides, int x, int y, float max_radius, float min_radius, float angle);

// transform polygon
int polygon_translate						(struct polygon *p, int x, int y);
int polygon_set_angle						(struct polygon *p, float angle);
int polygon_set_scale						(struct polygon *p, float scale_x, float scale_y);
int polygon_rebuild							(struct polygon *p);

// free polygon
void free_polygon								(struct polygon *p);


// float polygons
int draw_fpolygon								(SDL_Renderer *renderer, const struct fpolygon *p);
int draw_fpolygon_filled					(SDL_Renderer *renderer, const struct fpolygon *p);

// build float polygons
struct fpolygon *create_fpolygon			(SDL_FPoint *vectors, int nsides, float x, float y, float angle);
struct fpolygon *create_rfpolygon		(int nsides, float x, float y, float radius, float angle);
struct fpolygon *create_fpolygon_rand	(int nsides, float x, float y, float max_radius, float min_radius, float angle);

// transform float polygon
int fpolygon_translate						(struct fpolygon *p, float x, float y);
int fpolygon_set_angle						(struct fpolygon *p, float angle);
int fpolygon_set_scale						(struct fpolygon *p, float scale_x, float scale_y);
int fpolygon_rebuild							(struct fpolygon *p);

// free float polygon
void free_fpolygon							(struct fpolygon *p);

#endif
