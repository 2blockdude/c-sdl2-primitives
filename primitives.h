/*
 * Description:
 * Creates simple regular convex polygons and ellipses
 */

#ifndef MY_COOL_PRIMITIVES
#define MY_COOL_PRIMITIVES

#include <SDL2/SDL.h>

typedef struct polygon polygon;
typedef struct ellipse ellipse;

struct polygon
{
   float x, y;
   float angle;

   int nsides;

   float *vectors;
   float *points;

   struct { float x, y; } scale;
};

struct ellipse
{
   float x, y;
   float angle;

   struct { float x, y; } scale;
   struct { float x, y; } radius;
};

// float polygons
int draw_polygon								(SDL_Renderer *renderer, const struct polygon *p);
int draw_polygon_filled					   (SDL_Renderer *renderer, const struct polygon *p);

// build float polygons
struct polygon *create_polygon			(float *input_vectors, int nsides, float x, float y, float angle);
struct polygon *create_reg_polygon	   (int nsides, float x, float y, float radius, float angle);
struct polygon *create_rand_polygon	   (int nsides, float x, float y, float max_radius, float min_radius, float angle_offset, float angle);

// transform float polygon
int polygon_translate						(struct polygon *p, float x, float y);
int polygon_set_angle						(struct polygon *p, float angle);
int polygon_set_scale						(struct polygon *p, float scale_x, float scale_y);
int polygon_rebuild							(struct polygon *p);

// free float polygon
void free_polygon							   (struct polygon *p);

#endif
