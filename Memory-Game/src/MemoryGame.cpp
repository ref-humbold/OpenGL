#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "GLSLloader.hpp"
#include "GameController.hpp"

using namespace glm;

void createVertexArray()
{
    GLuint vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);
}

bool isInRange(int num, int min, int mx)
{
    return min <= num && num <= mx;
}

void printRound(int round)
{
    std::cout << "\t\tRUNDA " << round << "\n";
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
    int numRows = 4, numColumns = 4;

    switch(argc)
    {
        case 3:
            numColumns = atoi(argv[2]);

        case 2:
            numRows = atoi(argv[1]);
    }

    if(!isInRange(numRows, 1, 12))
        throw std::runtime_error("INCORRECT NUMBER OF ROWS, MINIMUM 1, MAXIMUM 12, GOT "
                                 + std::to_string(numRows));

    if(!isInRange(numColumns, 1, 12))
        throw std::runtime_error("INCORRECT NUMBER OF COLUMNS, MINIMUM 1, MAXIMUM 12, GOT "
                                 + std::to_string(numColumns));

    if((numRows * numColumns) % 2 != 0)
        throw std::runtime_error("ODD NUMBER OF CARDS");

    if(!glfwInit())
        throw std::runtime_error("FAILED TO INITIALIZE GLFW");

    glfwHints();

    GLFWwindow * window = glfwCreateWindow(1024, 768, "Memory Game", nullptr, nullptr);

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
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    GLuint programID = loadShaders();

    createVertexArray();

    GameController ctrl(std::make_pair(numRows, numColumns));
    int currentIndex = 0, round = 1, cardsLeft = numRows * numColumns;
    std::pair<int, int> visibleIndices = std::make_pair(-1, -1);

    printRound(round);

    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        ctrl.drawGame(programID, currentIndex, visibleIndices);
        glfwSwapBuffers(window);
        glfwPollEvents();

        if(cardsLeft > 0)
        {
            glfwPollEvents();

            Key key = ctrl.checkKeyPress(window);

            if(key != Key::None)
            {
                ctrl.checkKeyRelease(window, key);

                if(visibleIndices.second > 0)
                    visibleIndices = std::make_pair(-1, -1);
            }

            if(key == Key::Space && !ctrl.isVisible(currentIndex))
            {
                if(visibleIndices.first == -1)
                    visibleIndices.first = currentIndex;
                else if(visibleIndices.first != currentIndex)
                {
                    visibleIndices.second = currentIndex;

                    if(ctrl.checkSame(visibleIndices))
                    {
                        ctrl.setVisible(visibleIndices.first);
                        ctrl.setVisible(visibleIndices.second);
                        cardsLeft -= 2;
                        std::cout << "\tTRAFIONO!!!\n";
                    }

                    if(cardsLeft == 0)
                        std::cout << "WYGRANA W " << round << " RUNDACH\n";
                    else
                    {
                        ++round;
                        printRound(round);
                    }
                }
            }
            else if(key != Key::None && key != Key::Space)
                currentIndex = ctrl.moveFrame(key, currentIndex);
        }
    } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
            && glfwWindowShouldClose(window) == 0);

    if(cardsLeft != 0)
        std::cout << "PRZERWANO GRĘ\n\n";

    glfwTerminate();
    return 0;
}
