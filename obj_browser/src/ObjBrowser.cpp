#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Camera.hpp"
#include "FileReader.hpp"
#include "GLSLloader.hpp"
#include "GraphicObject.hpp"

using namespace glm;

bool vecDifferent(vec3 v1, vec3 v2)
{
    return v1[0] != v2[0] || v1[1] != v2[1] || v1[2] != v2[2];
}

GraphicObject * prepareGraphic(const char * filename)
{
    GraphicObject * grobj = new GraphicObject();

    readOBJ(grobj, filename);
    grobj->createBuffers();

    return grobj;
}

std::vector<std::string> readConfig(const char * filename)
{
    std::vector<std::string> result;
    FILE * file = fopen(filename, "r");

    while(true)
    {
        char str[51];
        int read = fscanf(file, "%50s", str);

        if(read == EOF)
            break;

        result.push_back(std::string(str));
    }

    fclose(file);

    return result;
}

void checkFile(const char * filename)
{
    int length = strlen(filename);

    if(length <= 4 || strcmp(filename + length - 4, ".obj") != 0)
        throw std::runtime_error(std::string(filename) + " is not an OBJ file.");
}

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

int main(int argc, char * argv[])
{
    if(!glfwInit())
        throw std::runtime_error("FAILED TO INITIALIZE GLFW");

    glfwHints();

    GLFWwindow * window = glfwCreateWindow(1024, 768, "ObjectBrowser", nullptr, nullptr);

    if(window == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("FAILED TO OPEN A NEW WINDOW");
    }

    glfwMakeContextCurrent(window);
    glewExperimental = true;

    if(glewInit() != GLEW_OK)
        throw std::runtime_error("FAILED TO INITIALIZE GLEW");

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    if(argc <= 1)
        throw std::runtime_error("No directory with shaders specified");

    std::string glsl_dir = argv[1];
    GLuint programID =
        loadShaders(glsl_dir + "/VertexShader.glsl", glsl_dir + "/FragmentShader.glsl");

    createVertexArray();

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    std::vector<GraphicObject *> objects;
    int objBegin = 2;

    if(argc <= objBegin)
        throw std::runtime_error("No OBJ files specified");

    if(strcmp(argv[objBegin], "config.txt") == 0)
    {
        std::vector<std::string> names = readConfig(argv[objBegin]);

        for(auto str : names)
        {
            checkFile(str.c_str());
            std::cout << "Adding file " << str << "\n";
            objects.push_back(prepareGraphic(str.c_str()));
        }

        ++objBegin;
    }

    for(int i = objBegin; i < argc; ++i)
    {
        checkFile(argv[i]);
        std::cout << "Adding file " << argv[i] << "\n";
        objects.push_back(prepareGraphic(argv[i]));
    }

    std::vector<int> keys = {GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_Z, GLFW_KEY_X};
    Camera * cam = new Camera(window);
    auto objIter = objects.begin();
    int iterationClicked = 0;
    vec3 mouseBegin, mouseEnd;
    bool mouseClicked = false;

    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);

        cam->drawObject(programID, *objIter);

        glfwSwapBuffers(window);
        glfwPollEvents();

        std::vector<bool> pressed = cam->checkKeyPress(window, keys);

        for(unsigned int i = 0; i < pressed.size(); ++i)
        {
            if(pressed[i])
                switch(keys[i])
                {
                    case GLFW_KEY_LEFT:
                        if(objIter != objects.begin())
                        {
                            if(iterationClicked >= 3)
                            {
                                iterationClicked = 0;
                                --objIter;
                            }
                            else
                                ++iterationClicked;
                        }

                        break;

                    case GLFW_KEY_RIGHT:
                        if(objIter != objects.end() - 1)
                        {
                            if(iterationClicked >= 3)
                            {
                                iterationClicked = 0;
                                ++objIter;
                            }
                            else
                                ++iterationClicked;
                        }

                        break;

                    case GLFW_KEY_Z:
                        cam->viewScale(0.99f);
                        break;

                    case GLFW_KEY_X:
                        cam->viewScale(1.01f);
                        break;
                }
        }

        if(!mouseClicked && cam->checkMouseAction(window, GLFW_PRESS))
        {
            mouseBegin = cam->getMousePos(window);
            mouseClicked = true;
        }

        if(mouseClicked && cam->checkMouseAction(window, GLFW_PRESS))
        {
            mouseEnd = cam->getMousePos(window);

            if(vecDifferent(mouseBegin, mouseEnd))
            {
                vec3 normBegin = normalize(mouseBegin);
                vec3 normEnd = normalize(mouseEnd);
                GLfloat cosine = dot(normBegin, normEnd);
                GLfloat angleRad = acos(min(cosine, 1.0f));
                vec3 axis = normalize(cross(normBegin, normEnd));

                cam->viewRotate(angleRad, axis);
                mouseBegin = mouseEnd;
            }
        }

        if(cam->checkMouseAction(window, GLFW_RELEASE))
            mouseClicked = false;
    } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
            && glfwWindowShouldClose(window) == 0);

    glfwTerminate();

    return 0;
}
