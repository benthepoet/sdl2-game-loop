#include <stdio.h>

#include "SDL2/SDL.h"

#include "sprite.h"

#define WIN_X 640
#define WIN_Y 480
#define WIN_TITLE "Demo"

#define TICKS_PER_SECOND 60
#define SKIP_TICKS (1000 / TICKS_PER_SECOND)
#define MAX_FRAMESKIP 10

#define SCALE_FACTOR 3

#define SPRITE_FILE "sprites.dat"

#define FACING_LEFT SDL_FLIP_HORIZONTAL
#define FACING_RIGHT SDL_FLIP_NONE

struct GameState {
  struct SpriteNode *sprites;
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
  
  struct GameTimer timer;
  timer.next_tick = 0;
  timer.running = SDL_TRUE;

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
            state->sprites->data.velocity_x = -1;
          }
          if (event.key.keysym.sym == SDLK_RIGHT) {
            state->sprites->data.velocity_x = 1;
          }
          break;

        case SDL_KEYUP:
          if (event.key.keysym.sym == SDLK_LEFT) {
            state->sprites->data.velocity_x = 0;
          }
          if (event.key.keysym.sym == SDLK_RIGHT) {
            state->sprites->data.velocity_x = 0;
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
  struct GameState *initial = calloc(1, sizeof(struct GameState));
  //initial->sprites = NULL;

  FILE *fp = fopen(SPRITE_FILE, "r");
  char buffer[64];
  
  struct SpriteNode *current;
  
  while (fscanf(fp, "%s", buffer) != EOF) {
    if (strcmp(buffer, ":sprite") == 0) {
      // Initialize node
      struct SpriteNode *node = calloc(1, sizeof(struct SpriteNode));

      // Read position
      fscanf(fp, "%*s %d %d", &node->data.x, &node->data.y);
      printf("X: %d, Y: %d\n", node->data.x, node->data.y);

      // Read velocity
      fscanf(fp, "%*s %d", &node->data.velocity_x);
      printf("Velocity X: %d\n", node->data.velocity_x);

      // Read animation
      struct Animation *animation = &node->data.animation;
      animation->frame_offset = 0;
      animation->frame_timer = 0;

      char texture_file[64];

      fscanf(fp, "%*s %s %d %d %d", texture_file, &animation->frame_w, &animation->frame_h, &animation->frame_span);   

      // Load texture
      SDL_Surface *surface = SDL_LoadBMP(texture_file);
      animation->texture_w = surface->w;
      animation->texture = SDL_CreateTextureFromSurface(renderer, surface);
      SDL_FreeSurface(surface);

      if (initial->sprites == NULL) {
        current = initial->sprites = node;
      } else {
        current = current->next = node;
      }
    }
  }
  
  fclose(fp);

  return initial;
}

void draw(SDL_Renderer *renderer, struct GameState *state) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  struct SpriteNode *current = state->sprites;

  while (current != NULL) {
    draw_sprite(renderer, &current->data);
    current = current->next;
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
  struct SpriteNode *current = state->sprites;

  while (current != NULL) {
    struct Sprite *sprite = &current->data;

    sprite->x += sprite->velocity_x;

    if (sprite->velocity_x < 0) {
      sprite->orientation = FACING_LEFT;
    }
    if (sprite->velocity_x > 0) {
      sprite->orientation = FACING_RIGHT;
    }

    update_animation(&sprite->animation);
    current = current->next;
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
