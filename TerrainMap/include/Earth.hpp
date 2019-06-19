#ifndef _EARTH_HPP_
#define _EARTH_HPP_

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
private:
    std::vector<GLfloat> vbData;
    std::vector<GLuint> ibData;
    GLuint vertexBuffer;
    GLuint indexBuffer;

    int numLongs;
    int numLats;

public:
    Earth();

    void draw(GLuint pID, mat4 worldToCamera);
};

#endif
