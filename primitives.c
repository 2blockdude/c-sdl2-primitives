#include <math.h>
#include "primitives.h"

#include <stdio.h>

#define PI 3.1415926535897932384626433832795
#define MAX(a, b) (a > b) ? a : b
#define MIN(a, b) (a < b) ? a : b

/*
 * polygons
 */

// draw out line of a polygon
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

// fill in polygon with lines
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

			// get corrent point at i == 0
			if (i == 0) {
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

				float x_int, y_int;
				float slope;
				int rise, run;
				if (x1 == x2)
				{
					x_int = x1;
				}
				else
				{
					rise = y2 - y1;
					run = x2 - x1;
					slope = (float)rise / (float)run;
					y_int = (float)y1 - ((float)slope * (float)x1);
					x_int = ((float)y - (float)y_int) / (float)slope;
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

		//		float x_int, y_int;
		//		double slope;
		//		if (x1 == x2)
		//		{
		//			x_int = x1;
		//		}
		//		else
		//		{
		//			slope = ((float)(y2 - y1) / (float)(x2 - x1));
		//			y_int = ((float)y1 - (float)(slope * x1));

		//			if (slope == 0)
		//				continue;

		//			x_int = ((float)((float)(y - y_int) / (float)slope));
		//		}

		//		nodes_x[nint++] = x_int;
		//	}

		//	j = i;
		//}

		//  Sort the nodes.
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
	}

	return 0;
}

// create polygon defined by specified vectors
struct polygon *create_polygon(SDL_Point *vectors, int nsides, int x, int y, float angle)
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
	p->scale.x = 1;
	p->scale.y = 1;
	p->nsides = nsides;
	p->vectors = (SDL_Point *)malloc(sizeof(SDL_Point) * nsides);
	p->points = (SDL_Point *)malloc(sizeof(SDL_Point) * nsides);

	for (int i = 0; i < p->nsides; i++)
	{
		// set vectors
		p->vectors[i].x = vectors[i].x;
		p->vectors[i].y = vectors[i].y;

		// set points with angle and scale applied
		p->points[i].x = round(p->x + (p->scale.x * p->vectors[i].x * cos(p->angle)) - (p->scale.x * p->vectors[i].y * sin(p->angle)));
		p->points[i].y = round(p->y + (p->scale.y * p->vectors[i].x * sin(p->angle)) + (p->scale.y * p->vectors[i].y * cos(p->angle)));
	}

	return p;
}

// create regular concave polygon defined by radius and number of sides
struct polygon *create_rpolygon(int nsides, int x, int y, float radius, float angle)
{
	if (nsides < 3)
		return NULL;

	struct polygon *p = (struct polygon *)malloc(sizeof(struct polygon));

	// copy stuff
	p->x = x;
	p->y = y;
	p->angle = angle;
	p->scale.x = 1;
	p->scale.y = 1;
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
		p->points[i].x = round(p->x + (p->scale.x * p->vectors[i].x * cos(p->angle)) - (p->scale.x * p->vectors[i].y * sin(p->angle)));
		p->points[i].y = round(p->y + (p->scale.y * p->vectors[i].x * sin(p->angle)) + (p->scale.y * p->vectors[i].y * cos(p->angle)));
	}

	return p;
}

// move polygon to specified position
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

// set angle of polygon
int polygon_set_angle(struct polygon *p, float angle)
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
		p->points[i].x = round(p->x + (p->scale.x * p->vectors[i].x * cos(p->angle)) - (p->scale.x * p->vectors[i].y * sin(p->angle)));
		p->points[i].y = round(p->y + (p->scale.y * p->vectors[i].x * sin(p->angle)) + (p->scale.y * p->vectors[i].y * cos(p->angle)));
	}

	return 0;
}

// set scale of polygon
int polygon_set_scale(struct polygon *p, float scale_x, float scale_y)
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
		p->points[i].x = (p->points[i].x - p->x) / p->scale.x * scale_x + p->x;
		p->points[i].y = (p->points[i].y - p->y) / p->scale.y * scale_y + p->y;
	}

	p->scale.x = scale_x;
	p->scale.y = scale_y;

	return 0;
}

// rebuild polygon. used if changed values in struct without above functions
int polygon_rebuild(struct polygon *p)
{
	if (p == NULL)
		return -1;

	for (int i = 0; i < p->nsides; i++)
	{
		p->points[i].x = round(p->x + (p->scale.x * p->vectors[i].x * cos(p->angle)) - (p->scale.x * p->vectors[i].y * sin(p->angle)));
		p->points[i].y = round(p->y + (p->scale.y * p->vectors[i].x * sin(p->angle)) + (p->scale.y * p->vectors[i].y * cos(p->angle)));
	}

	return 0;
}

// free stuff
void free_polygon(struct polygon *p)
{
	free(p->vectors);
	free(p->points);
	free(p);
}

/*
 * floating point polygons
 */

int draw_fpolygon(SDL_Renderer *renderer, const struct fpolygon *p)
{
	if (renderer == NULL)
		return -1;

	if (p == NULL)
		return -1;

	SDL_RenderDrawLinesF(renderer, p->points, p->nsides);

	// close polygon
	SDL_RenderDrawLineF(renderer,
			p->points[p->nsides - 1].x, p->points[p->nsides - 1].y,
			p->points[0].x, p->points[0].y);

	return 0;
}

int draw_fpolygon_filled(SDL_Renderer *renderer, const struct fpolygon *p)
{
	if (renderer == NULL)
		return -1;

	if (p == NULL)
		return -1;

	float max_y = p->points[0].y;
	float min_y = p->points[0].y;

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
		float nodes_x[1000];

		//  Build a list of nodes.
		int ind1;
		int ind2;
		for (int i = 0; i < p->nsides; i++)
		{
			float x1;
			float y1;
			float x2;
			float y2;

			// get corrent point at i == 0
			if (i == 0) {
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
				float x_int, y_int;
				float slope;
				float rise, run;
				if (x1 == x2)
				{
					x_int = x1;
				}
				else
				{
					rise = y2 - y1;
					run = x2 - x1;
					slope = (float)rise / (float)run;
					y_int = (float)y1 - ((float)slope * (float)x1);
					x_int = ((float)y - (float)y_int) / (float)slope;
				}

				nodes_x[nint++] = x_int;
			}

		}

		//  Sort the nodes.
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

		if (nint % 2 == 0)
			for (int k = 0; k < nint; k += 2)
				SDL_RenderDrawLineF(renderer, nodes_x[k], y, nodes_x[k + 1], y);
	}

	return 0;
}

struct fpolygon *create_fpolygon(SDL_FPoint *vectors, int nsides, float x, float y, float angle)
{
	if (vectors == NULL)
		return NULL;

	if (nsides < 3)
		return NULL;

	struct fpolygon *p = (struct fpolygon *)malloc(sizeof(struct fpolygon));

	// copy stuff
	p->x = x;
	p->y = y;
	p->angle = angle;
	p->scale.x = 1;
	p->scale.y = 1;
	p->nsides = nsides;
	p->vectors = (SDL_FPoint *)malloc(sizeof(SDL_FPoint) * nsides);
	p->points = (SDL_FPoint *)malloc(sizeof(SDL_FPoint) * nsides);

	for (int i = 0; i < p->nsides; i++)
	{
		// set vectors
		p->vectors[i].x = vectors[i].x;
		p->vectors[i].y = vectors[i].y;

		// set points with angle and scale applied
		p->points[i].x = (float)(p->x + (p->scale.x * p->vectors[i].x * cos(p->angle)) - (p->scale.x * p->vectors[i].y * sin(p->angle)));
		p->points[i].y = (float)(p->y + (p->scale.y * p->vectors[i].x * sin(p->angle)) + (p->scale.y * p->vectors[i].y * cos(p->angle)));
	}

	return p;
}

struct fpolygon *create_rfpolygon(int nsides, float x, float y, float radius, float angle)
{
	if (nsides < 3)
		return NULL;

	struct fpolygon *p = (struct fpolygon *)malloc(sizeof(struct fpolygon));

	// copy stuff
	p->x = x;
	p->y = y;
	p->angle = angle;
	p->scale.x = 1;
	p->scale.y = 1;
	p->nsides = nsides;
	p->vectors = (SDL_FPoint *)malloc(sizeof(SDL_FPoint) * nsides);
	p->points = (SDL_FPoint *)malloc(sizeof(SDL_FPoint) * nsides);

	// i am retarded
	float incr_angle = 2.0f * PI / nsides;

	for (int i = 0; i < p->nsides; i++)
	{
		// set vectors
		p->vectors[i].x = (float)(cos(i * incr_angle) * radius);
		p->vectors[i].y = (float)(sin(i * incr_angle) * radius);

		// set vectors transformed
		p->points[i].x = (float)(p->x + (p->scale.x * p->vectors[i].x * cos(p->angle)) - (p->scale.x * p->vectors[i].y * sin(p->angle)));
		p->points[i].y = (float)(p->y + (p->scale.y * p->vectors[i].x * sin(p->angle)) + (p->scale.y * p->vectors[i].y * cos(p->angle)));
	}

	return p;
}

int fpolygon_translate(struct fpolygon *p, float x, float y)
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

int fpolygon_set_angle(struct fpolygon *p, float angle)
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
		p->points[i].x = (float)(p->x + (p->scale.x * p->vectors[i].x * cos(p->angle)) - (p->scale.x * p->vectors[i].y * sin(p->angle)));
		p->points[i].y = (float)(p->y + (p->scale.y * p->vectors[i].x * sin(p->angle)) + (p->scale.y * p->vectors[i].y * cos(p->angle)));
	}

	return 0;
}

int fpolygon_set_scale(struct fpolygon *p, float scale_x, float scale_y)
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
		p->points[i].x = (p->points[i].x - p->x) / p->scale.x * scale_x + p->x;
		p->points[i].y = (p->points[i].y - p->y) / p->scale.y * scale_y + p->y;
	}

	p->scale.x = scale_x;
	p->scale.y = scale_y;

	return 0;
}

// rebuild floating point polygon
int fpolygon_rebuild(struct fpolygon *p)
{
	if (p == NULL)
		return -1;

	for (int i = 0; i < p->nsides; i++)
	{
		p->points[i].x = (float)(p->x + (p->scale.x * p->vectors[i].x * cos(p->angle)) - (p->scale.x * p->vectors[i].y * sin(p->angle)));
		p->points[i].y = (float)(p->y + (p->scale.y * p->vectors[i].x * sin(p->angle)) + (p->scale.y * p->vectors[i].y * cos(p->angle)));
	}

	return 0;
}

void free_fpolygon(struct fpolygon *p)
{
	free(p->vectors);
	free(p->points);
	free(p);
}
