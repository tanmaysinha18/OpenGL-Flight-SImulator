#include "missile.h"

Missile::Missile(float x,float y,float z,glm::mat4 rotation) {
  this->position = glm::vec3(x,y,z);
  this->acceleration = glm::vec3(0,-10,0); 
  this->velocity = 200.0f*glm::vec3(rotation[2][0],rotation[2][1],rotation[2][2]);
  this->rotation_matrix = rotation;
  exists = true;
  GLfloat cylinder_buffer_data[10000];
  int N=100;
  int j=0;
  cylinder_radius = 0.5;
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
    cylinder_buffer_data[j + 2] = cylinder_length;

    cylinder_buffer_data[j + 3] = (cylinder_radius/2)*cos(((i+1)*M_PI)/N);
    cylinder_buffer_data[j + 4] = (cylinder_radius/2)*sin(((i+1)*M_PI)/N);
    cylinder_buffer_data[j + 5] = (1.2)*cylinder_length;

    cylinder_buffer_data[j + 6] = cylinder_radius*cos(((i+2)*M_PI)/N);
    cylinder_buffer_data[j + 7] = cylinder_radius*sin(((i+2)*M_PI)/N);
    cylinder_buffer_data[j + 8] = cylinder_length;
      
    j += 9;
  }
  for(int i=0;i<2*N;i+=2) {
    cylinder_buffer_data[j] = (cylinder_radius/2)*cos(((i+1)*M_PI)/N);
    cylinder_buffer_data[j + 1] = (cylinder_radius/2)*sin(((i+1)*M_PI)/N);
    cylinder_buffer_data[j + 2] = (1.2)*cylinder_length;

    cylinder_buffer_data[j + 3] = cylinder_radius*cos(((i+2)*M_PI)/N);
    cylinder_buffer_data[j + 4] = cylinder_radius*sin(((i+2)*M_PI)/N);
    cylinder_buffer_data[j + 5] = cylinder_length;

    cylinder_buffer_data[j + 6] = (cylinder_radius/2)*cos(((i+3)*M_PI)/N);
    cylinder_buffer_data[j + 7] = (cylinder_radius/2)*sin(((i+3)*M_PI)/N);
    cylinder_buffer_data[j + 8] = (1.2)*cylinder_length;
      
    j += 9;
  }
  for(int i=0;i<2*N;i+=2) {
    cylinder_buffer_data[j] = 0;
    cylinder_buffer_data[j + 1] = 0;
    cylinder_buffer_data[j + 2] = 1.4*cylinder_length;

    cylinder_buffer_data[j + 3] = (cylinder_radius/2)*cos(((i+1)*M_PI)/N);
    cylinder_buffer_data[j + 4] = (cylinder_radius/2)*sin(((i+1)*M_PI)/N);
    cylinder_buffer_data[j + 5] = (1.2)*cylinder_length;

    cylinder_buffer_data[j + 6] = 0;
    cylinder_buffer_data[j + 7] = 0;
    cylinder_buffer_data[j + 8] = 1.4*cylinder_length;
      
    j += 9;
  }

  this->cylinder = create3DObject(GL_TRIANGLES,5*N*3,cylinder_buffer_data,(color_t){88, 91, 98},GL_FILL);  
 
  GLfloat fin_buffer_data[] ={
    0,cylinder_radius,-0.3*cylinder_length,
    0,cylinder_radius,-0.9*cylinder_length,
    0,2*cylinder_radius,-cylinder_length,

    0,-cylinder_radius,-0.3*cylinder_length,
    0,-cylinder_radius,-0.9*cylinder_length,
    0,-2*cylinder_radius,-cylinder_length,

    cylinder_radius,0,-0.3*cylinder_length,
    cylinder_radius,0,-0.9*cylinder_length,
    2*cylinder_radius,0,-cylinder_length,

    -cylinder_radius,0,-0.3*cylinder_length,
    -cylinder_radius,0,-0.9*cylinder_length,
    -2*cylinder_radius,0,-cylinder_length,

  };
  this->fins = create3DObject(GL_TRIANGLES,4*3,fin_buffer_data,(color_t){255,255,255},GL_FILL);  

}

void Missile::tick() {
  float dt = 0.01;
  this->velocity += this->acceleration*dt;
  this->position += this->velocity*dt;
}

void Missile::draw(glm::mat4 VP) {
  Matrices.model = glm::mat4(1.0f);
  glm::mat4 translate = glm::translate (this->position);
  Matrices.model *= (translate * this->rotation_matrix);
  glm::mat4 MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  draw3DObject(this->cylinder);
  draw3DObject(this->fins);
}