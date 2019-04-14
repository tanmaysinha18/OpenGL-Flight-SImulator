#include "compass.h"

Compass::Compass(float x,float y,float z) {
  this->position = glm::vec3(x,y,z);

  GLfloat compass_buffer_data[] = {
    0,0,0,
    -0.5,0,-3,
    0.5,0,-3,
  };

  this->compass = create3DObject(GL_TRIANGLES,1*3,compass_buffer_data,(color_t){255,0,0},GL_FILL);
}

void Compass::draw(glm::mat4 VP,glm::mat4 rotate) {
  Matrices.model = glm::mat4(1.0f);
  glm::mat4 translate = glm::translate (this->position);    // glTranslatef
  // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
  // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
  Matrices.model *= (translate * rotate);
  glm::mat4 MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  draw3DObject(this->compass);
}
