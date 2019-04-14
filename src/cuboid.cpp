#include "cuboid.h"

Cuboid::Cuboid(float x,float y,float z,float x_length,float y_breadth,float z_height,color_t color) {
  this->position = glm::vec3(x,y,z);
  length = x_length;
  breadth = y_breadth;
  height = z_height;
  this->acceleration = glm::vec3(0,0,0);
  this->velocity = glm::vec3(0,0,0);
  GLfloat vertex_buffer_data[] = {
        -length,-breadth,-height, 
        -length,-breadth, height,
        -length, breadth, height, 
        length, breadth,-height, 
        -length,-breadth,-height,
        -length, breadth,-height, 
        length,-breadth, height,
        -length,-breadth,-height,
        length,-breadth,-height,
        length, breadth,-height,
        length,-breadth,-height,
        -length,-breadth,-height,
        -length,-breadth,-height,
        -length, breadth, height,
        -length, breadth,-height,
        length,-breadth, height,
        -length,-breadth, height,
        -length,-breadth,-height,
        -length, breadth, height,
        -length,-breadth, height,
        length,-breadth, height,
        length, breadth, height,
        length,-breadth,-height,
        length, breadth,-height,
        length,-breadth,-height,
        length, breadth, height,
        length,-breadth, height,
        length, breadth, height,
        length, breadth,-height,
        -length, breadth,-height,
        length, breadth, height,
        -length, breadth,-height,
        -length, breadth, height,
        length, breadth, height,
        -length, breadth, height,
        length,-breadth, height
  };
  this->cuboid = create3DObject(GL_TRIANGLES,12*3,vertex_buffer_data,color,GL_FILL);
}

void Cuboid::draw(glm::mat4 VP) {
  Matrices.model = glm::mat4(1.0f);
  glm::mat4 translate = glm::translate (this->position);    // glTranslatef
  glm::mat4 rotate    = glm::rotate((float) (0 * M_PI / 180.0f), glm::vec3(1, 0, 0));
  // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
  // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
  Matrices.model *= (translate * rotate);
  glm::mat4 MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  draw3DObject(this->cuboid);
}


void Cuboid::set_acceleration(glm::vec3 a) {
  this->acceleration = a;
}

void Cuboid::set_velocity(glm::vec3 v) {
  this->velocity = v;
}

void Cuboid::tick() {
  float dt = 0.01;
  this->velocity += dt*this->acceleration;
  this->position += dt*this->velocity;
}

void Cuboid::draw(glm::mat4 VP,glm::mat4 rotation_matrix) {
  Matrices.model = glm::mat4(1.0f);
  glm::mat4 translate = glm::translate (this->position);
  Matrices.model *= (translate * rotation_matrix);
  glm::mat4 MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  draw3DObject(this->cuboid);
}
