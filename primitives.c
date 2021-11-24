#include <math.h>
#include "primitives.h"

#include <stdio.h>

#define PI 3.1415926535897932384626433832795

int draw_rcpolygon(SDL_Renderer *renderer, const struct reg_convex_polygon *p)
{
	// error checking
	if (renderer == NULL)
		return -1;

	if (p == NULL)
		return -1;

	if (p->n_points < 3)
		return -1;

	double incr_angle;
	double start_angle;

	// get angle for point ploting
	incr_angle = (p->n_points - 2.0f) * PI;	// sum of angles for a polygon
	incr_angle = incr_angle	/ p->n_points;	// get the angle for each point
	incr_angle = incr_angle / 2.0f;			// get angle from a corner to center
	incr_angle = PI - 2.0f * incr_angle;	// get the final increment angle

	start_angle = (p->n_points % 2 == 1) ? (3.0f * PI / 2.0f) : (incr_angle / 2.0f);

	int x[2], y[2];
	x[1] = round(cos(start_angle) * p->radius + p->x);
	y[1] = round(sin(start_angle) * p->radius + p->y);

	// draw lines for each point
	for (int i = 0; i < p->n_points; i++)
	{
		x[i & 1] = round(cos((i + 1) * incr_angle + start_angle) * p->radius + p->x);
		y[i & 1] = round(sin((i + 1) * incr_angle + start_angle) * p->radius + p->y);
		SDL_RenderDrawLine(renderer, x[0], y[0], x[1], y[1]);
	}
	//// temp storage for points. note: need one extra for last point to close the polygon
	//SDL_Point *points = (SDL_Point *)malloc(sizeof(SDL_Point) * p->n_points + 1);

	//// get all points
	//for (int i = 0; i <= p->n_points; i++)
	//{
	//	points[i].x = cos(i * incr_angle + start_angle) * p->radius + p->x;
	//	points[i].y = sin(i * incr_angle + start_angle) * p->radius + p->y;
	//}

	//SDL_RenderDrawLines(renderer, points, p->n_points + 1);
	//free(points);

	return 0;
}

int draw_polygon(SDL_Renderer *renderer, const struct polygon *p)
{
	if (renderer == NULL)
		return -1;

	if (p == NULL)
		return -1;

	SDL_RenderDrawLines(renderer, p->t_points, p->n_points + 1);

	return 0;
}

int build_rcpolygon(struct polygon *p, int x, int y, int n_points, double radius, double angle)
{
	// error checking
	if (p == NULL)
		return -1;

	if (n_points < 3)
		return -1;

	// set stuff
	p->x = x;
	p->y = y;
	p->angle = angle;
	p->n_points = n_points;
	p->points = (SDL_Point *)malloc(sizeof(SDL_Point) * n_points + 1);
	p->t_points = (SDL_Point *)malloc(sizeof(SDL_Point) * n_points + 1);

	/* 
	 * get angle for point ploting
	 * 1) sum of angles for a polygon
	 * 2) get the angle for each point
	 * 3) get angle from a corner to center
	 * 4) get the final increment angle
	 */
	double incr_angle;
	incr_angle = (n_points - 2.0f) * PI;
	incr_angle = incr_angle	/ n_points;
	incr_angle = incr_angle / 2.0f;
	incr_angle = PI - 2.0f * incr_angle;

	for (int i = 0; i <= p->n_points; i++)
	{
		p->points[i].x = round(cos(i * incr_angle) * radius);
		p->points[i].y = round(sin(i * incr_angle) * radius);

		p->t_points[i].x = round(cos(i * incr_angle + angle) * radius + x);
		p->t_points[i].y = round(sin(i * incr_angle + angle) * radius + y);
	}

	return 0;
}

void free_polygon(struct polygon *p)
{
	free(p->points);
	free(p->t_points);
}
