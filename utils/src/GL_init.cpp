#include "GL_init.hpp"
#include <string>

#define GLFW_TRANSPARENCY false
#define GLFW_3D false

using namespace std::string_literals;

GL_Program GL_ProgramBuilder::build(const std::string & programName)
{
    if(!glfwInit())
        throw GL_Error("Failed to initialize GLFW"s);

    addGlfwHints();

    GLFWwindow * window = glfwCreateWindow(1024, 768, programName.c_str(), nullptr, nullptr);

    if(window == nullptr)
    {
        glfwTerminate();
        throw GL_Error("Failed to open a new window"s);
    }

    glfwMakeContextCurrent(window);
    glewExperimental = true;

    if(glewInit() != GLEW_OK)
    {
        glfwTerminate();
        throw GL_Error("Failed to initialize GLEW"s);
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(backgroundColour.red, backgroundColour.green, backgroundColour.blue, 0.0f);

    std::vector<GLuint> programID = loader.loadShaders();

    createVertexArray();
    addGlfwSettings();
    return GL_Program(window, programID.at(0));
}

void GL_ProgramBuilder::createVertexArray()
{
    GLuint vertexArrayID;

    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);
}

void GL_ProgramBuilder::addGlfwHints()
{
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void GL_ProgramBuilder::addGlfwSettings()
{
    for(auto && s : programSettings)
        switch(s)
        {
            case GL_Settings::Transparency:
                glDisable(GL_CULL_FACE);
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                break;

            case GL_Settings::ThreeDimensions:
                glShadeModel(GL_SMOOTH);
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LESS);
                break;
        }
}
