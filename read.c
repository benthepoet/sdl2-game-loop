#include <stdio.h>
#include <string.h>

#include "SDL2/SDL.h"

#include "sprite.h"

int main(int argc, char *argv[]) {
  FILE *fp = fopen("sprites.dat", "r");
  char buffer[64];

  struct SpriteNode *head, *iterator;
  
  while (fscanf(fp, "%s", buffer) != EOF) {
    if (strcmp(buffer, ":sprite") == 0) {
      struct SpriteNode *node = calloc(1, sizeof(struct SpriteNode));

      // Read position
      fscanf(fp, "%*s %d %d", &node->data.x, &node->data.y);
      printf("X: %d, Y: %d\n", node->data.x, node->data.y);
      
      // Read velocity
      fscanf(fp, "%*s %d", &node->data.velocity_x);
      printf("Velocity X: %d\n", node->data.velocity_x);
      
      if (head == NULL) {
        iterator = head = node;
      } else {
        iterator = iterator->next = node;
      }
    }
  }

  fclose(fp);
  
  iterator = head;
    
  while (iterator != NULL) {
    printf("%d %d\n", iterator->data.x, iterator->data.y);
    iterator = iterator->next;
  }

  return 0;
}
