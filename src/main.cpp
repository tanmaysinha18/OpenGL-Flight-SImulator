#include "main.h"
#include "timer.h"
#include "plane.h"
#include "platform.h"
#include "checkpoint.h"
#include "island.h"
#include "cuboid.h"
#include "cannon.h"
#include "missile.h"
#include "parachute.h"
#include "bullet.h"
#include "compass.h"

#include<stdlib.h>
#include<time.h>
#include<unistd.h>
using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Objects and global variables *
**************************/

Plane plane;
Platform sky;
Platform water;
Checkpoint checkpoint;
vector<Island> islands;
Cuboid cuboid;
Cannon cannon;
vector<Missile> missiles;
Missile plane_missile;
Parachute parachute;
vector<Bullet> bullets;
Cuboid altitude_meter;
Cuboid fuel_meter;
Cuboid health_meter;
Cuboid speed_o_meter;
Compass compass;


/**************************
* Global Variables *
**************************/

const float water_level = -50;
const float sky_level = 300;
const float search_area = 2000;
glm::mat4 down;


/**************************
* Customizable functions *
**************************/




float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;

Timer t60(1.0 / 60);


void draw_dashboard(glm::vec3 eye,glm::vec3 target,glm::vec3 up,glm::mat4 VP) {
    glm::vec3 r = glm::normalize(target - eye);

    glm::vec3 c3 = r;
    glm::vec3 c2 = glm::normalize(glm::cross(c3,up));
    glm::vec3 c1 = glm::normalize(glm::cross(c3,c2));
    glm::mat4 rotation_matrix = glm::mat4(1.0f);
    
    glm::vec3 position = eye + 10.0f*r - 5.0f*c2 + 5.5f*c1;
    glm::vec3 position_fuel = eye + 10.0f*r - 4.5f*c2 + 5.5f*c1;
    glm::vec3 position_speed = eye + 10.0f*r - 4.0f*c2 + 5.5f*c1;
    glm::vec3 position_compass = eye + 10.0f*r + 4.0f*c2 + 4.0f*c1;
    for(int i=0;i<3;i++) {
        rotation_matrix[0][i] = c1[i];
        rotation_matrix[1][i] = c2[i];
        rotation_matrix[2][i] = c3[i];
    }
    altitude_meter = Cuboid(position.x,position.y,position.z,(plane.position.y-water_level)/(-water_level+sky_level),0.1,0,(color_t){255,255,255});
    altitude_meter.draw(VP,rotation_matrix);

    fuel_meter = Cuboid(position_fuel.x,position_fuel.y,position_fuel.z,plane.fuel/plane.max_fuel,0.1,0,(color_t){0,0,0});
    fuel_meter.draw(VP,rotation_matrix);
    
    speed_o_meter = Cuboid(position_speed.x,position_speed.y,position_speed.z,glm::length(plane.velocity)/100,0.1,0,(color_t){255,255,0});
    speed_o_meter.draw(VP,rotation_matrix);
    
    rotation_matrix[2][0] = 0;
    rotation_matrix[2][1] = 0;
    rotation_matrix[2][2] = 1;
    compass = Compass(position_compass.x,position_compass.y,position_compass.z);
    compass.draw(VP,glm::mat4(1.0f));
}

void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye = plane.camera_view_eye(window);
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target = plane.camera_view_target();
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up = plane.camera_view_up();

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    draw_dashboard(eye,target,up,VP);
    plane.draw(VP);
    checkpoint.draw(VP);
    for(int i=0;i<islands.size();i++) {
        islands[i].draw(VP);
    }
    water.draw(VP);
    sky.draw(VP);
    //cannon.draw(VP);
    for(int i=0;i<missiles.size();i++) {
        if(missiles[i].exists) {
            missiles[i].draw(VP);
        }
    }
    if(plane_missile.exists) {
        plane_missile.draw(VP);
    }

    parachute.draw(VP);

    for(int i=0;i<bullets.size();i++) {
        if(bullets[i].exists) {
            bullets[i].draw(VP);
        }
    }
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    plane.tick_input(window);
 

    int button_1_pressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1);
    if(button_1_pressed == 1) {
        if(plane.fired_bullets  < time(NULL)) {
            plane.fired_bullets = time(NULL);
        }
            bullets.push_back(Bullet(plane.position.x,plane.position.y,plane.position.z,1000.0f*glm::normalize(plane.velocity),glm::vec3(0,-10,0)));
    }
    int button_2_pressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2);
    if(button_2_pressed == 1) {
        if(plane.to_fire()) {
            plane_missile = Missile(plane.position.x,plane.position.y,plane.position.z,down);
        }
    }
}

void checkpoint_collision() {
  if(checkpoint.check_passing(plane.get_position())) {
    glm::vec3 previous_position = checkpoint.get_position();
    plane.score += 100;
    previous_position += glm::vec3(rand()%200-100,rand()%100 - 50,rand()%200-100);
    if(previous_position.y < water_level) {
        previous_position.y = 0;
    }
    if(previous_position.y > sky_level + 10) {
        previous_position.y = 0;
    }
    checkpoint = Checkpoint(previous_position.x,previous_position.y,previous_position.z,checkpoint.ring_radius-0.25);
  }
}

void plane_detect_collision() {
    if(plane.position.y < water_level) {
        cout << "Your score is:-" << plane.score << "\n";
        quit(window);
    }
}

void generate_islands() {
    float step = 200;
    float x_ = plane.get_position().x - search_area;
    float z_ = plane.get_position().z - search_area;

    while(x_ < plane.get_position().x + search_area ) {
        z_ = plane.get_position().z - search_area;
        while(z_  < plane.get_position().z + search_area) {
            int p = rand()%10;
            if(p == 0) {
                islands.push_back(Island(x_,water_level+10,z_));
            }
            z_ += step;
        }
        x_ += step;
    }
}

void tick_elements() {
    plane.tick();
    if(plane.fuel < 0) {
        cout << "Your score is:-" << plane.score << "\n";
        quit(window);
    }
    if(glm::length(plane.position) < 100) {
        plane.fuel = plane.max_fuel;
    }
    checkpoint_collision();
    plane_detect_collision();

    float least_index = 0;
    for(int i=0;i<islands.size();i++) {
        if(glm::length(plane.position - islands[i].position) < glm::length(plane.position - islands[least_index].position)) {
            least_index = i;
        }
    }

    islands[least_index].tick(plane.position);
    if(islands[least_index].cannon.to_fire()) {
        cannon = islands[least_index].cannon;
        missiles.push_back(Missile(cannon.position.x,cannon.position.y,cannon.position.z,cannon.rotation_matrix));    
    }
    if(plane_missile.exists) {
        if(abs(islands[least_index].cannon.position.y - plane_missile.position.y) < 10) {
            if(glm::length(plane_missile.position - islands[least_index].cannon.position) < 10) {
                islands[least_index].has_cannon = false;
                plane.score += 100;
            }
        }
    }
    for(int i=0;i<missiles.size();i++) {
        if(missiles[i].exists) {  
            missiles[i].tick();
            if(missiles[i].position.y < water_level) {
                missiles[i].exists = false;
            }
            if(glm::length(plane.position - missiles[i].position) < 5) {
                cout << "Your score is:-" << plane.score << "\n";
                quit(window);
            }
        }
    }
    if(plane_missile.exists) {
        plane_missile.tick();
        if(plane_missile.position.y < water_level) {
            plane_missile.exists = false;
        }
    }

    parachute.tick();
    if(parachute.position.y < water_level) {
        parachute = Parachute(rand()%1000,rand()%200,rand()%1000);
    }
    for(int i=0;i<bullets.size();i++) {
        if(bullets[i].exists) {
            bullets[i].tick();
            if(bullets[i].bullet.position.y < water_level) {
                bullets[i].exists = false;
            }
            if(glm::length(parachute.position - bullets[i].bullet.position) < 50) {
                parachute = Parachute(rand()%1000,rand()%200,rand()%1000);
                bullets[i].exists = false;
                plane.score += 100;
            }
        }
    }
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    down[2][0] = 0;
    down[2][1] = -1;
    down[2][2] = 0;

    down[1][0] = 0;
    down[1][1] = 0;
    down[1][2] = 1;

    srand(time(NULL));
    plane = Plane(0,0,0, COLOR_RED);
    water = Platform((color_t){0,0,255},water_level+1,10000000.0f);
    sky = Platform((color_t){135,206,235},sky_level,10000000.0f);
    checkpoint = (Checkpoint(0,0,100,10));
    islands.push_back(Island(0,water_level+10,0));
    plane_missile = Missile(0,0,0,down);
    plane_missile.exists = false;    
    generate_islands();        
    cannon = Cannon(0,50,0);
    parachute = (Parachute(50,50,50));
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;

    window = initGLFW(width, height);

    initGL (window, width, height);
    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_input(window);
            tick_elements();
            draw();
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
//        usleep(16.66);
    }

    cout << "Your score is:-" << plane.score << "\n";
    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    int width  = 600;
    int height = 600;

    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    //Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
    Matrices.projection = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f,150000.0f);
}
