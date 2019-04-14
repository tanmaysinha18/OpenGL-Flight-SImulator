#include "main.h"
#include "cuboid.h"

#ifndef PARACHUTE_H
#define PARACHUTE_H

class Parachute {
public:
  Parachute() {}
  Parachute(float x,float y,float z);
  glm::vec3 position;
  glm::vec3 velocity;
  glm::vec3 acceleration;
  float dist_human;
  void draw(glm::mat4 VP);
  void tick();
  bool exists;
private:
  VAO *parachute;
  std::vector<Cuboid> human_parts;
};

#endif 