#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mxml.h>
#include <SDL2/SDL.h>

#include "sprite.h"

#define START_ELEMENT 1
#define END_ELEMENT 15

#define SPRITE_FILE "sprites.xml"

struct GameState {
  struct SpriteNode *sprite_head;
};

void sax_cb(mxml_node_t *node, mxml_sax_event_t event, void *data);

int main(int argc, char *argv[]) {
  struct GameState *state = calloc(1, sizeof(struct GameState));
  
  FILE *fp = fopen(SPRITE_FILE, "r");
  mxmlSAXLoadFile(NULL, fp, MXML_TEXT_CALLBACK, sax_cb, state);
  
  fclose(fp);
  free(state);
}

void sax_cb(mxml_node_t *node, mxml_sax_event_t event, void *data) {
  struct GameState *state = (struct GameState *)data;
  
  if (event == MXML_SAX_ELEMENT_OPEN) {
    const char *name = mxmlGetElement(node);
    
    if (!strcmp(name, "sp:sprite")) {
      struct SpriteNode *node = calloc(1, sizeof(struct SpriteNode));
      node->next = state->sprite_head;
      state->sprite_head = node;
    }
  }
  
  if (event == MXML_SAX_DATA) {
    const char *value = mxmlGetText(node, 0);
    const char *parent = mxmlGetElement(mxmlGetParent(node));
    
    if (!strcmp(parent, "sp:x")) {
      state->sprite_head->data.x = atoi(value);
    }
    
    if (!strcmp(parent, "sp:y")) {
      state->sprite_head->data.y = atoi(value);
    }
    
    if (!strcmp(parent, "an:frame_width")) {
      state->sprite_head->data.animation.frame_w = atoi(value);
    }
    
    if (!strcmp(parent, "an:frame_height")) {
      state->sprite_head->data.animation.frame_h = atoi(value);
    }
    
    if (!strcmp(parent, "an:frame_span")) {
      state->sprite_head->data.animation.frame_span = atoi(value);
    }
  }
}
