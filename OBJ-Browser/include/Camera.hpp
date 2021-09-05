#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#define GLM_ENABLE_EXPERIMENTAL

#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "GraphicObject.hpp"

enum class Key : int
{
    ZoomIn = GLFW_KEY_Z,
    ZoomOut = GLFW_KEY_X,
    ItemLeft = GLFW_KEY_LEFT,
    ItemRight = GLFW_KEY_RIGHT
};

class Camera
{
private:
    static constexpr GLfloat PI_CONST = M_PI;

    glm::mat4 view;
    glm::mat4 proj;
    GLfloat fov;
    GLfloat persBegin;
    GLfloat persLength;
    int windowW;
    int windowH;

public:
    explicit Camera(GLFWwindow * window) : fov{PI_CONST / 4}, persBegin{2}, persLength{16}
    {
        glfwGetWindowSize(window, &windowW, &windowH);
        view = glm::lookAt(glm::vec3(0.0f, 0.0f, 8.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                           glm::vec3(0.0f, 1.0f, 0.0f));
        proj = glm::perspective(fov, (1.0f * windowW) / windowH, persBegin, persLength);
    }

    void drawObject(GLuint pID, GraphicObject & grobj);
    void viewScale(GLfloat zoom);
    void viewRotate(GLfloat angleRad, glm::vec3 axis);
    glm::vec3 getMousePos(GLFWwindow * window);
    std::vector<Key> checkKeyPress(GLFWwindow * window, const std::vector<Key> & keys);
    bool checkMouseAction(GLFWwindow * window, int action);
};

#endif
