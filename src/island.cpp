#include "island.h"
#include "cannon.h"
#include<stdlib.h>

Island::Island(float x,float y,float z) {
  this->position = glm::vec3(x,y,z);
  this->length = rand()%5 + 2;
  this->breadth = rand()%5 + 2;
  int height_difference = 3; 
  float size = 15;
  int height = 5;
  int grass_height = 1;
  bool a = true;
  glm::vec3 highest(0,-10000,0);
  glm::vec3 pos = this->position;
  for(int i=0;i<length;i++) {
    pos = this->position + ((float)(size*i))*glm::vec3(1,0,0);
    height += height_difference*(rand()%3 - 1);
    if(height < 1) {
      height = 5;
    }
    if(a) {
      this->initial_height = height;
      a = false;
    } 
    int new_height = height;
    for(int j=0;j<breadth;j++) {
      cuboids.push_back(Cuboid(pos.x,pos.y+(new_height),pos.z+(float)(size*j),size/2,(float)new_height,size/2,(color_t){144,108,63}));
      cuboids.push_back(Cuboid(pos.x,pos.y+(2*new_height)+(grass_height),pos.z+(float)(size*j),size/2,(float)grass_height,size/2,(color_t){0,100,0}));
      new_height += height_difference*(rand()%3 - 1);
      if(new_height < 1) {
        new_height = 5;
      }
      if((float)new_height > highest.y) {
        highest = glm::vec3(pos.x,pos.y+(2*new_height)+(grass_height),pos.z+(float)(size*j));
      }
    }
  }
  int p = rand()%2;
  if(p==0) {
    has_cannon = true;
  }
  else {
    has_cannon = false;
  }
  if(has_cannon) {
    cannon = Cannon(highest.x,highest.y+15,highest.z);
  }
}

void Island::draw(glm::mat4 VP) {
  for(int i=0;i<cuboids.size();i++) {
    cuboids[i].draw(VP);
  }
  if(has_cannon) {
    cannon.draw(VP);
  }
}

void Island::tick(glm::vec3 r) {
  if(has_cannon) {
    cannon.tick(r);
    if(cannon.exists = false) {
      has_cannon = false;
    }
  }
}