#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/xmlreader.h>
#include <SDL2/SDL.h>

#include "sprite.h"

#define START_ELEMENT 1
#define TEXT_ELEMENT 3
#define END_ELEMENT 15

#define SPRITE_FILE "sprites.xml"

struct GameState {
  struct SpriteNode *sprite_head;
};

int main(int argc, char *argv[]) {
  struct GameState *state = calloc(1, sizeof(struct GameState));
  
  xmlTextReaderPtr reader = xmlNewTextReaderFilename(SPRITE_FILE);
  
  if (reader != NULL) {
    while (xmlTextReaderRead(reader)) {
      int type = xmlTextReaderNodeType(reader);
      char *name = xmlTextReaderName(reader);

      switch (type) {
        case START_ELEMENT:
          if (!strcmp(name, "sp:sprite")) {
            struct SpriteNode *node = calloc(1, sizeof(struct SpriteNode));
            node->next = state->sprite_head;
            state->sprite_head = node; 
          }
          
          if (!strcmp(name, "sp:x")) {
            char *value = xmlTextReaderReadString(reader);
            state->sprite_head->data.x = atoi(value);
            xmlFree(value);
          }
          
          if (!strcmp(name, "sp:y")) {
            char *value = xmlTextReaderReadString(reader);
            state->sprite_head->data.y = atoi(value);
            xmlFree(value);
          }
          
          if (!strcmp(name, "an:frame_width")) {
            char *value = xmlTextReaderReadString(reader);
            state->sprite_head->data.animation.frame_w = atoi(value);
            xmlFree(value);
          }
          
          if (!strcmp(name, "an:frame_height")) {
            char *value = xmlTextReaderReadString(reader);
            state->sprite_head->data.animation.frame_h = atoi(value);
            xmlFree(value);
          }
          
          if (!strcmp(name, "an:frame_span")) {
            char *value = xmlTextReaderReadString(reader);
            state->sprite_head->data.animation.frame_span = atoi(value);
            xmlFree(value);
          }
          
          break;
      }
      
      xmlFree(name);
    }
    
    xmlFreeTextReader(reader);
  }
  
  free(state);
}
