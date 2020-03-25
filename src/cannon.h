#include "main.h"

#ifndef CANNON_H
#define CANNON_H

class Cannon {
public:
  Cannon() {}
  Cannon(float x,float y,float z);
  glm::vec3 position;
  glm::mat4 rotation_matrix;
  float cylinder_radius;
  static constexpr float cylinder_length = 20;
  void draw(glm::mat4 VP);
  void tick(glm::vec3 r);
  bool active;
  void change_activity();
  glm::vec3 get_position();
  time_t time_since_fire;
  bool to_fire(); 
  bool exists;
private:
  VAO *cylinder;
};

#endif 