#include <stdio.h>

#include "SDL2/SDL.h"

#define WIN_X 640
#define WIN_Y 480
#define TICKS_PER_SECOND 60
#define MAX_FRAMESKIP 10

const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;

struct Animation {
  int w;
  int h;
  int currentFrame;
  int numFrames;
  int frameDuration;
  int timer;
  SDL_Surface *surface;
  SDL_Texture *texture;
};

struct GameState {
  int offset;
  int speed;
  struct Animation *run;
};

void init(struct GameState **state, SDL_Renderer *renderer);
void draw(SDL_Renderer *renderer, struct GameState *state);
void update(struct GameState *state);
SDL_Rect* makeRect(int x, int y, int w, int h);

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

  SDL_SetWindowTitle(window, "Demo");
  
  struct GameState *state;
  Uint32 nextTick = 0;
  int frameLoops;
  
  init(&state, renderer);

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

void init(struct GameState **state, SDL_Renderer *renderer) {
  struct GameState *initial = malloc(sizeof(struct GameState));
  initial->offset = 0;
  initial->speed = 4;

  initial->run = malloc(sizeof(struct Animation));
  initial->run->timer = 0;
  initial->run->w = 24;
  initial->run->currentFrame = 0;
  initial->run->numFrames = 4;
  initial->run->frameDuration = 10;

  SDL_Surface *surface = SDL_LoadBMP("run.bmp");
  initial->run->texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  
  *state = initial;
}

void draw(SDL_Renderer *renderer, struct GameState *state) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  int x = state->run->currentFrame * state->run->w;
  SDL_Rect *srcRect = makeRect(x, 0, state->run->w, state->run->w);
  SDL_Rect *dstRect = makeRect(0, 0, state->run->w * 3, state->run->w * 3);
  
  SDL_RenderCopy(renderer, state->run->texture, srcRect, dstRect);

  SDL_RenderPresent(renderer);
}

void update(struct GameState *state) {
  state->run->timer++;

  if (state->run->timer > state->run->frameDuration) {
    state->run->timer = 0;
    state->run->currentFrame++;
    if (state->run->currentFrame == state->run->numFrames) {
      state->run->currentFrame = 0;
    }
  }
}

SDL_Rect* makeRect(int x, int y, int w, int h) {
  SDL_Rect *rect = malloc(sizeof(SDL_Rect));
  rect->x = x;
  rect->y = y;
  rect->w = w;
  rect->h = h;
  return rect;
}
