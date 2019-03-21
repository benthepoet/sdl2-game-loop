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
  xmlTextReaderPtr reader = xmlNewTextReaderFilename(SPRITE_FILE);
  
  if (reader != NULL) {
    struct SpriteNode *head = NULL, *current, *node;
    
    while (xmlTextReaderRead(reader)) {
      int node_type = xmlTextReaderNodeType(reader);
      char *node_name = xmlTextReaderName(reader);
      
      switch (node_type) {
        case START_ELEMENT:
          if (!strcmp(node_name, "sp:sprite")) {
            node = calloc(1, sizeof(struct SpriteNode));
          }
          
          if (xmlTextReaderHasAttributes(reader)) {
            while(xmlTextReaderMoveToNextAttribute(reader)) {
              char *attr_name = xmlTextReaderName(reader); 
              char *attr_value = xmlTextReaderValue(reader);
              
              if (!strcmp(node_name, "sp:sprite")) {
                if (!strcmp(attr_name, "x")) {
                  node->data.x = atoi(attr_value);
                }
                if (!strcmp(attr_name, "y")) {
                  node->data.y = atoi(attr_value);
                }
              }
              
              if (!strcmp(node_name, "sp:animation")) {
                if (!strcmp(attr_name, "frame_width")) {
                  node->data.animation.frame_w = atoi(attr_value);
                }
                if (!strcmp(attr_name, "frame_height")) {
                  node->data.animation.frame_h = atoi(attr_value);
                }
                if (!strcmp(attr_name, "frame_span")) {
                  node->data.animation.frame_span = atoi(attr_value);
                }
              }
              
              xmlFree(attr_name);
              xmlFree(attr_value);
            }
          }
          
          break;
          
        case END_ELEMENT:
          if (!strcmp(node_name, "sp:sprite")) {
            if (head == NULL) {
              current = head = node;
            } else {
              current = current->next = node;
            }
          }
          break;
      }
      
      xmlFree(node_name);
    }
    
    xmlFreeTextReader(reader);
  }
}
