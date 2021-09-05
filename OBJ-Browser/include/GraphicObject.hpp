#ifndef GRAPHIC_OBJECT_HPP_
#define GRAPHIC_OBJECT_HPP_

#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class GraphicObject
{
private:
    std::vector<glm::vec3> vbData;
    std::vector<glm::vec2> tbData;
    std::vector<glm::vec3> nbData;
    GLuint vertexBuffer;
    GLuint textureBuffer;
    GLuint normalBuffer;

public:
    GraphicObject()
    {
    }

    void setVertexBuffer(const std::vector<glm::vec3> & data)
    {
        vbData = data;
    }

    void setTextureBuffer(const std::vector<glm::vec2> & data)
    {
        tbData = data;
    }

    void setNormalBuffer(const std::vector<glm::vec3> & data)
    {
        nbData = data;
    }

    void createBuffers();
    void draw(GLuint pID, glm::mat4 cameraMat, glm::mat4 clipMat);
};

#endif
