#ifndef GRAPHIC_OBJECT_HPP
#define GRAPHIC_OBJECT_HPP

#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace glm;

class GraphicObject
{
private:
    std::vector<vec3> vbData;
    std::vector<vec2> tbData;
    std::vector<vec3> nbData;
    GLuint vertexBuffer;
    GLuint textureBuffer;
    GLuint normalBuffer;

public:
    GraphicObject()
    {
    }

    void setVertexBuffer(const std::vector<vec3> & data)
    {
        vbData = data;
    }

    void setTextureBuffer(const std::vector<vec2> & data)
    {
        tbData = data;
    }

    void setNormalBuffer(const std::vector<vec3> & data)
    {
        nbData = data;
    }

    void createBuffers();
    void draw(GLuint pID, mat4 cameraMat, mat4 clipMat);
};

#endif
