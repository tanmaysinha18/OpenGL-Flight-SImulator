#include "main.h"
#include "cuboid.h"
#include "cannon.h"

#ifndef ISLAND_H
#define ISLAND_H

class Island {
public:
  Island() {}
  Island(float x,float y,float z);
  glm::vec3 position;
  void draw(glm::mat4 VP);
  float length,breadth,height;
  int num_cuboids;
  bool has_cannon;
  Cannon cannon;
  int initial_height;
  void tick(glm::vec3 r);
private:
  std:: vector<Cuboid> cuboids;
};

#endif 