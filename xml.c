#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/xmlreader.h>
#include <SDL2/SDL.h>

#include "sprite.h"

#define START_ELEMENT 1
#define END_ELEMENT 15

#define SPRITE_FILE "sprites.xml"

int named(char*, char*);
int read_sprite(xmlTextReaderPtr, struct Sprite*);

int main(int argc, char *argv[]) {
  xmlTextReaderPtr reader = xmlNewTextReaderFilename(SPRITE_FILE);
  
  if (reader != NULL) {
    struct SpriteNode *head = NULL, *current, *node;
    
    while (xmlTextReaderRead(reader)) {
      int type = xmlTextReaderNodeType(reader);
      char *name = xmlTextReaderName(reader);

      switch (type) {
        case START_ELEMENT:
          if (!strcmp(name, "sp:sprite")) {
            node = calloc(1, sizeof(struct SpriteNode));

            if (read_sprite(reader, &node->data)) {
              if (head == NULL) {
                current = head = node;
              } else {
                current = current->next = node;
              }
            }
          }
          
          break;
      }
      
      xmlFree(name);
    }
    
    xmlFreeTextReader(reader);
  }
}

int read_animation(xmlTextReaderPtr reader, struct Animation *animation) {
  int ret = 0;
  
  while (!ret) {
    xmlTextReaderRead(reader);
    
    int type = xmlTextReaderNodeType(reader);
    char *name = xmlTextReaderName(reader);
    char *value;
    
    switch (type) {
      case START_ELEMENT:
        value = xmlTextReaderReadString(reader);
      
        if (!strcmp(name, "an:frame_width")) {
          animation->frame_w = atoi(value);
        }
        
        if (!strcmp(name, "an:frame_height")) {
          animation->frame_h = atoi(value);
        }
        
        if (!strcmp(name, "an:frame_span")) {
          animation->frame_span = atoi(value);
        }
        
        xmlFree(value);
        break;
        
      case END_ELEMENT:
        if (!strcmp(name, "an:animation")) {
          ret = 1;
        }
    }
    
    xmlFree(name);
  }
  
  return ret;
}

int read_sprite(xmlTextReaderPtr reader, struct Sprite *sprite) {
  int ret = 0;
  
  while (!ret) {
    xmlTextReaderRead(reader);
    
    int type = xmlTextReaderNodeType(reader);
    char *name = xmlTextReaderName(reader);
    
    switch (type) {
      case START_ELEMENT:
        if (!strcmp(name, "an:animation")) {
          read_animation(reader, &sprite->animation);
        } else {
          char *value = xmlTextReaderReadString(reader);
          
          if (!strcmp(name, "sp:x")) {
            sprite->x = atoi(value);
          }
          
          if (!strcmp(name, "sp:y")) {
            sprite->y = atoi(value);
          }
          
          xmlFree(value);
        }
        break;
        
      case END_ELEMENT:
        if (!strcmp(name, "sp:sprite")) {
          ret = 1;
        }
    }
    
    xmlFree(name);
  }
  
  return ret;
}
