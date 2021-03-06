#ifndef AREA_HPP_
#define AREA_HPP_

#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Detailing.hpp"

using namespace glm;

class Area
{
public:
    explicit Area(const char * filename);

    void draw(GLuint pID, mat4 worldToCamera, Detailing & details, int dims);
    std::pair<vec4, vec4> getCorners(int dims);

private:
    GLfloat countMapPosX(GLfloat longitude);
    GLfloat countMapPosY(GLfloat latitude);
    GLfloat countEarthPosX(GLfloat longitude, GLfloat latitude);
    GLfloat countEarthPosY(GLfloat latitude);
    GLfloat countEarthPosZ(GLfloat longitude, GLfloat latitude);
    void countCoefficients(const char * filename);
    void readHeights(const char * filename);

    static constexpr size_t SIDE = 1201;
    static constexpr GLfloat PI_CONST = M_PI;
    static constexpr GLfloat MERCATOR = 360.0f / (2 * PI_CONST);
    static constexpr GLfloat RADIUS = 6400000.0f;

    std::vector<GLfloat> hbData;
    GLuint heightBuffer;
    vec2 geoCoeffs;
};

#endif
