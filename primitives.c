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

	if (p->points_c < 3)
		return -1;

	double incr_angle;
	double start_angle;

	// get angle for point ploting
	incr_angle = (p->points_c - 2.0f) * PI;	// sum of angles for a polygon
	incr_angle = incr_angle	/ p->points_c;	// get the angle for each point
	incr_angle = incr_angle / 2.0f;			// get angle from a corner to center
	incr_angle = PI - 2.0f * incr_angle;	// get the final increment angle

	start_angle = (p->points_c % 2 == 1) ? (3.0f * PI / 2.0f) : (incr_angle / 2.0f);

	int x[2], y[2];
	x[1] = round(cos(start_angle) * p->radius + p->x);
	y[1] = round(sin(start_angle) * p->radius + p->y);

	// draw lines for each point
	for (int i = 0; i < p->points_c; i++)
	{
		x[i & 1] = round(cos((i + 1) * incr_angle + start_angle) * p->radius + p->x);
		y[i & 1] = round(sin((i + 1) * incr_angle + start_angle) * p->radius + p->y);
		SDL_RenderDrawLine(renderer, x[0], y[0], x[1], y[1]);
	}
	//// temp storage for points. note: need one extra for last point to close the polygon
	//SDL_Point *points = (SDL_Point *)malloc(sizeof(SDL_Point) * p->points_c + 1);

	//// get all points
	//for (int i = 0; i <= p->points_c; i++)
	//{
	//	points[i].x = cos(i * incr_angle + start_angle) * p->radius + p->x;
	//	points[i].y = sin(i * incr_angle + start_angle) * p->radius + p->y;
	//}

	//SDL_RenderDrawLines(renderer, points, p->points_c + 1);
	//free(points);

	return 0;
}

int draw_polygon(SDL_Renderer *renderer, const struct polygon *p)
{
	if (renderer == NULL)
		return -1;

	if (p == NULL)
		return -1;

	SDL_RenderDrawLines(renderer, p->points_t, p->points_c);

	// close polygon
	SDL_RenderDrawLine(renderer,
			p->points_t[p->points_c - 1].x, p->points_t[p->points_c - 1].y,
			p->points_t[0].x, p->points_t[0].y);

	return 0;
}

int build_rcpolygon(struct polygon *p, int x, int y, int points_c, double radius, double angle, double scale)
{
	// error checking
	if (p == NULL)
		return -1;

	if (points_c < 3)
		return -1;

	// copy stuff
	p->x = x;
	p->y = y;
	p->angle = angle;
	p->scale = scale;
	p->points_c = points_c;
	p->points = (SDL_Point *)malloc(sizeof(SDL_Point) * points_c + 1);
	p->points_t = (SDL_Point *)malloc(sizeof(SDL_Point) * points_c + 1);

	/* 
	 * get angle for point ploting
	 * 1) sum of angles for a polygon
	 * 2) get the angle for each point
	 * 3) get angle from a corner to center
	 * 4) get the final increment angle
	 */
	double incr_angle;
	incr_angle = (points_c - 2.0f) * PI;
	incr_angle = incr_angle	/ points_c;
	incr_angle = incr_angle / 2.0f;
	incr_angle = PI - 2.0f * incr_angle;

	for (int i = 0; i <= p->points_c; i++)
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

int set_polygon_angle(struct polygon *p, double angle)
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

	for (int i = 0; i < p->points_c; i++)
	{
		p->points_t[i].x = p->x + p->scale * p->points[i].x * round(cos(angle)) - p->scale * p->points[i].y * round(sin(angle));
		p->points_t[i].y = p->y + p->scale * p->points[i].x * round(sin(angle)) + p->scale * p->points[i].y * round(cos(angle));
	}

	return 0;
}

void free_polygon(struct polygon *p)
{
	free(p->points);
	free(p->points_t);
}
