#include "SDL2/SDL.h"

#define FPS 50
#define FRAME_SIZE 1000
#define WIN_X 640
#define WIN_Y 480

const int SKIP_TICKS = 1000 / FPS;

typedef struct
{
  Uint32 next_tick;
} GameTimer;

int dir = 1;
int offset = 0;

int main(int argc, char* argv[])
{
  GameTimer game_timer;
  game_timer.next_tick = SDL_GetTicks();
  
    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        if (SDL_CreateWindowAndRenderer(WIN_X, WIN_Y, 0, &window, &renderer) == 0) {
            SDL_bool done = SDL_FALSE;

            while (!done) {
                SDL_Event event;

		if (SDL_GetTicks() > game_timer.next_tick) {
		  game_timer.next_tick += SKIP_TICKS;
		  offset += dir;

		  if (offset < 0) {
		    dir = 1;
		  }
		  if (offset > 50) {
		    dir = -1;
		  }
		}
		  
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(renderer, 320 + offset, 200, 300 + offset, 240);
		SDL_RenderDrawLine(renderer, 300 + offset, 240, 340 + offset, 240);
		SDL_RenderDrawLine(renderer, 340 + offset, 240, 320 + offset, 200);
		SDL_RenderPresent(renderer);
		
		while (SDL_PollEvent(&event)) {
		  if (event.type == SDL_QUIT) {
		      done = SDL_TRUE;
		  }
		}
		
            }
        }

        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
    }
    SDL_Quit();
    return 0;
}

