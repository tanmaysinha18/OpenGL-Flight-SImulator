#include "parachute.h"

Parachute::Parachute(float x,float y,float z) {
  this->position = glm::vec3(x,y,z);
  this->velocity = glm::vec3(0,0,0);
  this->acceleration = glm::vec3(0,-10,0);
  exists = false;
  GLfloat cylinder_buffer_data[4000];
  int N=6;
  int j=0;
  float cylinder_radius = 15;
  float cylinder_length = 6;
  for(int i=0;i<2*N;i+=2) {
    cylinder_buffer_data[j] = cylinder_radius*cos((i*M_PI)/N);
    cylinder_buffer_data[j + 2] = cylinder_radius*sin((i*M_PI)/N);
    cylinder_buffer_data[j + 1] = 0;

    cylinder_buffer_data[j + 3] = 0;
    cylinder_buffer_data[j + 5] = 0;
    cylinder_buffer_data[j + 4] = cylinder_length;

    cylinder_buffer_data[j + 6] = cylinder_radius*cos(((i+2)*M_PI)/N);
    cylinder_buffer_data[j + 8] = cylinder_radius*sin(((i+2)*M_PI)/N);
    cylinder_buffer_data[j + 7] = 0;
      
    j += 9;
  }

  this->parachute = create3DObject(GL_TRIANGLES,N*3,cylinder_buffer_data,(color_t){255,255,0},GL_FILL);
  dist_human = 15;
  human_parts.push_back(Cuboid(position.x,position.y - dist_human,position.z,3,5,3,(color_t){252,222,173}));
  human_parts.push_back(Cuboid(position.x + 3,position.y - 0.75*dist_human,position.z,3,1,4,(color_t){252,222,173}));
  human_parts.push_back(Cuboid(position.x - 3,position.y - 0.75*dist_human,position.z,3,1,4,(color_t){252,222,173}));
  human_parts.push_back(Cuboid(position.x,position.y - 0.6*dist_human,position.z,1,1,1,(color_t){0,0,0}));
  human_parts.push_back(Cuboid(position.x - 2,position.y - 1.6*dist_human,position.z,0.5,4,1.5,(color_t){0,0,200}));
  human_parts.push_back(Cuboid(position.x + 2,position.y - 1.6*dist_human,position.z,0.5,4,1.5,(color_t){0,0,200}));

  for(int i=0;i<human_parts.size();i++) {
    human_parts[i].set_velocity(this->velocity);
    human_parts[i].set_acceleration(this->acceleration);
  }

}

void Parachute::draw(glm::mat4 VP) {
  glm::mat4 VP2 = VP;
  for(int i=0;i<human_parts.size();i++) {
    human_parts[i].draw(VP2);
  }
  Matrices.model = glm::mat4(1.0f);
  glm::mat4 translate = glm::translate (this->position);    // glTranslatef
  glm::mat4 rotate    = glm::rotate((float) (0 * M_PI / 180.0f), glm::vec3(1, 0, 0));
  Matrices.model *= (translate * rotate);
  
  glm::mat4 MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  draw3DObject(this->parachute);
}

void Parachute::tick() {
  for(int i=0;i<human_parts.size();i++) {
    human_parts[i].tick();
  }
  float dt = 0.01;
  this->velocity += dt*this->acceleration;
  this->position += dt*this->velocity;
}