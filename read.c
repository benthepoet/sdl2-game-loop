#include <stdio.h>
#include <string.h>

#include "SDL2/SDL.h"

#include "sprite.h"

int main(int argc, char *argv[]) {
  FILE *fp = fopen("sprites.dat", "r");
  char buffer[64];

  struct SpriteNode *root, *current;
  
  while (fscanf(fp, "%s", buffer) != EOF) {
    if (strcmp(buffer, ":sprite") == 0) {
      struct SpriteNode *node = malloc(sizeof(struct SpriteNode));

      // Read position
      fscanf(fp, "%*s %d %d", &node->sprite.x, &node->sprite.y);
      printf("X: %d, Y: %d\n", node->sprite.x, node->sprite.y);
      
      // Read velocity
      fscanf(fp, "%*s %d", &node->sprite.velocity_x);
      printf("Velocity X: %d\n", node->sprite.velocity_x);
      
      if (root == NULL) {
        current = root = node;
      } else {
        current = current->next = node;
      }
    }
  }
  
  current = root;
  
  while (current != NULL) {
    printf("%d %d\n", current->sprite.x, current->sprite.y);
    current = current->next;
  }
  
  fclose(fp);
  return 0;
}
