#include "GLinit.hpp"
#include <string>
#include "GLSLloader.hpp"

#define GLFW_TRANSPARENCY false
#define GLFW_3D false

using namespace std::string_literals;

const std::string programName = "Memory Game"s;

namespace
{
    void createVertexArray()
    {
        GLuint vertexArrayID;

        glGenVertexArrays(1, &vertexArrayID);
        glBindVertexArray(vertexArrayID);
    }

    void addGlfwHints()
    {
        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    void addGlfwSettings()
    {
#if GLFW_TRANSPARENCY
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif

#if GLFW_3D
        glShadeModel(GL_SMOOTH);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
#endif
    }
}

GLProgram initializeGL()
{
    if(!glfwInit())
        throw gl_error("Failed to initialize GLFW"s);

    addGlfwHints();

    GLFWwindow * window = glfwCreateWindow(1024, 768, programName.c_str(), nullptr, nullptr);

    if(window == nullptr)
    {
        glfwTerminate();
        throw gl_error("Failed to open a new window"s);
    }

    glfwMakeContextCurrent(window);
    glewExperimental = true;

    if(glewInit() != GLEW_OK)
    {
        glfwTerminate();
        throw gl_error("Failed to initialize GLEW"s);
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // white

    GLuint programID = loadShaders();

    createVertexArray();
    addGlfwSettings();
    return GLProgram(window, programID);
}
