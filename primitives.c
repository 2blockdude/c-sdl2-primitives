#include <math.h>
#include "primitives.h"

#define PI 3.1415926535897932384626433832795

int draw_rcpolygon(SDL_Renderer *renderer, const struct reg_convex_polygon *p)
{
	// error checking
	if (renderer == NULL)
		return -1;

	if (p == NULL)
		return -1;

	if (p->n_sides < 3)
		return -1;

	double incr_angle;
	double start_angle;

	// get angle to for point ploting
	incr_angle = (p->n_sides - 2.0f) * PI;	// sum of angles for a polygon
	incr_angle = incr_angle	/ p->n_sides;	// get the angle for each point
	incr_angle = incr_angle / 2.0f;			// get angle from a corner to center
	incr_angle = PI - 2.0f * incr_angle;	// get the final increment angle

	start_angle = (p->n_sides % 2 == 1) ? (3.0f * PI / 2.0f) : (incr_angle / 2.0f);

	int x[2], y[2];
	x[1] = round(cos(start_angle) * p->radius + p->x);
	y[1] = round(sin(start_angle) * p->radius + p->y);

	// draw lines for each point
	for (int i = 0; i < p->n_sides; i++)
	{
		x[i & 1] = round(cos((i + 1) * incr_angle + start_angle) * p->radius + p->x);
		y[i & 1] = round(sin((i + 1) * incr_angle + start_angle) * p->radius + p->y);
		SDL_RenderDrawLine(renderer, x[0], y[0], x[1], y[1]);
	}
	//// temp storage for points. note: need one extra for last point to close the polygon
	//SDL_Point *points = (SDL_Point *)malloc(sizeof(SDL_Point) * p->n_sides + 1);

	//// get all points
	//for (int i = 0; i <= p->n_sides; i++)
	//{
	//	points[i].x = cos(i * incr_angle + start_angle) * p->radius + p->x;
	//	points[i].y = sin(i * incr_angle + start_angle) * p->radius + p->y;
	//}

	//SDL_RenderDrawLines(renderer, points, p->n_sides + 1);
	//free(points);

	return 0;
}

int draw_rcpolygon_filled(SDL_Renderer *renderer, const struct reg_convex_polygon *p)
{
	return 0;
}

int draw_polygon(SDL_Renderer *renderer, const struct polygon *p)
{
	return 0;
}

int draw_polygon_filled(SDL_Renderer *renderer, const struct polygon *p)
{
	return 0;
}

int draw_ellipse(SDL_Renderer *renderer, const struct ellipse *e)
{
	return 0;
}

int draw_ellipse_filled(SDL_Renderer *renderer, const struct ellipse *e)
{
	return 0;
}
