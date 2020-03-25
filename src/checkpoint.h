#include "main.h"

#ifndef CHECKPOINT_H
#define CHECKPOINT_H

class Checkpoint {
public:
  Checkpoint() {}
  Checkpoint(float x,float y,float z,float radius);
  glm::vec3 position;
  float cylinder_radius;
  static constexpr float cylinder_length = 200;
  float ring_radius;
  static constexpr float ring_length = 1;
  void draw(glm::mat4 VP);
  bool check_passing(glm::vec3 pos);
  bool active;
  void change_activity();
  glm::vec3 get_position();
private:
  VAO *cylinder;
  VAO *ring;
  VAO *arrow;
};

#endif 