#include <math.h>
#include "primitives.h"

#define PI 3.1415926535897932384626433832795

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

   SDL_FPoint *points = (SDL_FPoint *)p->points;

   float max_y = points[0].y;
   float min_y = points[0].y;

   for (int i = 0; i < p->nsides; i ++)
   {
      max_y = points[i].y > max_y ? points[i].y : max_y;
      min_y = points[i].y < min_y ? points[i].y : min_y;
   }

   int nint;
   float nodes_x[p->nsides];

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

   SDL_FPoint *vectors = (SDL_FPoint *)p->vectors;
   SDL_FPoint *points = (SDL_FPoint *)p->points;

   for (int i = 0; i < p->nsides; i++)
   {
      // set vectors
      vectors[i].x = input_vectors[i * 2];
      vectors[i].y = input_vectors[i * 2 + 1];

      // set points with angle and scale applied
      points[i].x = (float)(p->x + (p->scale.x * vectors[i].x * cos(p->angle)) - (p->scale.x * vectors[i].x * sin(p->angle)));
      points[i].y = (float)(p->y + (p->scale.y * vectors[i].y * sin(p->angle)) + (p->scale.y * vectors[i].y * cos(p->angle)));
   }

   return p;
}

struct polygon *create_reg_polygon(int nsides, float x, float y, float radius, float angle)
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
   p->vectors = (float *)malloc(sizeof(float) * nsides * 2);
   p->points  = (float *)malloc(sizeof(float) * nsides * 2);

   SDL_FPoint *vectors = (SDL_FPoint *)p->vectors;
   SDL_FPoint *points = (SDL_FPoint *)p->points;

   // i am retarded
   float incr_angle = 2.0f * PI / nsides;

   for (int i = 0; i < p->nsides; i++)
   {
      // set vectors
      vectors[i].x = (float)(cos(i * incr_angle) * radius);
      vectors[i].y = (float)(sin(i * incr_angle) * radius);

      // set vectors transformed
      points[i].x = (float)(p->x + (p->scale.x * vectors[i].x * cos(p->angle)) - (p->scale.x * vectors[i].y * sin(p->angle)));
      points[i].y = (float)(p->y + (p->scale.y * vectors[i].x * sin(p->angle)) + (p->scale.y * vectors[i].y * cos(p->angle)));
   }

   return p;
}

struct polygon *create_rand_polygon(int nsides, float x, float y, float max_radius, float min_radius, float angle_offset, float angle)
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
   p->vectors = (float *)malloc(sizeof(float) * nsides * 2);
   p->points =  (float *)malloc(sizeof(float) * nsides * 2);

   SDL_FPoint *vectors = (SDL_FPoint *)p->vectors;
   SDL_FPoint *points = (SDL_FPoint *)p->points;

   // i am retarded
   float incr_angle = 2.0f * PI / nsides;

   for (int i = 0; i < p->nsides; i++)
   {
      // set vectors
      float radius = (float)((double)rand() * (double)((max_radius - min_radius) / RAND_MAX)) + min_radius;
      float rangle = (float)((double)rand() * (double)(((((i + 1) * incr_angle) - i * incr_angle) * angle_offset) / RAND_MAX)) + i * incr_angle;

      vectors[i].x = (float)(cos(rangle) * radius);
      vectors[i].y = (float)(sin(rangle) * radius);

      // set vectors transformed
      points[i].x = (float)(cos(rangle + p->angle) * radius + p->x);
      points[i].y = (float)(sin(rangle + p->angle) * radius + p->y);
   }

   return p;
}

int polygon_translate(struct polygon *p, float x, float y)
{
   if (p == NULL)
      return -1;

   SDL_FPoint *points = (SDL_FPoint *)p->points;

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

   SDL_FPoint *vectors = (SDL_FPoint *)p->vectors;
   SDL_FPoint *points = (SDL_FPoint *)p->points;

   p->angle = angle;

   /*
    * 2d rotation matrix
    * 
    * R = {cos(theta), -sin(theta)}
    *		 {sin(theta),  cos(theta)}
    */

   for (int i = 0; i < p->nsides; i++)
   {
      points[i].x = (float)(p->x + (p->scale.x * vectors[i].x * cos(p->angle)) - (p->scale.x * vectors[i].y * sin(p->angle)));
      points[i].y = (float)(p->y + (p->scale.y * vectors[i].x * sin(p->angle)) + (p->scale.y * vectors[i].y * cos(p->angle)));
   }

   return 0;
}

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

   SDL_FPoint *points = (SDL_FPoint *)p->points;

   for (int i = 0; i < p->nsides; i++)
   {
      points[i].x = (points[i].x - p->x) / p->scale.x * scale_x + p->x;
      points[i].y = (points[i].y - p->y) / p->scale.y * scale_y + p->y;
   }

   p->scale.x = scale_x;
   p->scale.y = scale_y;

   return 0;
}

// rebuild floating point polygon
int polygon_rebuild(struct polygon *p)
{
   if (p == NULL)
      return -1;

   SDL_FPoint *vectors = (SDL_FPoint *)p->vectors;
   SDL_FPoint *points = (SDL_FPoint *)p->points;

   for (int i = 0; i < p->nsides; i++)
   {
      points[i].x = (float)(p->x + (p->scale.x * vectors[i].x * cos(p->angle)) - (p->scale.x * vectors[i].y * sin(p->angle)));
      points[i].y = (float)(p->y + (p->scale.y * vectors[i].x * sin(p->angle)) + (p->scale.y * vectors[i].y * cos(p->angle)));
   }

   return 0;
}

void free_polygon(struct polygon *p)
{
   free(p->vectors);
   free(p->points);
   free(p);
}
