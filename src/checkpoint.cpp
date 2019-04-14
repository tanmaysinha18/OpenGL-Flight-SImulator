#include "checkpoint.h"
#include "main.h"

Checkpoint::Checkpoint(float x,float y,float z,float radius) {
  this->position = glm::vec3(x,y,z);
  active = true;
  cylinder_radius = radius;
  ring_radius = radius;
  GLfloat cylinder_buffer_data[1000];

  int N=100;
  int j=0;
  color_t active_color = {255,0,0};
  color_t inactive_color = {200,0,0};

/*  for(int i=0;i<2*N;i+=2) {
    cylinder_buffer_data[j] = cylinder_radius*cos((i*M_PI)/N);
    cylinder_buffer_data[j + 2] = cylinder_radius*sin((i*M_PI)/N);
    cylinder_buffer_data[j + 1] = -ring_radius;

    cylinder_buffer_data[j + 3] = cylinder_radius*cos(((i+1)*M_PI)/N);
    cylinder_buffer_data[j + 5] = cylinder_radius*sin(((i+1)*M_PI)/N);
    cylinder_buffer_data[j + 4] = -cylinder_length;

    cylinder_buffer_data[j + 6] = cylinder_radius*cos(((i+2)*M_PI)/N);
    cylinder_buffer_data[j + 8] = cylinder_radius*sin(((i+2)*M_PI)/N);
    cylinder_buffer_data[j + 7] = -ring_radius;
      
    j += 9;
  }
*/  for(int i=0;i<2*N;i+=2) {
    cylinder_buffer_data[j] = cylinder_radius*cos((i*M_PI)/N);
    cylinder_buffer_data[j + 2] = cylinder_radius*sin((i*M_PI)/N);
    cylinder_buffer_data[j + 1] = ring_radius;

    cylinder_buffer_data[j + 3] = cylinder_radius*cos(((i+1)*M_PI)/N);
    cylinder_buffer_data[j + 5] = cylinder_radius*sin(((i+1)*M_PI)/N);
    cylinder_buffer_data[j + 4] = cylinder_length;

    cylinder_buffer_data[j + 6] = cylinder_radius*cos(((i+2)*M_PI)/N);
    cylinder_buffer_data[j + 8] = cylinder_radius*sin(((i+2)*M_PI)/N);
    cylinder_buffer_data[j + 7] = ring_radius;
      
    j += 9;
  }

  this->cylinder = create3DObject(GL_TRIANGLES,(N*3),cylinder_buffer_data,active_color,GL_FILL);
  j=0;
  GLfloat ring_buffer_data[2000];
  for(int i=0;i<2*N;i+=2) {
    ring_buffer_data[j] = ring_radius*cos((i*M_PI)/N);
    ring_buffer_data[j + 1] = ring_radius*sin((i*M_PI)/N);
    ring_buffer_data[j + 2] = ring_length/2;

    ring_buffer_data[j + 3] = ring_radius*cos(((i+1)*M_PI)/N);
    ring_buffer_data[j + 4] = ring_radius*sin(((i+1)*M_PI)/N);
    ring_buffer_data[j + 5] = -ring_length/2;

    ring_buffer_data[j + 6] = ring_radius*cos(((i+2)*M_PI)/N);
    ring_buffer_data[j + 7] = ring_radius*sin(((i+2)*M_PI)/N);
    ring_buffer_data[j + 8] = ring_length/2;
      
    j += 9;
  }
  for(int i=0;i<2*N;i+=2) {
    ring_buffer_data[j] = ring_radius*cos(((i+1)*M_PI)/N);
    ring_buffer_data[j + 1] = ring_radius*sin(((i+1)*M_PI)/N);
    ring_buffer_data[j + 2] = -ring_length/2;

    ring_buffer_data[j + 3] = ring_radius*cos(((i+2)*M_PI)/N);
    ring_buffer_data[j + 4] = ring_radius*sin(((i+2)*M_PI)/N);
    ring_buffer_data[j + 5] = ring_length/2;

    ring_buffer_data[j + 6] = ring_radius*cos(((i+3)*M_PI)/N);
    ring_buffer_data[j + 7] = ring_radius*sin(((i+3)*M_PI)/N);
    ring_buffer_data[j + 8] = -ring_length/2;
      
    j += 9;
  }

  this->ring = create3DObject(GL_TRIANGLES,(2*N*3),ring_buffer_data,active_color,GL_FILL);
}

void Checkpoint::draw(glm::mat4 VP) {
  Matrices.model = glm::mat4(1.0f);
  glm::mat4 translate = glm::translate (this->position);    // glTranslatef
  glm::mat4 rotate    = glm::rotate((float) (0 * M_PI / 180.0f), glm::vec3(1, 0, 0));
  // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
  // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
  Matrices.model *= (translate * rotate);
  glm::mat4 MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  if(active) {  
    draw3DObject(this->cylinder); 
    draw3DObject(this->ring);
  }
}

bool Checkpoint::check_passing(glm::vec3 pos){
  bool to_return;
  if(pos.z > this->position.z-ring_length && pos.z < this->position.z + ring_length) {
    float dist_x = pos.x - this->position.x;
    float dist_y = pos.y - this->position.y;
    float dist = dist_x*dist_x + dist_y*dist_y;
    if(dist < ring_radius*ring_radius) {
      return true;
    }
  }
  return false;
}

void Checkpoint::change_activity() {
  active = false;
}

glm::vec3 Checkpoint::get_position() {
  return this->position;
}