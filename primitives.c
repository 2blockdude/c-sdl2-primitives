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

	//int y;
	//SDL_GetMouseState(NULL, &y);

	for (int y = min_y; y <= max_y; y++)
	{
		int nint = 0;
		int nodes_x[1000];

		//  Build a list of nodes.
		int ind1;
		int ind2;
		for (int i = 0; i < p->nsides; i++)
		{
			int x1;
			int y1;
			int x2;
			int y2;

			if (!i) {
				ind1 = p->nsides - 1;
				ind2 = 0;
			} else {
				ind1 = i - 1;
				ind2 = i;
			}
			y1 = p->points[ind1].y;
			y2 = p->points[ind2].y;
			if (y1 < y2) {
				x1 = p->points[ind1].x;
				x2 = p->points[ind2].x;
			} else if (y1 > y2) {
				y2 = p->points[ind1].y;
				y1 = p->points[ind2].y;
				x2 = p->points[ind1].x;
				x1 = p->points[ind2].x;
			} else {
				continue;
			}
			if ( ((y >= y1) && (y < y2)) || ((y == max_y) && (y > y1) && (y <= y2)) )
			{
				//nodes_x[nint++] = ((65536 * (y - y1)) / (y2 - y1)) * (x2 - x1) + (65536 * x1);

				int x_int, y_int;
				double slope;
				if (x1 == x2)
				{
					x_int = x1;
				}
				else
				{
					slope = ((double)(y2 - y1) / (double)(x2 - x1));
					y_int = ((double)y1 - (double)(slope * x1));

					if (slope == 0)
						continue;

					x_int = ceil((double)((double)(y - y_int) / (double)slope));
				}

				nodes_x[nint++] = x_int;
			}

		}

		//int j = p->nsides - 1;
		//for (int i = 0; i < p->nsides; i++)
		//{
		//	if ((p->points[i].y < y && p->points[j].y >= y) ||
		//			(p->points[j].y < y && p->points[i].y >= y))
		//	{
		//		//nodes_x[nint++] = p->points[i].x + (y - p->points[i].y) / (p->points[j].y - p->points[i].y) * (p->points[j].x - p->points[i].x);
		//		int x1 = p->points[j].x;
		//		int y1 = p->points[j].y;
		//		int x2 = p->points[i].x;
		//		int y2 = p->points[i].y;

		//		int x_int, y_int;
		//		double slope;
		//		if (x1 == x2)
		//		{
		//			x_int = x1;
		//		}
		//		else
		//		{
		//			slope = ((double)(y2 - y1) / (double)(x2 - x1));
		//			y_int = ((double)y1 - (double)(slope * x1));

		//			if (slope == 0)
		//				continue;

		//			x_int = ceil((double)((double)(y - y_int) / (double)slope));
		//		}

		//		nodes_x[nint++] = x_int;
		//	}

		//	j = i;
		//}

		//  Sort the nodes, via a simple “Bubble” sort.
		int i = 0;
		while (i < nint - 1)
		{
			if (nodes_x[i] > nodes_x[i + 1])
			{
				int swap;
				swap = nodes_x[i];
				nodes_x[i] = nodes_x[i + 1];
				nodes_x[i + 1] = swap;
				if (i)
					i--;
			}
			else
			{
				i++;
			}
		}

		//for (int j = 0; j < p->nsides; j++)
		//{
		//	int x1 = p->points[j].x;
		//	int y1 = p->points[j].y;
		//	int x2 = p->points[(j + 1) % p->nsides].x;
		//	int y2 = p->points[(j + 1) % p->nsides].y;

		//	double slope;
		//	double y_int;
		//	int x_int;

		//	if (x1 == x2)
		//	{
		//		x_int = x1;
		//	}
		//	else
		//	{
		//		slope = (double)(y2 - y1) / (double)(x2 - x1);
		//		y_int = (double)y1 - (double)(slope * x1);

		//		if (slope == 0)
		//			continue;

		//		x_int = ((double)((double)(y - y_int) / (double)slope));
		//	}

		//	int max_x, min_x;
		//	max_x = MAX(x1, x2);
		//	min_x = MIN(x1, x2);

		//	int max_y, min_y;
		//	max_y = MAX(y1, y2);
		//	min_y = MIN(y1, y2);

		//	if (x_int <= max_x && x_int >= min_x &&
		//			intersections[nint - 1] != x_int &&
		//			y >= min_y && y <= max_y)
		//		nodes_x[nint++] = x_int;
		//}

		if (nint % 2 == 0)
		{
			for (int k = 0; k < nint; k += 2)
			{
				SDL_RenderDrawLine(renderer, nodes_x[k], y, nodes_x[k + 1], y);

				//struct polygon *p;
				//p = create_reg_polygon(50, nodes_x[k], y, 10, 0, 1, 1);
				//draw_polygon(renderer, p);
				//free_polygon(p);

				//p = create_reg_polygon(50, nodes_x[k + 1], y, 10, 0, 1, 1);
				//draw_polygon(renderer, p);
				//free_polygon(p);
			}
		}
		printf("%d\n", nint);
	}

	//if (rand_y > max_y || rand_y < min_y)
	//	continue;


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
