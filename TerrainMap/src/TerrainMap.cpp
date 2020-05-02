#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Area.hpp"
#include "Camera.hpp"
#include "Detailing.hpp"
#include "Earth.hpp"
#include "GLSLloader.hpp"

using namespace glm;
using namespace std::string_literals;

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

    if(length <= 4 || strcmp(filename + length - 4, ".hgt") != 0)
        throw std::runtime_error(filename + " is not an HGT file."s);
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

    GLFWwindow * window = glfwCreateWindow(1024, 768, "TerrainMap", nullptr, nullptr);

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

    GLuint areaProgramID, earthProgramID;

    std::tie(areaProgramID, earthProgramID) = loadShaders();
    createVertexArray();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    Camera camera(window);
    Detailing details;
    Earth earth;
    std::vector<Area> terrain;
    int hgtBegin = 2;

    if(argc <= hgtBegin)
        throw std::runtime_error("No HGT files specified");

    if(strcmp(argv[hgtBegin], "config.txt") == 0)
    {
        std::vector<std::string> names = readConfig(argv[hgtBegin]);

        for(auto str : names)
        {
            checkFile(str.c_str());
            terrain.push_back(Area(str.c_str()));
        }

        ++hgtBegin;
    }

    for(int i = hgtBegin; i < argc; ++i)
    {
        checkFile(argv[i]);
        terrain.push_back(Area(argv[i]));
    }

    std::vector<int> keys = {GLFW_KEY_TAB,   GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT,
                             GLFW_KEY_RIGHT, GLFW_KEY_Z,  GLFW_KEY_X,    GLFW_KEY_A,
                             GLFW_KEY_Q,     GLFW_KEY_W,  GLFW_KEY_1,    GLFW_KEY_2,
                             GLFW_KEY_3,     GLFW_KEY_4,  GLFW_KEY_5,    GLFW_KEY_6,
                             GLFW_KEY_7,     GLFW_KEY_8,  GLFW_KEY_9,    GLFW_KEY_0};
    std::vector<GLfloat> levels = {-1.2f, -0.8f, -0.4f, -0.01f, 0.4f, 0.8f, 1.2f};
    auto nextLevel = std::find_if(levels.begin(), levels.end(),
                                  [&](GLfloat lvl) { return lvl >= camera.getZoom(); });
    int frames = 0, lastFrames = 0;
    bool autoLOD = true;
    GLfloat time_start = glfwGetTime();

    std::cout << "CENTER AT: longitude = 0, latitude = 0\n";

    do
    {
        GLfloat time_end = glfwGetTime();

        ++frames;

        if(time_end - time_start >= 1.0)
        {
            std::cout << "\t\t" << frames << " FPS - " << 1000.0f / frames << " ms per frame";
            std::cout << " >> LOD " << details.getLOD() << " - step " << details.getStep();
            std::cout << " >> TRIANGLES " << camera.getTriangles(terrain, details) << "\n";

            if(autoLOD)
            {
                if(nextLevel != levels.end() && camera.getZoom() > *nextLevel)
                {
                    ++nextLevel;
                    details.setLOD(details.getLOD() - 1);
                }
                else if(nextLevel != levels.begin() && camera.getZoom() < *(nextLevel - 1))
                    --nextLevel;

                if(frames < 10 && lastFrames < 10)
                    details.setLOD(details.getLOD() + 1);
                else if(frames >= 30 && lastFrames >= 30)
                    details.setLOD(details.getLOD() - 1);
            }

            lastFrames = frames;
            frames = 0;
            time_start = glfwGetTime();
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(camera.getDims() == 3)
        {
            glUseProgram(earthProgramID);
            camera.drawEarth(earthProgramID, earth);
        }

        glUseProgram(areaProgramID);
        camera.drawTerrain(areaProgramID, terrain, details);

        glfwSwapBuffers(window);
        glfwPollEvents();

        std::vector<bool> pressed = camera.checkKeyPress(window, keys);

        for(unsigned int i = 0; i < pressed.size(); ++i)
            if(pressed[i])
            {
                int loop_start;

                switch(keys[i])
                {
                    case GLFW_KEY_TAB:
                        loop_start = glfwGetTime();

                        while(glfwGetKey(window, GLFW_KEY_TAB) != GLFW_RELEASE)
                            glfwPollEvents();

                        time_start = glfwGetTime() - loop_start + time_start;
                        camera.changeDims();
                        std::cout << "DIMENSIONS = " << camera.getDims() << "\n";
                        std::cout << "CENTER AT: longitude = 0, latitude = 0\n";
                        break;

                    case GLFW_KEY_Z:
                        camera.viewScale(0.99f);
                        std::cout << "ZOOM: " << camera.getZoom() << "\n";
                        break;

                    case GLFW_KEY_X:
                        camera.viewScale(1.01f);
                        std::cout << "ZOOM: " << camera.getZoom() << "\n";
                        break;

                    case GLFW_KEY_A:
                        camera.resetScale();
                        std::cout << "ZOOM: " << camera.getZoom() << "\n";
                        break;

                    case GLFW_KEY_Q:
                        camera.cameraRotate(1.25f);
                        break;

                    case GLFW_KEY_W:
                        camera.cameraRotate(-1.25f);
                        break;

                    case GLFW_KEY_UP:
                        if(camera.getDims() == 2)
                            camera.viewTranslate(vec3(0.0f, 0.25f, 0.0f));
                        else
                            camera.viewRotate(0.25f, false);

                        std::cout << "CENTER AT: longitude = " << camera.getGeoCenter()[0]
                                  << ", latitude = " << camera.getGeoCenter()[1] << "\n";
                        break;

                    case GLFW_KEY_DOWN:
                        if(camera.getDims() == 2)
                            camera.viewTranslate(vec3(0.0f, -0.25f, 0.0f));
                        else
                            camera.viewRotate(-0.25f, false);

                        std::cout << "CENTER AT: longitude = " << camera.getGeoCenter()[0]
                                  << ", latitude = " << camera.getGeoCenter()[1] << "\n";
                        break;

                    case GLFW_KEY_LEFT:
                        if(camera.getDims() == 2)
                            camera.viewTranslate(vec3(-0.25f, 0.0f, 0.0f));
                        else
                            camera.viewRotate(-0.5f, true);

                        std::cout << "CENTER AT: longitude = " << camera.getGeoCenter()[0]
                                  << ", latitude = " << camera.getGeoCenter()[1] << "\n";
                        break;

                    case GLFW_KEY_RIGHT:
                        if(camera.getDims() == 2)
                            camera.viewTranslate(vec3(0.25f, 0.0f, 0.0f));
                        else
                            camera.viewRotate(0.5f, true);

                        std::cout << "CENTER AT: longitude = " << camera.getGeoCenter()[0]
                                  << ", latitude = " << camera.getGeoCenter()[1] << "\n";
                        break;

                    case GLFW_KEY_0:
                        autoLOD = true;
                        details.setLOD(0);
                        break;

                    case GLFW_KEY_1:
                        autoLOD = false;
                        details.setLOD(0);
                        break;

                    case GLFW_KEY_2:
                        autoLOD = false;
                        details.setLOD(1);
                        break;

                    case GLFW_KEY_3:
                        autoLOD = false;
                        details.setLOD(2);
                        break;

                    case GLFW_KEY_4:
                        autoLOD = false;
                        details.setLOD(3);
                        break;

                    case GLFW_KEY_5:
                        autoLOD = false;
                        details.setLOD(4);
                        break;

                    case GLFW_KEY_6:
                        autoLOD = false;
                        details.setLOD(5);
                        break;

                    case GLFW_KEY_7:
                        autoLOD = false;
                        details.setLOD(6);
                        break;

                    case GLFW_KEY_8:
                        autoLOD = false;
                        details.setLOD(7);
                        break;

                    case GLFW_KEY_9:
                        autoLOD = false;
                        details.setLOD(8);
                        break;
                }
            }
    } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
            && glfwWindowShouldClose(window) == 0);

    glfwTerminate();
    return 0;
}
