#include <math.h>
#include "primitives.h"

#include <stdio.h>

#define PI 3.1415926535897932384626433832795
#define MAX(a, b) (a > b) ? a : b
#define MIN(a, b) (a < b) ? a : b

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

	int max_y = p->points[0].y;
	int min_y = p->points[0].y;

	for (int i = 0; i < p->nsides; i++)
	{
		max_y = p->points[i].y > max_y ? p->points[i].y : max_y;
		min_y = p->points[i].y < min_y ? p->points[i].y : min_y;
	}

	int rand_y;
	SDL_GetMouseState(NULL, &rand_y);

	int nint = 0;
	int intersections[1000];
	for (int j = 0; j < p->nsides; j++)
	{
		int x1 = p->points[j].x;
		int y1 = p->points[j].y;
		int x2 = p->points[(j + 1) % p->nsides].x;
		int y2 = p->points[(j + 1) % p->nsides].y;

		double slope;
		double y_int;
		int x_int;

		if (x1 == x2)
		{
			x_int = x1;

			int max_y, min_y;
			max_y = MAX(y1, y2);
			min_y = MIN(y1, y2);
			if (rand_y > max_y || rand_y < min_y)
				continue;
		}
		else
		{
			slope = (double)(y2 - y1) / (double)(x2 - x1);
			y_int = (double)y1 - (double)(slope * x1);

			if (slope == 0)
				continue;

			x_int = (double)((double)(rand_y - y_int) / (double)slope);
		}

		int max_x, min_x;
		max_x = MAX(x1, x2);
		min_x = MIN(x1, x2);

		if ((x_int < max_x && x_int > min_x) || x1 == x2 || rand_y == y1 || rand_y == y2)
			intersections[nint++] = x_int;
	}

	if (nint % 2 == 0)
	{
		for (int k = 0; k < nint; k += 2)
		{
			SDL_RenderDrawLine(renderer, intersections[k], rand_y, intersections[k + 1], rand_y);

			struct polygon *p;
			p = create_reg_polygon(50, intersections[k], rand_y, 10, 0, 1, 1);
			draw_polygon(renderer, p);
			free_polygon(p);

			p = create_reg_polygon(50, intersections[k + 1], rand_y, 10, 0, 1, 1);
			draw_polygon(renderer, p);
			free_polygon(p);
		}
	}

	printf("%d\n", nint);



	// intersections for x only
	//int *intersections = (int *)calloc(2000, sizeof(int));
	//for (int i = min_y; i <= max_y; i++)
	//{
	//	// number of intersections
	//	int nint = 0;

	//	int j;
	//	for (j = 0; j < p->nsides; j++)
	//	{
	//		int x1 = p->points[j].x;
	//		int y1 = p->points[j].y;
	//		int x2 = p->points[(j + 1) % p->nsides].x;
	//		int y2 = p->points[(j + 1) % p->nsides].y;

	//		double slope;
	//		double y_int;
	//		int x_int;

	//		if (x1 == x2)
	//		{
	//			x_int = x1;
	//		}
	//		else
	//		{
	//			// slop and y intersect
	//			slope = (double)(y2 - y1) / (double)(x2 - x1);
	//			y_int = (double)y1 - (double)(slope * x1);

	//			if (slope == 0)
	//				continue;

	//			x_int = (double)((double)(i - y_int) / (double)slope);
	//		}

	//		int max, min;
	//		max = MAX(x1, x2);
	//		min = MIN(x1, x2);
	//		if (x_int < max && x_int > min)
	//		{
	//			intersections[nint++] = x_int;
	//		}
	//	}

	//	printf("intersections: %d\n", nint);
	//	printf("calculations : %d\n", j);
	//	printf("current line : %d\n", i);

	//	if (nint % 2 == 0)
	//	{
	//		for (int k = 0; k < nint; k += 2)
	//		{
	//			SDL_RenderDrawLine(renderer, intersections[k], i, intersections[k + 1], i);
	//		}
	//	}
	//}
	//free(intersections);

	return 0;
}

struct polygon *create_polygon(SDL_Point *vectors, int nsides, int x, int y, float angle, float scale_x, float scale_y)
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

struct polygon *create_reg_polygon(int nsides, int x, int y, float radius, float angle, float scale_x, float scale_y)
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
	float incr_angle = 2.0f * PI / nsides;

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

int polygon_angle(struct polygon *p, float angle)
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

int polygon_scale(struct polygon *p, float scale_x, float scale_y)
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
