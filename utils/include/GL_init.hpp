#ifndef GL_INIT_HPP_
#define GL_INIT_HPP_

#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GLSL_loader.hpp"

struct GL_Error : std::runtime_error
{
    explicit GL_Error(const std::string & s) : std::runtime_error(s)
    {
    }
};

struct Colour
{
    Colour(GLclampf red, GLclampf green, GLclampf blue) : red{red}, green{green}, blue{blue}
    {
    }

    GLclampf red;
    GLclampf green;
    GLclampf blue;
};

enum class GL_Settings
{
    Transparency,
    ThreeDimensions
};

struct GL_Program
{
    explicit GL_Program(GLFWwindow * window, GLuint programID)
        : window{window}, programID{programID}
    {
    }

    ~GL_Program()
    {
        glfwTerminate();
    }

    GLFWwindow * window;
    GLuint programID;
};

class GL_ProgramBuilder
{
public:
    explicit GL_ProgramBuilder(const GLSL_ShaderLoader & loader)
        : loader{loader}, backgroundColour{0.0f, 0.0f, 0.0f}
    {
    }

    GL_ProgramBuilder & addSettings(GL_Settings settings)
    {
        programSettings.push_back(settings);
        return *this;
    }

    GL_ProgramBuilder & background(const Colour & colour)
    {
        backgroundColour = colour;
        return *this;
    }

    GL_Program build(const std::string & programName);

private:
    void createVertexArray();
    void addGlfwHints();
    void addGlfwSettings();

    GLSL_ShaderLoader loader;
    std::vector<GL_Settings> programSettings;
    Colour backgroundColour;
};

#endif
