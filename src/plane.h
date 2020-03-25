#include "main.h"

#ifndef PLANE_H
#define PLANE_H


class Plane {
public:
    Plane() {}
    Plane(float x, float y,float z, color_t color);
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float camera_height;
    float length;
    float roll_y;
    float roll_z;
    float roll_x;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    glm::vec3 get_position();
    static constexpr float Stokes_constant = 0.05;
    void tick();
    //static const float acceleration_constant = 30.0f;
    int camera_flag;
    glm::vec3 forward_direction;
    glm::vec3 upward_direction;
    glm::mat4 rotation_matrix;
    glm::vec3 camera_view_eye(GLFWwindow *window);
    glm::vec3 camera_view_target();
    glm::vec3 camera_view_up();
    bool missile_reloaded;
    time_t time_since_fire;
    time_t fired_bullets;
    float fuel;
    int score;
    static constexpr float max_fuel = 10000;
    bool to_fire();
    void change_camera(GLFWwindow *window);
    void tick_input(GLFWwindow *window);
    bool is_thrusting;
    void ROTATE_X(float angle);
    void ROTATE_Y(float angle);
    void ROTATE_GLOBAL_Y(float angle);
    void ROTATE_GLOBAL_Z(float angle);
    void ROTATE_Z(float angle);
    double speed;
    int mouse_input(GLFWwindow *window);
private:
    VAO *wings;
    VAO *body;
    VAO *tail_wings;
    VAO *rudder;
    VAO *thrusters;
    VAO *thruster_fire;
    VAO *thruster_flame;
    VAO *nose;
    VAO *nose_cap;
    VAO *cockpit;
};

#endif