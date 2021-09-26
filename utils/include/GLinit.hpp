#ifndef GL_INIT_HPP_
#define GL_INIT_HPP_

#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <tuple>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct gl_error : std::runtime_error
{
    explicit gl_error(const std::string & s) : std::runtime_error(s)
    {
    }
};

std::tuple<GLFWwindow *, GLuint> initializeGL();

#endif
