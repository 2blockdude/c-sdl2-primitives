#include <math.h>
#include "primitives.h"

#define PI 3.1415926535897932384626433832795

// simple struct for casting
struct point
{
   float x;
   float y;
};

int draw_polygon(SDL_Renderer *renderer, const struct polygon *p)
{
   if (renderer == NULL)
      return -1;

   if (p == NULL)
      return -1;

   SDL_FPoint *points = (SDL_FPoint *)p->points;

   SDL_RenderDrawLinesF(renderer, points, p->nsides);
   SDL_RenderDrawLineF(renderer,
         points[0].x, points[0].y,
         points[p->nsides - 1].x, points[p->nsides - 1].y);

   return 0;
}

int fpolycmp(const void *a, const void *b)
{
   return *(float *)a - *(float *)b;
}

int draw_polygon_filled(SDL_Renderer *renderer, const struct polygon *p)
{
   if (renderer == NULL)
      return -1;

   if (p == NULL)
      return -1;

   struct point *points = (struct point *)p->points;

   float max_y = points[0].y;
   float min_y = points[0].y;

   for (int i = 0; i < p->nsides; i ++)
   {
      max_y = points[i].y > max_y ? points[i].y : max_y;
      min_y = points[i].y < min_y ? points[i].y : min_y;
   }

   int nint;
   float *nodes_x = (float *)malloc(sizeof(float) * p->nsides);

   for (int y = min_y; y <= max_y; y++)
   {
      nint = 0;

      //  Build a list of nodes.
      int ind1;
      int ind2;
      for (int i = 0; i < p->nsides; i++)
      {
         float x1;
         float y1;
         float x2;
         float y2;

         // get current point at i == 0
         if (i == 0) {
            ind1 = p->nsides - 1;
            ind2 = 0;
         } else {
            ind1 = i - 1;
            ind2 = i;
         }

         y1 = points[ind1].y;
         y2 = points[ind2].y;
         if (y1 < y2) {
            x1 = points[ind1].x;
            x2 = points[ind2].x;
         } else if (y1 > y2) {
            y2 = points[ind1].y;
            y1 = points[ind2].y;
            x2 = points[ind1].x;
            x1 = points[ind2].x;
         } else {
            continue;
         }
         if ( ((y >= y1) && (y < y2)) || ((y == max_y) && (y > y1) && (y <= y2)) )
         {
            nodes_x[nint++] = (y - y1) * (x2 - x1) / (y2 - y1) + x1;
         }

      }

      qsort(nodes_x, nint, sizeof(float), fpolycmp);

      if (nint % 2 == 0)
         for (int k = 0; k < nint; k += 2)
            SDL_RenderDrawLineF(renderer, nodes_x[k], y, nodes_x[k + 1], y);
   }

   free(nodes_x);

   return 0;
}

struct polygon *create_polygon(float *input_vectors, int nsides, float x, float y, float angle)
{
   if (input_vectors == NULL)
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
   p->vectors = (float *)malloc(sizeof(float) * nsides * 2);
   p->points = (float *)malloc(sizeof(float) * nsides * 2);

   struct point *vectors = (struct point *)p->vectors;
   struct point *points = (struct point *)p->points;

   for (int i = 0; i < p->nsides; i++)
   {
      // set vectors
      vectors[i].x = ((struct point *)input_vectors)[i].x;
      vectors[i].y = ((struct point *)input_vectors)[i].y;

      // set points with angle and scale applied
      points[i].x = (float)(p->x + p->scale.x * ((vectors[i].x * cos(p->angle)) - (vectors[i].y * sin(p->angle))));
      points[i].y = (float)(p->y + p->scale.y * ((vectors[i].x * sin(p->angle)) + (vectors[i].y * cos(p->angle))));
   }

   return p;
}

struct polygon *create_reg_polygon(int nsides, float x, float y, float angle, float radius)
{
   if (nsides < 3)
      return NULL;

   float *vectors = (float *)malloc(sizeof(float) * nsides * 2);

   float incr_angle = 2.0f * PI / nsides;

   for (int i = 0; i < nsides; i++)
   {
      // set vectors
      ((struct point *)vectors)[i].x = (float)(cos(i * incr_angle) * radius);
      ((struct point *)vectors)[i].y = (float)(sin(i * incr_angle) * radius);
   }

   struct polygon *p = create_polygon(vectors, nsides, x, y, angle);
   free(vectors);
   return p;
}

struct polygon *create_rand_polygon(int nsides, float x, float y, float angle, float max_radius, float min_radius, float angle_offset)
{
   if (nsides < 3)
      return NULL;

   float *vectors = (float *)malloc(sizeof(float) * nsides * 2);

   float incr_angle = 2.0f * PI / nsides;

   for (int i = 0; i < nsides; i++)
   {
      // set vectors
      float rand_radius = (float)((double)rand() * (double)((max_radius - min_radius) / RAND_MAX)) + min_radius;
      float rand_angle = (float)((double)rand() * (double)(((((i + 1) * incr_angle) - i * incr_angle) * angle_offset) / RAND_MAX)) + i * incr_angle;

      ((struct point *)vectors)[i].x = (float)(cos(rand_angle) * rand_radius);
      ((struct point *)vectors)[i].y = (float)(sin(rand_angle) * rand_radius);
   }

   struct polygon *p = create_polygon(vectors, nsides, x, y, angle);
   free(vectors);
   return p;
}

struct polygon *create_copy_polygon(struct polygon *p)
{
   if (p == NULL)
      return NULL;

   struct polygon *p_copy = create_polygon(p->vectors, p->nsides, p->x, p->y, p->angle);
   p_copy->scale.x = p->scale.x;
   p_copy->scale.y = p->scale.y;

   return p_copy;
}

// rebuild floating point polygon
int polygon_rebuild(struct polygon *p)
{
   if (p == NULL)
      return -1;

   struct point *vectors = (struct point *)p->vectors;
   struct point *points = (struct point *)p->points;

   /*
    * 2d rotation matrix
    * 
    * R = {cos(theta), -sin(theta)}
    *		 {sin(theta),  cos(theta)}
    */

   /*
    * 2d scaling matrix
    *
    * S = { Sx, 0 }
    *		 { 0, Sy }
    */

   for (int i = 0; i < p->nsides; i++)
   {
      points[i].x = (float)(p->x + p->scale.x * ((vectors[i].x * cos(p->angle)) - (vectors[i].y * sin(p->angle))));
      points[i].y = (float)(p->y + p->scale.y * ((vectors[i].x * sin(p->angle)) + (vectors[i].y * cos(p->angle))));
   }

   return 0;
}

int polygon_translate(struct polygon *p, float x, float y)
{
   if (p == NULL)
      return -1;

   struct point *points = (struct point *)p->points;

   for (int i = 0; i < p->nsides; i++)
   {
      points[i].x = points[i].x - p->x + x;
      points[i].y = points[i].y - p->y + y;
   }

   // store new position
   p->x = x;
   p->y = y;

   return 0;
}

int polygon_set_angle(struct polygon *p, float angle)
{
   if (p == NULL)
      return -1;

   p->angle = angle;

   polygon_rebuild(p);

   return 0;
}

int polygon_set_scale(struct polygon *p, float scale_x, float scale_y)
{
   if (p == NULL)
      return -1;

   struct point *points = (struct point *)p->points;

   for (int i = 0; i < p->nsides; i++)
   {
      points[i].x = (points[i].x - p->x) / p->scale.x * scale_x + p->x;
      points[i].y = (points[i].y - p->y) / p->scale.y * scale_y + p->y;
   }

   p->scale.x = scale_x;
   p->scale.y = scale_y;

   return 0;
}

void free_polygon(struct polygon *p)
{
   free(p->vectors);
   free(p->points);
   free(p);
}
