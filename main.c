#include "SDL2/SDL.h"

#define WIN_X 640
#define WIN_Y 480

void draw(SDL_Renderer *renderer);

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

  while (running) {
    SDL_Event event;

    draw(renderer);
    
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

void draw(SDL_Renderer *renderer) {
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
}
