#include <math.h>
#include "primitives.h"

#define PI 3.1415926535897932384626433832795

struct polygon *create_polygon(float *input_vectors, int nsides, float x, float y, float angle)
{
   if (input_vectors == NULL)
      return NULL;

   if (nsides < 3)
      return NULL;

   struct polygon *p = (struct polygon *)malloc(sizeof(struct polygon));

   // init values
   p->position.x = x;
   p->position.y = y;
   p->scale.x = 1;
   p->scale.y = 1;
   p->angle = angle;

   p->nsides = nsides;
   p->radius = 0;
   p->far.x = 0;
   p->far.y = 0;
   p->centroid.x = 0;
   p->centroid.y = 0;

   p->vectors = (struct point *)malloc(sizeof(struct point) * nsides);
   p->points = (struct point *)malloc(sizeof(struct point) * nsides);

   for (int i = 0; i < p->nsides; i++)
   {
      // set vectors
      p->vectors[i].x = ((struct point *)input_vectors)[i].x;
      p->vectors[i].y = ((struct point *)input_vectors)[i].y;

      // set points with angle and scale applied
      p->points[i].x = (p->position.x + p->scale.x * ((p->vectors[i].x * cos(p->angle)) - (p->vectors[i].y * sin(p->angle))));
      p->points[i].y = (p->position.y + p->scale.y * ((p->vectors[i].x * sin(p->angle)) + (p->vectors[i].y * cos(p->angle))));
   }

   return p;
}

struct polygon *create_reg_polygon(int nsides, float x, float y, float angle, float radius)
{
   if (nsides < 3)
      return NULL;

   struct point *vectors = (struct point *)malloc(sizeof(struct point) * nsides);

   float incr_angle = 2.0f * PI / nsides;

   for (int i = 0; i < nsides; i++)
   {
      // set vectors
      vectors[i].x = cos(i * incr_angle) * radius;
      vectors[i].y = sin(i * incr_angle) * radius;
   }

   struct polygon *p = create_polygon((float *)vectors, nsides, x, y, angle);
   free(vectors);

   return p;
}

struct polygon *create_rand_polygon(int nsides, float x, float y, float angle, float max_radius, float min_radius, float angle_offset)
{
   if (nsides < 3)
      return NULL;

   struct point *vectors = (struct point *)malloc(sizeof(struct point) * nsides);

   float incr_angle = 2.0f * PI / nsides;

   for (int i = 0; i < nsides; i++)
   {
      // set vectors
      float rand_radius = (float)((double)rand() * (double)((max_radius - min_radius) / RAND_MAX)) + min_radius;
      float rand_angle = (float)((double)rand() * (double)(((((i + 1) * incr_angle) - i * incr_angle) * angle_offset) / RAND_MAX)) + i * incr_angle;

      vectors[i].x = cos(rand_angle) * rand_radius;
      vectors[i].y = sin(rand_angle) * rand_radius;
   }

   struct polygon *p = create_polygon((float *)vectors, nsides, x, y, angle);
   free(vectors);

   return p;
}

struct polygon *create_copy_polygon(struct polygon *p)
{
   if (p == NULL)
      return NULL;

   struct polygon *p_copy = create_polygon((float *)p->vectors, p->nsides, p->position.x, p->position.y, p->angle);
   polygon_scale(p_copy, p->scale.x, p->scale.y);

   return p_copy;
}

// rebuild floating point polygon
int polygon_rebuild(struct polygon *p)
{
   if (p == NULL)
      return -1;

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
      p->points[i].x = (p->position.x + p->scale.x * ((p->vectors[i].x * cos(p->angle)) - (p->vectors[i].y * sin(p->angle))));
      p->points[i].y = (p->position.y + p->scale.y * ((p->vectors[i].x * sin(p->angle)) + (p->vectors[i].y * cos(p->angle))));
   }

   return 0;
}

int polygon_translate(struct polygon *p, float x, float y)
{
   if (p == NULL)
      return -1;

   for (int i = 0; i < p->nsides; i++)
   {
      p->points[i].x += x;
      p->points[i].y += y;
   }

   // set new position
   p->position.x += x;
   p->position.y += y;

   return 0;
}

int polygon_rotate(struct polygon *p, float angle)
{
   if (p == NULL)
      return -1;

   p->angle += angle;

   for (int i = 0; i < p->nsides; i++)
   {
      // 4 additions 2 multiplications
      p->points[i].x = (p->position.x + (((p->points[i].x - p->position.x) * cos(p->angle)) - ((p->points[i].y - p->position.y) * sin(p->angle))));
      p->points[i].y = (p->position.y + (((p->points[i].x - p->position.x) * sin(p->angle)) + ((p->points[i].y - p->position.y) * cos(p->angle))));

      // 2 additions 3 multiplications
      //p->points[i].x = (p->position.x + p->scale.x * ((p->vectors[i].x * cos(p->angle)) - (p->vectors[i].y * sin(p->angle))));
      //p->points[i].y = (p->position.y + p->scale.y * ((p->vectors[i].x * sin(p->angle)) + (p->vectors[i].y * cos(p->angle))));
   }

   return 0;
}

int polygon_scale(struct polygon *p, float scale_x, float scale_y)
{
   if (p == NULL)
      return -1;

   p->scale.x += scale_x;
   p->scale.y += scale_y;

   for (int i = 0; i < p->nsides; i++)
   {
      p->points[i].x = (p->points[i].x - p->position.x) * scale_x + p->position.x;
      p->points[i].y = (p->points[i].y - p->position.y) * scale_y + p->position.y;
   }

   return 0;
}

int polygon_set_pos(struct polygon *p, float x, float y)
{
   if (p == NULL)
      return -1;

   for (int i = 0; i < p->nsides; i++)
   {
      p->points[i].x = (p->points[i].x - p->position.x) + x;
      p->points[i].y = (p->points[i].y - p->position.y) + y;
   }

   // store new position
   p->position.x = x;
   p->position.y = y;

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

   p->scale.x = scale_x;
   p->scale.y = scale_y;

   polygon_rebuild(p);

   return 0;
}

void free_polygon(struct polygon *p)
{
   free(p->vectors);
   free(p->points);
   free(p);
}
