#ifndef _DETAILING_HPP_
#define _DETAILING_HPP_

#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Detailing
{
private:
    static constexpr int SIDE = 1201;

    std::vector<GLfloat> vbData;
    std::vector<std::vector<GLuint>> ibData;
    GLuint vertexBuffer;
    GLuint indexBuffer;

    int detailsLevel;
    std::vector<long long int> triangles;
    std::vector<int> detailsSteps;

public:
    Detailing();

    long long int getTriangles();
    int getLOD();
    void setLOD(int degLOD);
    int getStep();
    GLuint getVertexBuffer();
    GLuint getIndexBuffer();
};

#endif
