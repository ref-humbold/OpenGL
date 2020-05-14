#include "GLinit.hpp"

using namespace std::string_literals;

void createVertexArray()
{
    GLuint vertexArrayID;

    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);
}

void glfwHints()
{
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

std::tuple<GLFWwindow *, GLuint> initializeGL()
{
    if(!glfwInit())
        throw gl_exception("Failed to initialize GLFW"s);

    glfwHints();

    GLFWwindow * window = glfwCreateWindow(1024, 768, "Arkanoid", nullptr, nullptr);

    if(window == nullptr)
    {
        glfwTerminate();
        throw gl_exception("Failed to open a new window"s);
    }

    glfwMakeContextCurrent(window);
    glewExperimental = true;

    if(glewInit() != GLEW_OK)
        throw gl_exception("Failed to initialize GLEW"s);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    GLuint programID = loadShaders();

    createVertexArray();
    return std::make_tuple(window, programID);
}
