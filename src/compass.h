#include "main.h"
#include "cuboid.h"

#ifndef COMPASS_H
#define COMPASS_H

class Compass {
public:
  Compass() {}
  Compass(float x,float y,float z);
  glm::vec3 position;
  glm::mat4 rotation_matrix;
  void draw(glm::mat4 VP,glm::mat4 rotate);
private:
  VAO *compass;

};

#endif 