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

int main(int argc, char *argv[]) {
  xmlTextReaderPtr reader;
  reader = xmlNewTextReaderFilename(SPRITE_FILE);
  
  if (reader != NULL) {
    struct Sprite *sprite;
    
    while (xmlTextReaderRead(reader)) {
      int node_type = xmlTextReaderNodeType(reader);
      char *node_name = xmlTextReaderName(reader);
      
      switch (node_type) {
        case START_ELEMENT:
          if (named(node_name, "sp:sprite")) {
            sprite = calloc(1, sizeof(struct Sprite));
          
            if (xmlTextReaderHasAttributes(reader)) {
              while(xmlTextReaderMoveToNextAttribute(reader)) {
                char *attr_name = xmlTextReaderName(reader); 
                char *attr_value = xmlTextReaderValue(reader);
                
                if (named(attr_name, "x")) {
                  sprite->x = atoi(attr_value);
                }
                if (named(attr_name, "y")) {
                  sprite->y = atoi(attr_value);
                }
              }
            }
          }
          
          break;
          
        case END_ELEMENT:
          if (named(node_name, "sp:sprite")) {
            sprite = NULL;
          }
          break;
      }
    }
    xmlFreeTextReader(reader);
    free(sprite);
  }
}

int named(char *node_name, char *name) {
  return strcmp(node_name, name) == 0;
}
