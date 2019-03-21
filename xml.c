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
            
            if (xmlTextReaderHasAttributes(reader)) {
              while(xmlTextReaderMoveToNextAttribute(reader)) {
                char *attr_name = xmlTextReaderName(reader); 
                char *attr_value = xmlTextReaderValue(reader);
                
                if (!strcmp(name, "x")) {
                  node->data.x = atoi(attr_value);
                }
                if (!strcmp(name, "y")) {
                  node->data.y = atoi(attr_value);
                }

                xmlFree(attr_name);
                xmlFree(attr_value);
              }
            }
            
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

int read_sprite(xmlTextReaderPtr reader, struct Sprite *sprite) {
  int ret = 0;
  
  while (!ret) {
    xmlTextReaderRead(reader);
    
    int type = xmlTextReaderNodeType(reader);
    char *name = xmlTextReaderName(reader);
    
    switch (type) {
      case START_ELEMENT:
        if (!strcmp(name, "sp:animation")) {
          if (xmlTextReaderHasAttributes(reader)) {
              while(xmlTextReaderMoveToNextAttribute(reader)) {
                char *attr_name = xmlTextReaderName(reader); 
                char *attr_value = xmlTextReaderValue(reader);
                
                if (!strcmp(attr_name, "frame_width")) {
                  sprite->animation.frame_w = atoi(attr_value);
                }
                if (!strcmp(attr_name, "frame_height")) {
                  sprite->animation.frame_h = atoi(attr_value);
                }
                if (!strcmp(attr_name, "frame_span")) {
                  sprite->animation.frame_span = atoi(attr_value);
                }
                
                xmlFree(attr_name);
                xmlFree(attr_value);
              }
            }
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
