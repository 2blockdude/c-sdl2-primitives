#include <math.h>
#include "primitives.h"

#define PI 3.1415926535897932384626433832795

int draw_rpolygon(SDL_Renderer *renderer, const struct regular_polygon *p)
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

	// temp storage for points. note: need one extra for last point to close the polygon
	SDL_Point *points = (SDL_Point *)malloc(sizeof(SDL_Point) * p->n_sides + 1);

	// get all points
	for (int i = 0; i <= p->n_sides; i++)
	{
		points[i].x = cos(i * incr_angle + start_angle) * p->radius + p->x;
		points[i].y = sin(i * incr_angle + start_angle) * p->radius + p->y;
	}

	SDL_RenderDrawLines(renderer, points, p->n_sides + 1);
	free(points);

	return 0;
}

int draw_rpolygon_filled(SDL_Renderer *renderer, const struct regular_polygon *p)
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
