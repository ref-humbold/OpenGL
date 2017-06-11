#include "Camera.hpp"

void Camera::drawObject(GLuint pID, GraphicObject * grobj)
{
    grobj->draw(pID, view, proj);
}

void Camera::viewScale(GLfloat zoom)
{
    fov *= zoom;
    fov = std::min(fov, PI_CONST / 2);
    fov = std::max(fov, PI_CONST / 6);
    proj = perspective(fov, (1.0f * windowW) / windowH, persBegin, persLength);
}

void Camera::viewRotate(GLfloat angleRad, vec3 axis)
{
    vec4 axisObj4 = inverse(view) * vec4(axis[0], axis[1], axis[2], 0.0f);
    vec3 axisObj = vec3(axisObj4[0], axisObj4[1], axisObj4[2]);

    view = view * toMat4(angleAxis(angleRad, axisObj));
}

vec3 Camera::getMousePos(GLFWwindow * window)
{
    double x, y;

    glfwGetCursorPos(window, &x, &y);

    vec3 res = vec3(2.0f * x / windowW - 1.0f, -(2.0f * y / windowH - 1.0f), 0.0f);
    GLfloat ln = res[0] * res[0] + res[1] * res[1];

    if(ln <= 1.0f)
        res[2] = sqrt(1.0f - ln);

    return res;
}

std::vector<bool> Camera::checkKeyPress(GLFWwindow * window, std::vector<int> & keys)
{
    std::vector<bool> result(keys.size());

    auto pressed = [=](int k) -> bool { return glfwGetKey(window, k) == GLFW_PRESS; };

    std::transform(keys.begin(), keys.end(), result.begin(), pressed);

    return result;
}

bool Camera::checkMouseAction(GLFWwindow * window, int action)
{
    return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == action;
}
