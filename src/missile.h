#include "main.h"

#ifndef MISSILE_H
#define MISSILE_H

class Missile {
public:
  Missile() {}
  Missile(float x,float y,float z,glm::mat4 rotation);
  glm::vec3 position;
  glm::vec3 velocity;
  glm::vec3 acceleration;
  float cylinder_radius;
  static constexpr float cylinder_length = 1;
  void draw(glm::mat4 VP);
  void tick();
  bool exists;
  glm::vec3 get_position();
  glm::mat4 rotation_matrix;
private:
  VAO *cylinder;
  VAO *fins;
};

#endif 