#include "bullet.h"

Bullet::Bullet(float x,float y,float z, glm::vec3 v,glm::vec3 a) {
  this->position = glm::vec3(0,0,0);
  this->acceleration = a;
  this->velocity = v;
  exists = true;
  glm::vec3 c3 = glm::normalize(v);
  glm::vec3 e1(0,1,0);
  glm::vec3 c2 = glm::normalize(glm::cross(e1,c3));
  glm::vec3 c1 = glm::normalize(glm::cross(c3,c2));

  for(int i=0;i<3;i++) {
    rotation_matrix[2][i] = c3[i];
    rotation_matrix[1][i] = c2[i];
    rotation_matrix[0][i] = c1[i];
  }

  bullet = Cuboid(x,y,z,0.3,0.1,1,(color_t){255,255,0});
  bullet.set_acceleration(a);
  bullet.set_velocity(v);
}

void Bullet::draw(glm::mat4 VP) {
  bullet.draw(VP,this->rotation_matrix);
}

void Bullet::tick() {
  bullet.tick();
  float dt;
  this->velocity += dt*this->acceleration;
  this->position += dt*this->velocity;
}