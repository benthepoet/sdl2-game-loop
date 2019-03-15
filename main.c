#include <stdio.h>

#include "SDL2/SDL.h"

#define WIN_X 640
#define WIN_Y 480
#define WIN_TITLE "Demo"

#define TICKS_PER_SECOND 60
#define SKIP_TICKS (1000 / TICKS_PER_SECOND)
#define MAX_FRAMESKIP 10

#define SCALE_FACTOR 3

#define FACING_LEFT SDL_FLIP_HORIZONTAL
#define FACING_RIGHT SDL_FLIP_NONE

struct Animation {
  int frame_w;
  int frame_h;
  int frame_offset;
  int frame_span;
  int frame_timer;
  int texture_w;
  SDL_Texture *texture;
};

struct Sprite {
  int x;
  int y;
  int velocity_x;
  SDL_RendererFlip orientation;
  struct Animation animation;
};

struct GameState {
  int sprite_count;
  struct Sprite *sprites;
};

struct GameTimer {
  int frame_loops;
  Uint32 next_tick;
  SDL_bool running;
};

struct GameState* init(SDL_Renderer *renderer);

void draw(SDL_Renderer *renderer, struct GameState *state);
void draw_sprite(SDL_Renderer *renderer, struct Sprite *sprite);

void update(struct GameState *state);
void update_animation(struct Animation *animation);

int main(int argc, char *argv[]) {
  SDL_Renderer *renderer;
  SDL_Window *window;

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    return 3;
  }

  if (SDL_CreateWindowAndRenderer(WIN_X, WIN_Y, 0, &window, &renderer)) {
    return 3;
  }

  SDL_SetWindowTitle(window, WIN_TITLE);

  struct GameState *state = init(renderer);
  struct GameTimer timer = { 0, 0, SDL_TRUE };

  while (timer.running) {
    SDL_Event event;

    timer.frame_loops = 0;

    while (SDL_GetTicks() > timer.next_tick && timer.frame_loops < MAX_FRAMESKIP) {
      update(state);
      timer.frame_loops++;
      timer.next_tick += SKIP_TICKS;
    }

    draw(renderer, state);

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          timer.running = SDL_FALSE;
          break;

        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_LEFT) {
            state->sprites->velocity_x = -1;
          }
          if (event.key.keysym.sym == SDLK_RIGHT) {
            state->sprites->velocity_x = 1;
          }
          break;

        case SDL_KEYUP:
          if (event.key.keysym.sym == SDLK_LEFT) {
            state->sprites->velocity_x = 0;
          }
          if (event.key.keysym.sym == SDLK_RIGHT) {
            state->sprites->velocity_x = 0;
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

  SDL_Surface *surface = SDL_LoadBMP("run.bmp");
  
  struct Animation animation;
  animation.frame_w = 24;
  animation.frame_h = 24;
  animation.frame_offset = 0;
  animation.frame_span = 10;
  animation.frame_timer = 0;
  animation.texture_w = surface->w;
  animation.texture = SDL_CreateTextureFromSurface(renderer, surface);
  
  SDL_FreeSurface(surface);

  initial->sprite_count = 1;
  initial->sprites = malloc(sizeof(struct Sprite));
  initial->sprites->x = 0;
  initial->sprites->y = 0;
  initial->sprites->velocity_x = 0;
  initial->sprites->orientation = FACING_RIGHT;
  initial->sprites->animation = animation;

  return initial;
}

void draw(SDL_Renderer *renderer, struct GameState *state) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  struct Sprite *sprite = state->sprites;

  for (int i = 0; i < state->sprite_count; i++, sprite++) {
    draw_sprite(renderer, sprite);
  }

  SDL_RenderPresent(renderer);
}

void draw_sprite(SDL_Renderer *renderer, struct Sprite *sprite) {
  SDL_Rect src;  
  src.x = sprite->animation.frame_offset;
  src.y = 0;
  src.w = sprite->animation.frame_w;
  src.h = sprite->animation.frame_h;
  
  SDL_Rect dst;
  dst.x = sprite->x * SCALE_FACTOR;
  dst.y = sprite->y * SCALE_FACTOR;
  dst.w = sprite->animation.frame_w * SCALE_FACTOR;
  dst.h = sprite->animation.frame_h * SCALE_FACTOR;

  SDL_RenderCopyEx(renderer, sprite->animation.texture, &src, &dst, 0, NULL, sprite->orientation);
}

void update(struct GameState *state) {
  struct Sprite *sprite = state->sprites;

  for (int i = 0; i < state->sprite_count; i++, sprite++) {
    sprite->x += sprite->velocity_x;

    if (sprite->velocity_x < 0) {
      sprite->orientation = FACING_LEFT;
    }
    if (sprite->velocity_x > 0) {
      sprite->orientation = FACING_RIGHT;
    }
    
    update_animation(&sprite->animation);
  }
}

void update_animation(struct Animation *animation) {
  animation->frame_timer++;
  
  if (animation->frame_timer > animation->frame_span) {
    animation->frame_timer = 0;
    animation->frame_offset += animation->frame_w;
  }

  if (animation->frame_offset == animation->texture_w) {
    animation->frame_offset = 0;
  }
}
