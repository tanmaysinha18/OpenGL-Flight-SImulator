#include "plane.h"
#include "main.h"

Plane::Plane(float x, float y,float z, color_t color) {
    this->fired_bullets = time(NULL);
    this->position = glm::vec3(x, y, 0);
    this->velocity = glm::vec3(0, 0, 0);
    this->acceleration = glm::vec3(0, 0, 0);
    this->roll_x = 0;
    this->roll_y = 0;
    this->roll_z = 0;
    this->camera_height = 5;
    this->is_thrusting = false;
    this->rotation_matrix = glm::mat4(1.0f);
    this->missile_reloaded = true;
    this->time_since_fire = time(NULL) - 10;
    this->fuel = max_fuel;
    this->score = 0;
    forward_direction = glm::vec3(0,0,-1);
    camera_flag = 1;
    speed = 1;
    length = 1;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices

    float fin_length = 2;
    float wingspan = 6;
    GLfloat wings_buffer_data[] = {
      -wingspan,0,-fin_length/2,
      0,0,0,
      0,0,fin_length,

      wingspan,0,-fin_length/2,
      0,0,0,
      0,0,fin_length,
    };

    this->wings = create3DObject(GL_TRIANGLES,2*3,wings_buffer_data,(color_t){100,100,100},GL_FILL);
    
    float body_length = 3;
    float body_radius = 1;
    GLfloat body_buffer_data[5000];
    
    int N=100;
    int j=0;
    
    for(int i=0;i<2*N;i+=2) {
      body_buffer_data[j] = body_radius*cos((i*M_PI)/N);
      body_buffer_data[j + 1] = body_radius*sin((i*M_PI)/N);
      body_buffer_data[j + 2] = body_length;

      body_buffer_data[j + 3] = body_radius*cos(((i+1)*M_PI)/N);
      body_buffer_data[j + 4] = body_radius*sin(((i+1)*M_PI)/N);
      body_buffer_data[j + 5] = -body_length;

      body_buffer_data[j + 6] = body_radius*cos(((i+2)*M_PI)/N);
      body_buffer_data[j + 7] = body_radius*sin(((i+2)*M_PI)/N);
      body_buffer_data[j + 8] = body_length;
      
      j += 9;
    }
    for(int i=0;i<2*N;i+=2) {
      body_buffer_data[j] = body_radius*cos(((i+1)*M_PI)/N);
      body_buffer_data[j + 1] = body_radius*sin(((i+1)*M_PI)/N);
      body_buffer_data[j + 2] = -body_length;

      body_buffer_data[j + 3] = body_radius*cos(((i+2)*M_PI)/N);
      body_buffer_data[j + 4] = body_radius*sin(((i+2)*M_PI)/N);
      body_buffer_data[j + 5] = body_length;

      body_buffer_data[j + 6] = body_radius*cos(((i+3)*M_PI)/N);
      body_buffer_data[j + 7] = body_radius*sin(((i+3)*M_PI)/N);
      body_buffer_data[j + 8] = -body_length;
      
      j += 9;
    }

    this->body = create3DObject(GL_TRIANGLES,2*N*3,body_buffer_data,(color_t){200,200,200},GL_FILL);

    float tail_fin_length = 1;
    float tail_wingspan = 3;
    GLfloat tail_wings_buffer_data[] = {
      -tail_wingspan,0,-(tail_fin_length/2)-(body_length),
      0,0,-body_length,
      0,0,tail_fin_length-body_length,

      tail_wingspan,0,-(tail_fin_length/2)-(body_length),
      0,0,-body_length,
      0,0,tail_fin_length-body_length,
    };

    this->tail_wings = create3DObject(GL_TRIANGLES,2*3,tail_wings_buffer_data,(color_t){100,100,100},GL_FILL);

    float rudder_height = 2;
    float rudder_length = 3;
    float rudder_seperation = 1;
    GLfloat rudder_buffer_data[] = {
      rudder_seperation,0,-body_length,
      rudder_seperation,rudder_height,-body_length,
      rudder_seperation,0,rudder_length-body_length,
      
      -rudder_seperation,0,-body_length,
      -rudder_seperation,rudder_height,-body_length,
      -rudder_seperation,0,rudder_length-body_length,
    };

    this->rudder = create3DObject(GL_TRIANGLES,2*3,rudder_buffer_data,(color_t){100,100,100},GL_FILL);

    GLfloat thrusters_buffer_data[1000];
    j=0;
    float thruster_length = 1;
    for(int i=0;i<2*N;i+=2) {
      thrusters_buffer_data[j] = body_radius*cos((i*M_PI)/N);
      thrusters_buffer_data[j + 1] = body_radius*sin((i*M_PI)/N);
      thrusters_buffer_data[j + 2] = -body_length;

      thrusters_buffer_data[j + 3] = 0.5*body_radius*cos(((i+1)*M_PI)/N);
      thrusters_buffer_data[j + 4] = 0.5*body_radius*sin(((i+1)*M_PI)/N);
      thrusters_buffer_data[j + 5] = -body_length-thruster_length;

      thrusters_buffer_data[j + 6] = body_radius*cos(((i+2)*M_PI)/N);
      thrusters_buffer_data[j + 7] = body_radius*sin(((i+2)*M_PI)/N);
      thrusters_buffer_data[j + 8] = -body_length;
      
      j += 9;
    }
    for(int i=0;i<2*N;i+=2) {
      thrusters_buffer_data[j] = 0.5*body_radius*cos(((i+1)*M_PI)/N);
      thrusters_buffer_data[j + 1] = 0.5*body_radius*sin(((i+1)*M_PI)/N);
      thrusters_buffer_data[j + 2] = -body_length-thruster_length;

      thrusters_buffer_data[j + 3] = body_radius*cos(((i+2)*M_PI)/N);
      thrusters_buffer_data[j + 4] = body_radius*sin(((i+2)*M_PI)/N);
      thrusters_buffer_data[j + 5] = -body_length;

      thrusters_buffer_data[j + 6] = 0.5*body_radius*cos(((i+3)*M_PI)/N);
      thrusters_buffer_data[j + 7] = 0.5*body_radius*sin(((i+3)*M_PI)/N);
      thrusters_buffer_data[j + 8] = -body_length-thruster_length;
      
      j += 9;
    }

    this->thrusters = create3DObject(GL_TRIANGLES,2*N*3,thrusters_buffer_data,(color_t){50,50,50},GL_FILL);
    j = 0;
    GLfloat thruster_fire_buffer_data[1000];
    float thruster_fire_length = 1;
    for(int i=0;i<2*N;i+=2) {
      thruster_fire_buffer_data[j] = 0.5*body_radius*cos((i*M_PI)/N);
      thruster_fire_buffer_data[j + 1] = 0.5*body_radius*sin((i*M_PI)/N);
      thruster_fire_buffer_data[j + 2] = -body_length-thruster_length;

      thruster_fire_buffer_data[j + 3] = 0.25*body_radius*cos((i*M_PI)/N);
      thruster_fire_buffer_data[j + 4] = 0.25*body_radius*sin((i*M_PI)/N);
      thruster_fire_buffer_data[j + 5] = -body_length-thruster_length-thruster_fire_length;

      thruster_fire_buffer_data[j + 6] = 0.5*body_radius*cos(((i+2)*M_PI)/N);
      thruster_fire_buffer_data[j + 7] = 0.5*body_radius*sin(((i+2)*M_PI)/N);
      thruster_fire_buffer_data[j + 8] = -body_length - thruster_length;
      
      j += 9;
    }
    this->thruster_fire = create3DObject(GL_TRIANGLES,N*3,thruster_fire_buffer_data,(color_t){255,255,0},GL_FILL);
    j = 0;
    for(int i=0;i<2*N;i+=2) {
      thruster_fire_buffer_data[j] = 0.4*body_radius*cos((i*M_PI)/N);
      thruster_fire_buffer_data[j + 1] = 0.4*body_radius*sin((i*M_PI)/N);
      thruster_fire_buffer_data[j + 2] = -body_length-thruster_length;

      thruster_fire_buffer_data[j + 3] = 0.1*body_radius*cos((i*M_PI)/N);
      thruster_fire_buffer_data[j + 4] = 0.1*body_radius*sin((i*M_PI)/N);
      thruster_fire_buffer_data[j + 5] = -body_length-thruster_length-1.5*thruster_fire_length;

      thruster_fire_buffer_data[j + 6] = 0.4*body_radius*cos(((i+2)*M_PI)/N);
      thruster_fire_buffer_data[j + 7] = 0.4*body_radius*sin(((i+2)*M_PI)/N);
      thruster_fire_buffer_data[j + 8] = -body_length - thruster_length;
      
      j += 9;
    }
    this->thruster_flame = create3DObject(GL_TRIANGLES,N*3,thruster_fire_buffer_data,(color_t){255,0,0},GL_FILL);

    GLfloat nose_buffer_data[1000];
    j = 0;
    float nose_length =2;
    float nose_cap_length = 1;

    for(int i=0;i<2*N;i+=2) {
      nose_buffer_data[j] = body_radius*cos((i*M_PI)/N);
      nose_buffer_data[j + 1] = body_radius*sin((i*M_PI)/N);
      nose_buffer_data[j + 2] = body_length;

      nose_buffer_data[j + 3] = 0.5*body_radius*cos(((i+1)*M_PI)/N);
      nose_buffer_data[j + 4] = 0.5*body_radius*sin(((i+1)*M_PI)/N);
      nose_buffer_data[j + 5] = body_length+nose_length;

      nose_buffer_data[j + 6] = body_radius*cos(((i+2)*M_PI)/N);
      nose_buffer_data[j + 7] = body_radius*sin(((i+2)*M_PI)/N);
      nose_buffer_data[j + 8] = body_length;
      
      j += 9;
    }
    for(int i=0;i<2*N;i+=2) {
      nose_buffer_data[j] = 0.5*body_radius*cos(((i+1)*M_PI)/N);
      nose_buffer_data[j + 1] = 0.5*body_radius*sin(((i+1)*M_PI)/N);
      nose_buffer_data[j + 2] = body_length+nose_length;

      nose_buffer_data[j + 3] = body_radius*cos(((i+2)*M_PI)/N);
      nose_buffer_data[j + 4] = body_radius*sin(((i+2)*M_PI)/N);
      nose_buffer_data[j + 5] = body_length;

      nose_buffer_data[j + 6] = 0.5*body_radius*cos(((i+3)*M_PI)/N);
      nose_buffer_data[j + 7] = 0.5*body_radius*sin(((i+3)*M_PI)/N);
      nose_buffer_data[j + 8] = body_length+nose_length;
      
      j += 9;
    }
    this->nose = create3DObject(GL_TRIANGLES,2*N*3,nose_buffer_data,(color_t){200,200,200},GL_FILL);
    j=0;
    for(int i=0;i<2*N;i+=2) { 
      nose_buffer_data[j] = 0.5*body_radius*cos((i*M_PI)/N);
      nose_buffer_data[j + 1] = 0.5*body_radius*sin((i*M_PI)/N);
      nose_buffer_data[j + 2] = body_length+nose_length;

      nose_buffer_data[j + 3] = 0;
      nose_buffer_data[j + 4] = 0;
      nose_buffer_data[j + 5] = body_length+nose_length+nose_cap_length;

      nose_buffer_data[j + 6] = 0.5*body_radius*cos(((i+2)*M_PI)/N);
      nose_buffer_data[j + 7] = 0.5*body_radius*sin(((i+2)*M_PI)/N);
      nose_buffer_data[j + 8] = body_length+nose_length;
      
      j += 9;
    }
    this->nose_cap = create3DObject(GL_TRIANGLES,N*3,nose_buffer_data,(color_t){0,0,0},GL_FILL);
    
    GLfloat cockpit_buffer_data[1000];
    float start_window = body_length + 0.2*nose_length;
    float start_radius = 0.91*body_radius;
    float end_radius = 0.66*body_radius;
    float start_phase = M_PI/6;
    float end_window = body_length + 0.7*nose_length;
    j=0;
    for(int i=0;i<2*N;i+=2) {
      cockpit_buffer_data[j] = start_radius*cos(start_phase+(i*M_PI)/(3*N));
      cockpit_buffer_data[j + 1] = start_radius*sin(start_phase+(i*M_PI)/(3*N));
      cockpit_buffer_data[j + 2] = start_window;

      cockpit_buffer_data[j + 3] = end_radius*cos(start_phase+((i+1)*M_PI)/(3*N));
      cockpit_buffer_data[j + 4] = end_radius*sin(start_phase+((i+1)*M_PI)/(3*N));
      cockpit_buffer_data[j + 5] = end_window;

      cockpit_buffer_data[j + 6] = start_radius*cos(start_phase+((i+2)*M_PI)/(3*N));
      cockpit_buffer_data[j + 7] = start_radius*sin(start_phase+((i+2)*M_PI)/(3*N));
      cockpit_buffer_data[j + 8] = start_window;
      
      j += 9;
    }
    for(int i=0;i<2*N;i+=2) {
      cockpit_buffer_data[j] = end_radius*cos(start_phase+((i+1)*M_PI)/(3*N));
      cockpit_buffer_data[j + 1] = end_radius*sin(start_phase+((i+1)*M_PI)/(3*N));
      cockpit_buffer_data[j + 2] = end_window;

      cockpit_buffer_data[j + 3] = start_radius*cos(start_phase+((i+2)*M_PI)/(3*N));
      cockpit_buffer_data[j + 4] = start_radius*sin(start_phase+((i+2)*M_PI)/(3*N));
      cockpit_buffer_data[j + 5] = start_window;

      cockpit_buffer_data[j + 6] = end_radius*cos(start_phase+((i+3)*M_PI)/(3*N));
      cockpit_buffer_data[j + 7] = end_radius*sin(start_phase+((i+3)*M_PI)/(3*N));
      cockpit_buffer_data[j + 8] = end_window;
      
      j += 9;
    }
    this->cockpit = create3DObject(GL_TRIANGLES,2*N*3,cockpit_buffer_data,(color_t){178,232,255},GL_FILL);

}


void Plane::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position);    // glTranslatef
    //glm::mat4 rotate1 = glm::rotate((float) (this->roll_x * M_PI / 180.0f), glm::vec3(1, 0, 0));
    //glm::mat4 rotate2 = glm::rotate((float) (this->roll_z * M_PI / 180.0f), glm::vec3(0,0,1));
    //glm::mat4 rotate3 = glm::rotate((float) (this->roll_y * M_PI / 180.0f), glm::vec3(0, 1, 0));

    glm::mat4 rotate;// = rotate3*rotate2*rotate1;

    forward_direction = glm::vec3(rotation_matrix[2][0],rotation_matrix[2][1],rotation_matrix[2][2]);
    upward_direction = glm::vec3(rotation_matrix[1][0],rotation_matrix[1][1],rotation_matrix[1][2]);
    //rotate2 = glm::rotate((float) (this->roll_z * M_PI / 180.0f), forward_direction);
    rotate = rotation_matrix;
    Matrices.model = Matrices.model*(translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->wings);
    draw3DObject(this->tail_wings);
    draw3DObject(this->body);
    draw3DObject(this->rudder);
    draw3DObject(this->thrusters);
    if(is_thrusting) {
      draw3DObject(this->thruster_fire);
      draw3DObject(this->thruster_flame);
    }
    draw3DObject(this->nose);
    draw3DObject(this->nose_cap);
    draw3DObject(this->cockpit);
}

void Plane::ROTATE_Z(float angle) {
    roll_z += angle;  
    glm::mat4 rotate_z = glm::rotate((float) (angle * M_PI / 180.0f),glm::vec3(rotation_matrix[2][0],rotation_matrix[2][1],rotation_matrix[2][2]));
    rotation_matrix = rotate_z * rotation_matrix;
}

void Plane::ROTATE_GLOBAL_Z(float angle) {
    roll_z += angle;  
    glm::mat4 rotate_z = glm::rotate((float) (angle * M_PI / 180.0f),glm::vec3(0,0,1));
    rotation_matrix = rotate_z * rotation_matrix;
}

void Plane::ROTATE_Y(float angle) {
    roll_y += angle;  
    glm::mat4 rotate_y = glm::rotate((float) (angle * M_PI / 180.0f),glm::vec3(rotation_matrix[1][0],rotation_matrix[1][1],rotation_matrix[1][2]));
    rotation_matrix = rotate_y * rotation_matrix;
}


void Plane::ROTATE_GLOBAL_Y(float angle) {
    roll_y += angle;  
    glm::mat4 rotate_y = glm::rotate((float) (angle * M_PI / 180.0f),glm::vec3(0,1,0));
    rotation_matrix = rotate_y * rotation_matrix;
}


void Plane::ROTATE_X(float angle) {
    roll_x += angle;  
    glm::mat4 rotate_x = glm::rotate((float) (angle * M_PI / 180.0f),glm::vec3(rotation_matrix[0][0],rotation_matrix[0][1],rotation_matrix[0][2]));
    rotation_matrix = rotate_x * rotation_matrix;
}


void Plane::tick_input(GLFWwindow *window) {
  int roll_right_z = glfwGetKey(window,GLFW_KEY_D);
  int roll_left_z = glfwGetKey(window,GLFW_KEY_A);
  float acceleration_constant = 75.0f;
  float Stokes_constant = 0.5;
  float angle = 2;
  if(roll_right_z) {  
      ROTATE_Z(2*angle);
    //ROTATE_Y(1);

  }

  if(roll_left_z) {
      ROTATE_Z(-2*angle);
    
    //ROTATE_Y(-1);

  }
  int roll_down_x = glfwGetKey(window,GLFW_KEY_X);
  int roll_up_x = glfwGetKey(window,GLFW_KEY_S);
  if(roll_down_x) {
    ROTATE_X(angle);
  }
  if(roll_up_x) {
    ROTATE_X(-1.0f);
  }
  int roll_left_y = glfwGetKey(window,GLFW_KEY_Q);
  int roll_right_y = glfwGetKey(window,GLFW_KEY_E);
  if(roll_right_y) {
    ROTATE_Y(-angle);
  }
  if(roll_left_y) {
    ROTATE_Y(angle);
  }
  this->acceleration = glm::vec3(0,0,0);
  //glm::vec3 forward(5*sin(this->roll_y * M_PI / 180.0f),0,5*cos(this->roll_y * M_PI / 180.0f));
  int thrusters = glfwGetKey(window,GLFW_KEY_W);
  if(thrusters) {
    is_thrusting = true;
    this->acceleration += acceleration_constant*forward_direction;
  }
  else {
    is_thrusting = false;
  }
  this->acceleration -=  this->velocity*Stokes_constant;
  change_camera(window);
}

void Plane::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Plane::tick() {
  this->fuel -= 1;
  float dt = 0.01;
  glm::vec3 forward(sin(this->roll_y * M_PI / 180.0f),0,cos(this->roll_y * M_PI / 180.0f));
  this->velocity = glm::length(this->velocity)*forward_direction;
  this->velocity += this->acceleration*dt;
  this->position += this->velocity*dt;
}

glm::vec3 Plane::camera_view_eye(GLFWwindow *window) {
  //1 -> Back view
  //2 -> First person view
  //3 -> Tower view
  //4 -> Helicopter view
  //5 -> Top down view
  //6 -> Stationary follow cam
  //return glm::vec3( 0, this->camera_height, -2*this->camera_height);
  if(camera_flag == 1) {
    return position - 15.0f*forward_direction + 5.0f*upward_direction;
    
    /*Uncomment for camera moving in an ellipse behind the plane*/
    //return glm::vec3 ((position.x-forward.x),position.y + 5,2*(position.z-forward.z));
  }
  if(camera_flag == 2) {
    return position+6.0f*forward_direction;
  }
  if(camera_flag == 3) {
    return this->position + glm::vec3(-25,25,0);
  }
  if(camera_flag == 4) {
    double xpos,ypos;
    glfwGetCursorPos(window,&xpos,&ypos);
    float angle = (360*(xpos/1365) )*M_PI/180;
    float phi = (360*((ypos+30)/737))*M_PI/180;
    return this->position - 15.0f*glm::vec3(cos(angle)*sin(phi),cos(phi),sin(angle)*sin(phi));
  }
  if(camera_flag == 5) {
    return this->position + 50.0f*upward_direction;
  }

  if(camera_flag ==6) {
    return glm::vec3( 0, this->camera_height, -2*this->camera_height);
  }
}

glm::vec3 Plane::camera_view_target() {
  //1 -> Back view
  //2 -> First person view
  //3 -> Tower view
  //4 -> Helicopter view
  //5 -> Top down view
  //6 -> Stationary follow cam

  if(camera_flag == 1) {
    return this->position;  
  }
  if(camera_flag == 2) {
    return position+15.0f*forward_direction;
  }
  if(camera_flag == 3) {
    return this->position;
  }
  if(camera_flag == 4) {
    return this->position;
  }
  if(camera_flag == 5) {
    return this->position;
  }
  if(camera_flag == 6) {
    return this->position;
  }
}
glm::vec3 Plane::camera_view_up() {
  //1 -> Back view
  //2 -> First person view
  //3 -> Tower view
  //4 -> Helicopter view
  //5 -> Top down view
  //6 -> Stationary follow cam
  //glm::vec3 up(sin(this->roll_z * M_PI / 180.0f),cos(this->roll_z * M_PI / 180.0f),0);
  //return glm::vec3(0,1,0);
  if(camera_flag == 1) { 
    return upward_direction;
    return glm::vec3(0,1,0);
  }
  if(camera_flag == 2) {
    return upward_direction;
  }
  if(camera_flag == 3) {
    return glm::vec3(0,1,0);
  }
  if(camera_flag == 4) {
    return glm::vec3(0,1,0);
  }
  if(camera_flag == 5) {
    return forward_direction;
  }
  if(camera_flag == 6) {
    return glm::vec3(0,1,0);
  }
}

void Plane::change_camera(GLFWwindow *window) {
  int change_cam_1 = glfwGetKey(window,GLFW_KEY_F1);
  if(change_cam_1) {
      camera_flag = 1;
  }
  int change_cam_2 = glfwGetKey(window,GLFW_KEY_F2);
  if(change_cam_2) {
      camera_flag = 2;
  }
  int change_cam_3 = glfwGetKey(window,GLFW_KEY_F3);
  if(change_cam_3) {
    camera_flag = 3;
  }
  int change_cam_4 = glfwGetKey(window,GLFW_KEY_F4);
  if(change_cam_4) {
    camera_flag = 4;
  }  
  int change_cam_5 = glfwGetKey(window,GLFW_KEY_F5);
  if(change_cam_5) {
    camera_flag = 5;
  }
  int change_cam_6 = glfwGetKey(window,GLFW_KEY_F6);
  if(change_cam_6) {
    camera_flag = 6;
  }
}

glm::vec3 Plane::get_position() {
  return this->position;
}

bool Plane::to_fire() {
  if(time(NULL) - 3 > time_since_fire) {
    time_since_fire = time(NULL);
    return true;
  }
  return false;
}

