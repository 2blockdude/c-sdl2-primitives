#include <math.h>
#include <SDL2/SDL.h>
#include "primitives.h"

#define SCREEN_WIDTH		700
#define SCREEN_HEIGHT	700

typedef struct game_window game_window;

struct game_window
{
   // window stuff
   SDL_Window *window;
   SDL_Renderer *renderer;
   SDL_Event event;

   // bools
   char running;
   char pause;
   char mouse_down;
   char left_down;
   char right_down;

   // stuff
   int mouse_pos_x;
   int mouse_pos_y;

   // settings
   int width;
   int height;

   int sides;
   float angle;

   double delta;
};

int init_window(game_window *win, char *title, int width, int height)
{
   int wflags = 0; // window flags
   int rflags = 0; // render flags

   if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
      return -1;

   win->window = SDL_CreateWindow(title, 0, 0, width, height, wflags);

   if (!win->window)
   {
      SDL_Quit();
      return -2;
   }

   win->renderer = SDL_CreateRenderer(win->window, -1, rflags);

   if (!win->renderer)
   {
      SDL_DestroyWindow(win->window);
      SDL_Quit();
      return -3;
   }

   // init data
   SDL_GetMouseState(&win->mouse_pos_x, &win->mouse_pos_y);
   win->width = width;
   win->height = height;
   win->mouse_down = 0;
   win->running = 1;
   win->pause = 0;
   win->sides = 3;
   win->angle = 0;
   win->delta = 0;
   win->left_down = 0;
   win->right_down = 0;

   return 0;
}

void destroy_window(game_window *win)
{
   // clean up
   SDL_DestroyWindow(win->window);
   SDL_DestroyRenderer(win->renderer);
   SDL_Quit();
}

void handle_events(game_window *win)
{
   while (SDL_PollEvent(&(win->event)))
   {
      switch (win->event.type)
      {
         case SDL_QUIT:
            win->running = 0;
            break;

         case SDL_MOUSEMOTION:
            SDL_GetMouseState(&win->mouse_pos_x, &win->mouse_pos_y);
            if (win->pause == 1 && win->mouse_down == 1 && win->event.button.button == SDL_BUTTON_LEFT)
            {
            }
            break;

         case SDL_MOUSEBUTTONDOWN:
            if (win->pause == 1 && win->mouse_down == 0 && win->event.button.button == SDL_BUTTON_LEFT)
            {
               win->mouse_down = 1;
            }
            break;

         case SDL_MOUSEBUTTONUP:
            if (win->mouse_down == 1) win->mouse_down = 0;
            break;

         case SDL_KEYDOWN:
            switch (win->event.key.keysym.sym)
            {
               case SDLK_SPACE:
                  win->sides = 3;
                  break;
               case SDLK_UP:
                  win->sides++;
                  break;

               case SDLK_DOWN:
                  if (win->sides != 3)
                     win->sides--;
                  break;

               case SDLK_LEFT:
                  win->left_down = 1;
                  break;

               case SDLK_RIGHT:
                  win->right_down = 1;
                  break;
            };
            break;

         case SDL_KEYUP:
            switch (win->event.key.keysym.sym)
            {
               case SDLK_LEFT:
                  win->left_down = 0;
                  break;

               case SDLK_RIGHT:
                  win->right_down = 0;
                  break;
            };
            break;
      };
   }
}

void render(game_window *win)
{
   SDL_RenderClear(win->renderer);

   SDL_SetRenderDrawColor(win->renderer, 0, 0, 0, 255);

   struct fpolygon *fp;
   struct polygon *p;
   fp = create_rfpolygon(win->sides, 350, 350, 100, win->angle);

   p = create_rpolygon(win->sides, 350, 350, 100, win->angle);
   draw_fpolygon(win->renderer, fp);
   draw_fpolygon_filled(win->renderer, fp);
   free_polygon(p);

   fpolygon_translate(fp, 100, 100);
   draw_fpolygon(win->renderer, fp);
   draw_fpolygon_filled(win->renderer, fp);

   fpolygon_translate(fp, 600, 100);
   draw_fpolygon(win->renderer, fp);
   draw_fpolygon_filled(win->renderer, fp);

   fpolygon_translate(fp, 600, 600);
   draw_fpolygon(win->renderer, fp);
   draw_fpolygon_filled(win->renderer, fp);

   fpolygon_translate(fp, 100, 600);
   draw_fpolygon(win->renderer, fp);
   draw_fpolygon_filled(win->renderer, fp);
   free_fpolygon(fp);

   SDL_SetRenderDrawColor(win->renderer, 255, 255, 255, 255);
   SDL_RenderPresent(win->renderer);
}

int main()
{
   game_window win;
   int code = init_window(&win, "primitives", SCREEN_WIDTH, SCREEN_HEIGHT);
   if (code != 0) return -1;

   while (win.running)
   {

      if (win.left_down)
         win.angle -= 5.0f * win.delta;

      if (win.right_down)
         win.angle += 5.0f * win.delta;

      unsigned int start, end;

      start = SDL_GetTicks();

      handle_events(&win);
      if (!win.pause)
         render(&win);

      SDL_Delay((1000.0f / 60.0f) - (SDL_GetTicks() - start));

      end = SDL_GetTicks();

      win.delta = (float)(end - start) / 1000.0f;
   }

   destroy_window(&win);

   return 0;
}
