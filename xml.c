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

struct Loader {
  struct GameState *state;
  void *ptr;
};

void sax_cb(mxml_node_t *node, mxml_sax_event_t event, void *data);

int main(int argc, char *argv[]) {
  struct Loader loader;
  loader.state = calloc(1, sizeof(struct GameState));
  
  FILE *fp = fopen(SPRITE_FILE, "r");
  mxmlSAXLoadFile(NULL, fp, MXML_TEXT_CALLBACK, sax_cb, &loader);
  
  free(loader.state);
}

void sax_cb(mxml_node_t *node, mxml_sax_event_t event, void *data) {
  struct Loader *loader = (struct Loader*)data;
  
  if (event == MXML_SAX_ELEMENT_OPEN) {
    const char *name = mxmlGetElement(node);
    
    if (!strcmp(name, "sp:sprite")) {
      loader->ptr = calloc(1, sizeof(struct SpriteNode));
      struct SpriteNode *node = loader->ptr;
      node->next = loader->state->sprite_head;
      loader->state->sprite_head = node;
    }
  }
  
  if (event == MXML_SAX_DATA) {
    const char *value = mxmlGetText(node, 0);
    const char *parent = mxmlGetElement(mxmlGetParent(node));
    
    if (!strcmp(parent, "sp:x")) {
      loader->state->sprite_head->data.x = atoi(value);
    }
    
    if (!strcmp(parent, "sp:y")) {
      loader->state->sprite_head->data.y = atoi(value);
    }
    
    if (!strcmp(parent, "an:frame_width")) {
      loader->state->sprite_head->data.animation.frame_w = atoi(value);
    }
    
    if (!strcmp(parent, "an:frame_height")) {
      loader->state->sprite_head->data.animation.frame_h = atoi(value);
    }
    
    if (!strcmp(parent, "an:frame_span")) {
      loader->state->sprite_head->data.animation.frame_span = atoi(value);
    }
  }
}
