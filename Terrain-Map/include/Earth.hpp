#ifndef EARTH_HPP_
#define EARTH_HPP_

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace glm;

class Earth
{
public:
    Earth();

    void draw(GLuint pID, mat4 worldToCamera);

private:
    void push_point(GLfloat longitude, GLfloat latitude);

    std::vector<GLfloat> vbData;
    std::vector<GLuint> ibData;
    GLuint vertexBuffer, indexBuffer;
    size_t numLongs, numLats;
};

#endif
