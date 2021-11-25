#include <math.h>
#include "primitives.h"

#include <stdio.h>

#define PI 3.1415926535897932384626433832795

int draw_polygon(SDL_Renderer *renderer, const struct polygon *p)
{
	if (renderer == NULL)
		return -1;

	if (p == NULL)
		return -1;

	SDL_RenderDrawLines(renderer, p->points, p->nsides);

	// close polygon
	SDL_RenderDrawLine(renderer,
			p->points[p->nsides - 1].x, p->points[p->nsides - 1].y,
			p->points[0].x, p->points[0].y);

	return 0;
}

int draw_polygon_filled(SDL_Renderer *renderer, const struct polygon *p)
{
	if (renderer == NULL)
		return -1;

	if (p == NULL)
		return -1;
	return 0;
}

struct polygon *create_polygon(SDL_Point *vectors, int nsides, int x, int y, double angle, double scale_x, double scale_y)
{
	if (vectors == NULL)
		return NULL;

	if (nsides < 3)
		return NULL;

	struct polygon *p = (struct polygon *)malloc(sizeof(struct polygon));

	// copy stuff
	p->x = x;
	p->y = y;
	p->angle = angle;
	p->scale_x = scale_x;
	p->scale_y = scale_y;
	p->nsides = nsides;
	p->vectors = (SDL_Point *)malloc(sizeof(SDL_Point) * nsides);
	p->points = (SDL_Point *)malloc(sizeof(SDL_Point) * nsides);

	for (int i = 0; i < p->nsides; i++)
	{
		// set vectors
		p->vectors[i].x = vectors[i].x;
		p->vectors[i].y = vectors[i].y;

		// set points with angle and scale applied
		p->points[i].x = p->x + (p->scale_x * p->vectors[i].x * round(cos(p->angle))) - (p->scale_x * p->vectors[i].y * round(sin(p->angle)));
		p->points[i].y = p->y + (p->scale_y * p->vectors[i].x * round(sin(p->angle))) + (p->scale_y * p->vectors[i].y * round(cos(p->angle)));
	}

	return p;
}

struct polygon *create_reg_polygon(int nsides, int x, int y, double radius, double angle, double scale_x, double scale_y)
{
	if (nsides < 3)
		return NULL;

	struct polygon *p = (struct polygon *)malloc(sizeof(struct polygon));

	// copy stuff
	p->x = x;
	p->y = y;
	p->angle = angle;
	p->scale_x = scale_x;
	p->scale_y = scale_y;
	p->nsides = nsides;
	p->vectors = (SDL_Point *)malloc(sizeof(SDL_Point) * nsides);
	p->points = (SDL_Point *)malloc(sizeof(SDL_Point) * nsides);

	// i am retarded
	double incr_angle = 2.0f * PI / nsides;

	for (int i = 0; i < p->nsides; i++)
	{
		// set vectors
		p->vectors[i].x = round(cos(i * incr_angle) * radius);
		p->vectors[i].y = round(sin(i * incr_angle) * radius);

		// set vectors transformed
		p->points[i].x = p->x + (p->scale_x * p->vectors[i].x * round(cos(p->angle))) - (p->scale_x * p->vectors[i].y * round(sin(p->angle)));
		p->points[i].y = p->y + (p->scale_y * p->vectors[i].x * round(sin(p->angle))) + (p->scale_y * p->vectors[i].y * round(cos(p->angle)));
	}

	return p;
}

int polygon_translate(struct polygon *p, int x, int y)
{
	if (p == NULL)
		return -1;

	for (int i = 0; i < p->nsides; i++)
	{
		p->points[i].x = p->points[i].x - p->x + x;
		p->points[i].y = p->points[i].y - p->y + y;
	}
	
	// store new position
	p->x = x;
	p->y = y;

	return 0;
}

int polygon_angle(struct polygon *p, double angle)
{
	if (p == NULL)
		return -1;

	p->angle = angle;

	/*
	 * 2d rotation matrix
	 * 
	 * R = {cos(theta), -sin(theta)}
	 *		 {sin(theta),  cos(theta)}
	 */

	for (int i = 0; i < p->nsides; i++)
	{
		p->points[i].x = p->x + (p->scale_x * p->vectors[i].x * round(cos(p->angle))) - (p->scale_x * p->vectors[i].y * round(sin(p->angle)));
		p->points[i].y = p->y + (p->scale_y * p->vectors[i].x * round(sin(p->angle))) + (p->scale_y * p->vectors[i].y * round(cos(p->angle)));
	}

	return 0;
}

int polygon_scale(struct polygon *p, double scale_x, double scale_y)
{
	if (p == NULL)
		return -1;

	/*
	 * 2d scaling matrix
	 *
	 * S = { Sx, 0 }
	 *		 { 0, Sy }
	 */

	for (int i = 0; i < p->nsides; i++)
	{
		p->points[i].x = (p->points[i].x - p->x) / p->scale_x * scale_x + p->x;
		p->points[i].y = (p->points[i].y - p->y) / p->scale_y * scale_y + p->y;
	}
	
	p->scale_x = scale_x;
	p->scale_y = scale_y;

	return 0;
}

int polygon_rebuild(struct polygon *p)
{
	if (p == NULL)
		return -1;

	for (int i = 0; i < p->nsides; i++)
	{
		p->points[i].x = p->x + (p->scale_x * p->vectors[i].x * round(cos(p->angle))) - (p->scale_x * p->vectors[i].y * round(sin(p->angle)));
		p->points[i].y = p->y + (p->scale_y * p->vectors[i].x * round(sin(p->angle))) + (p->scale_y * p->vectors[i].y * round(cos(p->angle)));
	}

	return 0;
}

void free_polygon(struct polygon *p)
{
	free(p->vectors);
	free(p->points);
	free(p);
}
