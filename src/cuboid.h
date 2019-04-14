#include "main.h"

#ifndef CUBOID_H
#define CUBOID_H

class Cuboid {
public:
  Cuboid() {}
  Cuboid(float x,float y,float z,float length,float breadth,float height,color_t color);
  glm::vec3 position;
  glm::vec3 velocity;
  glm::vec3 acceleration;
  void set_acceleration(glm::vec3 a);
  void set_velocity(glm::vec3 v);
  void tick();
  void draw(glm::mat4 VP);
  float length;
  float breadth;
  float height;
  void draw(glm::mat4 VP,glm::mat4 rotate);
private:
  VAO *cuboid;
};

#endif 