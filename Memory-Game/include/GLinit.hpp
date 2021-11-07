#ifndef GL_INIT_HPP_
#define GL_INIT_HPP_

#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct gl_error : std::runtime_error
{
    explicit gl_error(const std::string & s) : std::runtime_error(s)
    {
    }
};

struct GLProgram
{
    GLProgram(GLFWwindow * window, GLuint programID) : window{window}, programID{programID}
    {
    }

    ~GLProgram()
    {
        glfwTerminate();
    }

    GLFWwindow * window;
    GLuint programID;
};

GLProgram initializeGL();

#endif
