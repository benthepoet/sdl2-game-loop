#include <stdio.h>

#include "SDL2/SDL.h"

#define WIN_X 640
#define WIN_Y 480
#define TICKS_PER_SECOND 60
#define MAX_FRAMESKIP 10

const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;

struct GameState {
  int offset;
  int speed;
};

void init(struct GameState **state);
void draw(SDL_Renderer *renderer, struct GameState *state);
void update(struct GameState *state);

int main(int argc, char *argv[]) {
  SDL_bool running = SDL_TRUE;
  SDL_Renderer *renderer;
  SDL_Window *window;
  
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    return 3;
  }

  if (SDL_CreateWindowAndRenderer(WIN_X, WIN_Y, 0, &window, &renderer)) {
    return 3;
  }

  struct GameState *state;
  Uint32 nextTick = 0;
  int frameLoops;
  
  init(&state);

  while (running) {
    SDL_Event event;

    frameLoops = 0;
    
    while (SDL_GetTicks() > nextTick && frameLoops < MAX_FRAMESKIP) {
      update(state);
      nextTick += SKIP_TICKS;
      frameLoops++;
    }
    
    draw(renderer, state);
    
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
	running = SDL_FALSE;
      }
    }
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  
  return 0;
}

void init(struct GameState **state) {
  struct GameState *initial = malloc(sizeof(struct GameState));
  initial->offset = 0;
  initial->speed = 4;

  *state = initial;
}

void draw(SDL_Renderer *renderer, struct GameState *state) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDrawLine(renderer, state->offset, 200, 100 + state->offset, 200);
  SDL_RenderPresent(renderer);
}

void update(struct GameState *state) {
  state->offset += state->speed;
}
