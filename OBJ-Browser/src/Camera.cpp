#include "Camera.hpp"

void Camera::drawObject(GLuint pID, GraphicObject & object)
{
    object.draw(pID, view, proj);
}

void Camera::viewScale(GLfloat zoom)
{
    fov *= zoom;
    fov = std::min(fov, PI_CONST / 2);
    fov = std::max(fov, PI_CONST / 6);
    proj = glm::perspective(fov, (1.0f * windowW) / windowH, persBegin, persLength);
}

void Camera::viewRotate(GLfloat angleRad, glm::vec3 axis)
{
    glm::vec4 axisObj4 = inverse(view) * glm::vec4(axis[0], axis[1], axis[2], 0.0f);
    glm::vec3 axisObj = glm::vec3(axisObj4[0], axisObj4[1], axisObj4[2]);

    view = view * glm::toMat4(angleAxis(angleRad, axisObj));
}

glm::vec3 Camera::getMousePos(GLFWwindow * window)
{
    double x, y;

    glfwGetCursorPos(window, &x, &y);

    glm::vec3 res = glm::vec3(2.0f * x / windowW - 1.0f, -(2.0f * y / windowH - 1.0f), 0.0f);
    GLfloat length = res[0] * res[0] + res[1] * res[1];

    if(length <= 1.0f)
        res[2] = sqrt(1.0f - length);

    return res;
}

std::vector<Key> Camera::checkKeyPress(GLFWwindow * window, const std::vector<Key> & keys)
{
    std::vector<Key> pressed;
    auto was_pressed = [=](Key k) -> bool {
        return glfwGetKey(window, static_cast<int>(k)) == GLFW_PRESS;
    };

    std::copy_if(keys.begin(), keys.end(), std::back_inserter(pressed), was_pressed);
    return pressed;
}

bool Camera::checkMouseAction(GLFWwindow * window, int action)
{
    return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == action;
}
