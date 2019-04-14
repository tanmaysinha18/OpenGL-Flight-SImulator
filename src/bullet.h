#include "main.h"
#include "cuboid.h"

#ifndef BULLET_H
#define BULLET_H

class Bullet {
public:
  Bullet() {}
  Bullet(float x,float y,float z,glm::vec3 v,glm::vec3 a);
  glm::vec3 position;
  glm::vec3 velocity;
  glm::vec3 acceleration;
  glm::mat4 rotation_matrix;
  float dist_human;
  void draw(glm::mat4 VP);
  void tick();
  bool exists;
  Cuboid bullet;
private:

};

#endif 