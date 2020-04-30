#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <tuple>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "GLSLloader.hpp"
#include "GameController.hpp"
#include "Parameters.hpp"

using namespace glm;

struct GameState
{
    explicit GameState(const GameController & ctrl) : cardsCount{ctrl.fieldsCount}
    {
        restart();
    }

    void restart()
    {
        currentIndex = 0;
        round = 1;
        cardPairsLeft = cardsCount / 2;
        visibleIndices = std::make_pair(-1, -1);
        std::cout << "\n## NEW GAME ##\n";
    }

    void printRound()
    {
        std::cout << "** Round " << round << "\n";
    }

    const int cardsCount;
    int currentIndex, round, cardPairsLeft;
    std::pair<int, int> visibleIndices;
};

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

std::tuple<GLFWwindow *, GLuint> initialize()
{
    if(!glfwInit())
        throw std::runtime_error("Failed to initialize glfw");

    glfwHints();

    GLFWwindow * window = glfwCreateWindow(1024, 768, "Memory Game", nullptr, nullptr);

    if(window == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to open a new window");
    }

    glfwMakeContextCurrent(window);
    glewExperimental = true;

    if(glewInit() != GLEW_OK)
        throw std::runtime_error("Failed to initialize glew");

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    GLuint programID = loadShaders();

    createVertexArray();
    return std::make_tuple(window, programID);
}

int main(int argc, char * argv[])
{
    parameters params(argc, argv);
    GLFWwindow * window;
    GLuint programID;

    std::tie(window, programID) = initialize();

    GameController ctrl(params.rows(), params.columns());
    GameState state(ctrl);

    state.printRound();

    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        ctrl.drawGame(programID, state.currentIndex, state.visibleIndices);
        glfwSwapBuffers(window);
        glfwPollEvents();

        if(state.cardPairsLeft > 0)
        {
            Key key = ctrl.checkKeyPress(window);

            if(key != Key::None)
            {
                ctrl.checkKeyRelease(window, key);

                if(state.visibleIndices.second >= 0)
                    state.visibleIndices = std::make_pair(-1, -1);
            }

            if(key == Key::Select && !ctrl.isVisible(state.currentIndex))
            {
                if(state.visibleIndices.first == -1)
                    state.visibleIndices.first = state.currentIndex;
                else if(state.visibleIndices.first != state.currentIndex)
                {
                    state.visibleIndices.second = state.currentIndex;

                    if(ctrl.checkSame(state.visibleIndices))
                    {
                        ctrl.setVisible(state.visibleIndices.first);
                        ctrl.setVisible(state.visibleIndices.second);
                        --state.cardPairsLeft;
                        std::cout << "-- Matched! --\n";
                    }

                    if(state.cardPairsLeft == 0)
                        std::cout << "You won in " << state.round << " rounds\n";
                    else
                    {
                        ++state.round;
                        state.printRound();
                    }
                }
            }
            else if(key != Key::None && key != Key::Select)
                state.currentIndex = ctrl.moveFrame(key, state.currentIndex);
        }
        else
        {
            Key key = ctrl.checkKeyPress(window);

            if(key == Key::Select)
            {
                ctrl.checkKeyRelease(window, key);
                ctrl.restart();
                state.restart();
            }
        }
    } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
            && glfwWindowShouldClose(window) == 0);

    if(state.cardPairsLeft > 0)
        std::cout << "Game was interrupted\n\n";

    glfwTerminate();
    return 0;
}
