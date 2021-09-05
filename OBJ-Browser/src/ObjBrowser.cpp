#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
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
#include "GLinit.hpp"
#include "GraphicObject.hpp"

using namespace std::string_literals;

bool vecDifferent(glm::vec3 v1, glm::vec3 v2)
{
    return v1[0] != v2[0] || v1[1] != v2[1] || v1[2] != v2[2];
}

GraphicObject prepareGraphic(const char * filename)
{
    GraphicObject object = readOBJ(filename);
    object.createBuffers();

    return object;
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
        throw std::runtime_error(filename + " is not an OBJ file."s);
}

int main(int argc, char * argv[])
{
    GLFWwindow * window;
    GLuint programID;

    std::tie(window, programID) = initializeGL();

    std::vector<GraphicObject> objects;
    int objBegin = 1;

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

    std::vector<Key> keys = {Key::ItemLeft, Key::ItemRight, Key::ZoomIn, Key::ZoomOut};
    Camera camera(window);
    auto objIter = objects.begin();
    int iterationClicked = 0;
    glm::vec3 mouseBegin, mouseEnd;
    bool mouseClicked = false;

    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);

        camera.drawObject(programID, *objIter);

        glfwSwapBuffers(window);
        glfwPollEvents();

        for(Key k : camera.checkKeyPress(window, keys))
            switch(k)
            {
                case Key::ItemLeft:
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

                case Key::ItemRight:
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

                case Key::ZoomIn:
                    camera.viewScale(0.99f);
                    break;

                case Key::ZoomOut:
                    camera.viewScale(1.01f);
                    break;
            }

        if(!mouseClicked && camera.checkMouseAction(window, GLFW_PRESS))
        {
            mouseBegin = camera.getMousePos(window);
            mouseClicked = true;
        }

        if(mouseClicked && camera.checkMouseAction(window, GLFW_PRESS))
        {
            mouseEnd = camera.getMousePos(window);

            if(vecDifferent(mouseBegin, mouseEnd))
            {
                glm::vec3 normBegin = glm::normalize(mouseBegin);
                glm::vec3 normEnd = glm::normalize(mouseEnd);
                GLfloat cosine = glm::dot(normBegin, normEnd);
                GLfloat angleRad = acos(glm::min(cosine, 1.0f));
                glm::vec3 axis = glm::normalize(glm::cross(normBegin, normEnd));

                camera.viewRotate(angleRad, axis);
                mouseBegin = mouseEnd;
            }
        }

        if(camera.checkMouseAction(window, GLFW_RELEASE))
            mouseClicked = false;
    } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
            && glfwWindowShouldClose(window) == 0);

    glfwTerminate();

    return 0;
}
