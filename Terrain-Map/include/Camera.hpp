#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Area.hpp"
#include "Detailing.hpp"
#include "Earth.hpp"

using namespace glm;

class Camera
{
public:
    explicit Camera(GLFWwindow * window);

    int getDims();
    void changeDims();
    void drawEarth(GLuint pID, Earth & earth);
    void drawTerrain(GLuint pID, const std::vector<Area> & areas, Detailing & details);
    long long int getTriangles(const std::vector<Area> & areas, Detailing & details);
    vec2 getGeoCenter();
    GLfloat getZoom();
    void viewScale(GLfloat zoom);
    void resetScale();
    void viewRotate(GLfloat angleDeg, bool latitudeAlong);
    void cameraRotate(GLfloat angleDeg);
    void viewTranslate(vec3 trans);
    std::vector<bool> checkKeyPress(GLFWwindow * window, std::vector<int> & keys);

    int dims;

private:
    vec3 cameraDir3D();
    bool areaInside(Area & area, mat4 scview);

    static constexpr GLfloat PI_CONST = M_PI;
    static constexpr GLfloat MERCATOR = 360.0f / (2 * PI_CONST);
    static constexpr GLfloat RADIUS = 6400000.0f;

    mat4 sc, view, proj;
    GLfloat distance;
    vec2 cameraCoeffs, cameraMapPos;
    vec3 cameraEarthPos;
    GLfloat fov, persBegin, persLength;
    int windowW, windowH;
};

#endif
