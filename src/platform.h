#include "main.h"

#ifndef PLATFORM_H
#define PLATFORM_H


class Platform {
public:
    Platform() {}
    Platform(color_t color,float y,float length);
    float length;
    glm::vec3 position;
    void draw(glm::mat4 VP);
private:
    VAO *object;
};

#endif