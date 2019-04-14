#include "cannon.h"

Cannon::Cannon(float x,float y,float z) {
  exists = true;
  this->position = glm::vec3(x,y,z);
  this->rotation_matrix = glm::mat4(1.0f);
  time_since_fire = time(NULL);
  GLfloat cylinder_buffer_data[4000];
  int N=100;
  int j=0;
  cylinder_radius = 5;
  for(int i=0;i<2*N;i+=2) {
    cylinder_buffer_data[j] = cylinder_radius*cos((i*M_PI)/N);
    cylinder_buffer_data[j + 1] = cylinder_radius*sin((i*M_PI)/N);
    cylinder_buffer_data[j + 2] = cylinder_length;

    cylinder_buffer_data[j + 3] = cylinder_radius*cos(((i+1)*M_PI)/N);
    cylinder_buffer_data[j + 4] = cylinder_radius*sin(((i+1)*M_PI)/N);
    cylinder_buffer_data[j + 5] = -cylinder_length;

    cylinder_buffer_data[j + 6] = cylinder_radius*cos(((i+2)*M_PI)/N);
    cylinder_buffer_data[j + 7] = cylinder_radius*sin(((i+2)*M_PI)/N);
    cylinder_buffer_data[j + 8] = cylinder_length;
      
    j += 9;
  }
  for(int i=0;i<2*N;i+=2) {
    cylinder_buffer_data[j] = cylinder_radius*cos(((i+1)*M_PI)/N);
    cylinder_buffer_data[j + 1] = cylinder_radius*sin(((i+1)*M_PI)/N);
    cylinder_buffer_data[j + 2] = -cylinder_length;

    cylinder_buffer_data[j + 3] = cylinder_radius*cos(((i+2)*M_PI)/N);
    cylinder_buffer_data[j + 4] = cylinder_radius*sin(((i+2)*M_PI)/N);
    cylinder_buffer_data[j + 5] = cylinder_length;

    cylinder_buffer_data[j + 6] = cylinder_radius*cos(((i+3)*M_PI)/N);
    cylinder_buffer_data[j + 7] = cylinder_radius*sin(((i+3)*M_PI)/N);
    cylinder_buffer_data[j + 8] = -cylinder_length;
      
    j += 9;
  }
  for(int i=0;i<2*N;i+=2) {
    cylinder_buffer_data[j] = cylinder_radius*cos((i*M_PI)/N);
    cylinder_buffer_data[j + 1] = cylinder_radius*sin((i*M_PI)/N);
    cylinder_buffer_data[j + 2] = -cylinder_length;

    cylinder_buffer_data[j + 3] = (cylinder_radius/2)*cos(((i+1)*M_PI)/N);
    cylinder_buffer_data[j + 4] = (cylinder_radius/2)*sin(((i+1)*M_PI)/N);
    cylinder_buffer_data[j + 5] = -(1.2)*cylinder_length;

    cylinder_buffer_data[j + 6] = cylinder_radius*cos(((i+2)*M_PI)/N);
    cylinder_buffer_data[j + 7] = cylinder_radius*sin(((i+2)*M_PI)/N);
    cylinder_buffer_data[j + 8] = -cylinder_length;
      
    j += 9;
  }
  for(int i=0;i<2*N;i+=2) {
    cylinder_buffer_data[j] = (cylinder_radius/2)*cos(((i+1)*M_PI)/N);
    cylinder_buffer_data[j + 1] = (cylinder_radius/2)*sin(((i+1)*M_PI)/N);
    cylinder_buffer_data[j + 2] = -(1.2)*cylinder_length;

    cylinder_buffer_data[j + 3] = cylinder_radius*cos(((i+2)*M_PI)/N);
    cylinder_buffer_data[j + 4] = cylinder_radius*sin(((i+2)*M_PI)/N);
    cylinder_buffer_data[j + 5] = -cylinder_length;

    cylinder_buffer_data[j + 6] = (cylinder_radius/2)*cos(((i+3)*M_PI)/N);
    cylinder_buffer_data[j + 7] = (cylinder_radius/2)*sin(((i+3)*M_PI)/N);
    cylinder_buffer_data[j + 8] = -(1.2)*cylinder_length;
      
    j += 9;
  }
  this->cylinder = create3DObject(GL_TRIANGLES,4*N*3,cylinder_buffer_data,(color_t){88, 91, 98},GL_FILL);  
}

void Cannon::draw(glm::mat4 VP) {
  Matrices.model = glm::mat4(1.0f);
  glm::mat4 translate = glm::translate (this->position);    // glTranslatef
  Matrices.model *= (translate * this->rotation_matrix);
  glm::mat4 MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  draw3DObject(this->cylinder);
}

void Cannon::tick(glm::vec3 r) {
  //r gives point where cannon points.
  glm::vec3 c3 = glm::normalize(r - this->position);
  glm::vec3 e1(0,1,0);
  glm::vec3 c2 = glm::normalize(glm::cross(e1,c3));
  if(glm::length(c2) < 0.01f) {
    e1 = glm::vec3(1,1,1);
    c2 = glm::normalize(glm::cross(e1,c3));
  }
  glm::vec3 c1 = glm::normalize(glm::cross(c3,c2));

  for(int i=0;i<3;i++) {
    this->rotation_matrix[0][i] = c1[i];
    this->rotation_matrix[1][i] = c2[i];
    this->rotation_matrix[2][i] = c3[i];
  }
}

bool Cannon::to_fire() {
  if(time_since_fire + 10 < time(NULL)) {
    time_since_fire = time(NULL);
    return true;
  }
  else {
    return false;
  }
}