#include <stdio.h>

#include "SDL2/SDL.h"

#define WIN_X 640
#define WIN_Y 480
#define TICKS_PER_SECOND 60
#define SKIP_TICKS (1000 / TICKS_PER_SECOND)
#define MAX_FRAMESKIP 10
#define SCALE_FACTOR 3

struct Animation {
  int w;
  int h;
  int current;
  int total;
  int duration;
  int timer;
  SDL_Texture *texture;
};

struct Sprite {
  int x;
  int y;
  int speed;
  SDL_RendererFlip direction;
  struct Animation animation;
};

struct GameState {
  struct Sprite *sprites;
};

struct GameState* init(SDL_Renderer *renderer);
void draw(SDL_Renderer *renderer, struct GameState *state);
void drawSprite(SDL_Renderer *renderer, struct Sprite *sprite);
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

  SDL_SetWindowTitle(window, "Demo");
  
  Uint32 nextTick = 0;
  int frameLoops;
  
  struct GameState *state = init(renderer);
  
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
      switch (event.type) {
        case SDL_QUIT:
	  running = SDL_FALSE;
	  break;

        case SDL_KEYDOWN:
	  if (event.key.keysym.sym == SDLK_LEFT) {
	    state->sprites->speed = -1;
	  }
	  if (event.key.keysym.sym == SDLK_RIGHT) {
	    state->sprites->speed = 1;
	  }
	  break;

	case SDL_KEYUP:
	  if (event.key.keysym.sym == SDLK_LEFT) {
	    state->sprites->speed = 0;
	  }
	  if (event.key.keysym.sym == SDLK_RIGHT) {
	    state->sprites->speed = 0;
	  }
	  default:
	    break;
      }
    }
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  
  return 0;
}

struct GameState* init(SDL_Renderer *renderer) {
  struct GameState *initial = malloc(sizeof(struct GameState));

  struct Animation animation = { 24, 24, 0, 4, 10, 0 };
  SDL_Surface *surface = SDL_LoadBMP("run.bmp");
  animation.texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  initial->sprites = malloc(sizeof(struct Sprite));
  initial->sprites->x = 0;
  initial->sprites->y = 0;
  initial->sprites->speed = 0;
  initial->sprites->direction = SDL_FLIP_NONE;
  initial->sprites->animation = animation;
  
  return initial;
}

void draw(SDL_Renderer *renderer, struct GameState *state) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  int n = sizeof(*state->sprites) / sizeof(struct Sprite);
  struct Sprite *sprite = state->sprites;
  
  for (int i = 0; i < n; i++, sprite++) {
    drawSprite(renderer, sprite);
  }
  
  SDL_RenderPresent(renderer);
}

void drawSprite(SDL_Renderer *renderer, struct Sprite *sprite) {
  int x = sprite->animation.w * sprite->animation.current;
  
  SDL_Rect src = { x, 0, sprite->animation.w, sprite->animation.h };
  SDL_Rect dst = {
    sprite->x * SCALE_FACTOR,
    sprite->y * SCALE_FACTOR,
    sprite->animation.w * SCALE_FACTOR,
    sprite->animation.h * SCALE_FACTOR
  };
  
  SDL_RenderCopyEx(renderer, sprite->animation.texture, &src, &dst, 0, NULL, sprite->direction);
}

void update(struct GameState *state) {
  int n = sizeof(*state->sprites) / sizeof(struct Sprite);
  struct Sprite *sprite = state->sprites;

  for (int i = 0; i < n; i++, sprite++) {
    sprite->animation.timer++;
    sprite->x += sprite->speed;

    if (sprite->speed < 0) {
      sprite->direction = SDL_FLIP_HORIZONTAL;
    }
    if (sprite->speed > 0) {
      sprite->direction = SDL_FLIP_NONE;
    }
    
    if (sprite->animation.timer > sprite->animation.duration) {
      sprite->animation.timer = 0;
      sprite->animation.current++;

      if (sprite->animation.current == sprite->animation.total) {
	sprite->animation.current = 0;
      }
    }
  }
}
