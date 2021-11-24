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

	SDL_RenderDrawLines(renderer, p->points_t, p->npoints);

	// close polygon
	SDL_RenderDrawLine(renderer,
			p->points_t[p->npoints - 1].x, p->points_t[p->npoints - 1].y,
			p->points_t[0].x, p->points_t[0].y);

	return 0;
}

int build_polygon(struct polygon *p, SDL_Point *points, int npoints, int x, int y, double angle, double scale)
{
	if (p == NULL)
		return -1;

	if (points == NULL)
		return -1;

	if (npoints < 3)
		return -1;

	// copy stuff
	p->x = x;
	p->y = y;
	p->angle = angle;
	p->scale = scale;
	p->npoints = npoints;
	p->points = (SDL_Point *)malloc(sizeof(SDL_Point) * npoints);
	p->points_t = (SDL_Point *)malloc(sizeof(SDL_Point) * npoints);

	for (int i = 0; i < p->npoints; i++)
	{
		// set points
		p->points[i].x = points[i].x;
		p->points[i].y = points[i].y;

		// set points transformed
		p->points_t[i].x = p->x + (p->scale * p->points[i].x * round(cos(p->angle))) - (p->scale * p->points[i].y * round(sin(p->angle)));
		p->points_t[i].y = p->y + (p->scale * p->points[i].x * round(sin(p->angle))) + (p->scale * p->points[i].y * round(cos(p->angle)));
	}

	return 0;
}

int build_rcpolygon(struct polygon *p, int npoints, int x, int y, double radius, double angle, double scale)
{
	// error checking
	if (p == NULL)
		return -1;

	if (npoints < 3)
		return -1;

	// copy stuff
	p->x = x;
	p->y = y;
	p->angle = angle;
	p->scale = scale;
	p->npoints = npoints;
	p->points = (SDL_Point *)malloc(sizeof(SDL_Point) * npoints);
	p->points_t = (SDL_Point *)malloc(sizeof(SDL_Point) * npoints);

	/* 
	 * get angle for point ploting
	 * 1) sum of angles for a polygon
	 * 2) get the angle for each point
	 * 3) get angle from a corner to center
	 * 4) get the final increment angle
	 */
	double incr_angle;
	incr_angle = (npoints - 2.0f) * PI;
	incr_angle = incr_angle	/ npoints;
	incr_angle = incr_angle / 2.0f;
	incr_angle = PI - 2.0f * incr_angle;

	for (int i = 0; i < p->npoints; i++)
	{
		// set points
		p->points[i].x = round(cos(i * incr_angle) * radius);
		p->points[i].y = round(sin(i * incr_angle) * radius);

		// set points transformed
		p->points_t[i].x = round(cos(i * incr_angle + angle) * (radius * scale) + x);
		p->points_t[i].y = round(sin(i * incr_angle + angle) * (radius * scale) + y);
	}

	return 0;
}

int set_polygon_pos(struct polygon *p, int x, int y)
{
	if (p == NULL)
		return -1;

	for (int i = 0; i < p->npoints; i++)
	{
		p->points_t[i].x = p->points_t[i].x - p->x + x;
		p->points_t[i].y = p->points_t[i].y - p->y + y;
	}
	
	// store new position
	p->x = x;
	p->y = y;

	return 0;
}

int set_polygon_rot(struct polygon *p, double angle)
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

	for (int i = 0; i < p->npoints; i++)
	{
		p->points_t[i].x = p->x + (p->scale * p->points[i].x * round(cos(p->angle))) - (p->scale * p->points[i].y * round(sin(p->angle)));
		p->points_t[i].y = p->y + (p->scale * p->points[i].x * round(sin(p->angle))) + (p->scale * p->points[i].y * round(cos(p->angle)));
	}

	return 0;
}

int set_polygon_scale(struct polygon *p, double scale)
{
	if (p == NULL)
		return -1;

	p->scale = scale;

	for (int i = 0; i < p->npoints; i++)
	{
		p->points_t[i].x = p->x + (p->scale * p->points[i].x * round(cos(p->angle))) - (p->scale * p->points[i].y * round(sin(p->angle)));
		p->points_t[i].y = p->y + (p->scale * p->points[i].x * round(sin(p->angle))) + (p->scale * p->points[i].y * round(cos(p->angle)));
	}

	return 0;
}

void free_polygon(struct polygon *p)
{
	free(p->points);
	free(p->points_t);
}
