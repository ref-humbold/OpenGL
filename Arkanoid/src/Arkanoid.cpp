#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "GLSLloader.hpp"
#include "GameController.hpp"
#include "GameElement.hpp"

using namespace glm;

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

int main()
{
    if(!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    glfwHints();

    GLFWwindow * window = glfwCreateWindow(1024, 768, "Arkanoid", nullptr, nullptr);

    if(window == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to open a new window");
    }

    glfwMakeContextCurrent(window);
    glewExperimental = true;

    if(glewInit() != GLEW_OK)
        throw std::runtime_error("Failed to initialize GLEW");

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    GLuint programID = loadShaders();

    createVertexArray();
    srand(time(nullptr));

    GameController ctrl;
    GameBoard board;
    GameBall ball;
    GameBrick brick;
    GamePaddle paddle;

    int gamePhase = 0, tryings = 0;
    GLfloat timer = 0.0f, pauseTime;

    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        ctrl.drawGame(programID, board, ball, brick, paddle);
        glfwSwapBuffers(window);
        glfwPollEvents();

        if(gamePhase == 1)
        {
            Key key = ctrl.checkKeyPress(window);

            switch(key)
            {
                case Key::StartPause:
                    pauseTime = glfwGetTime();
                    ctrl.checkKeyRelease(window, key);
                    std::cout << "\tPRZERWA - wciśnij spację, by kontunuować grę\n";
                    gamePhase = 0;
                    break;

                case Key::MoveLeft:
                    paddle.moveLeft(glfwGetTime() - timer);
                    break;

                case Key::MoveRight:
                    paddle.moveRight(glfwGetTime() - timer);
                    break;

                default:
                    break;
            }

            if(ball.checkOutside())
            {
                std::cout << "KULKA SPADŁA Z PLANSZY!! Spróbuj ponownie.\n";
                gamePhase = 0;
                ++tryings;
                ball.restart();
                paddle.restart();
            }
            else
            {
                ball.checkCollisionPaddle(paddle);
                ball.checkCollisionBrick(brick);
                ball.checkCollisionBoard(board);

                GLfloat delta = gamePhase == 0 ? pauseTime - timer : glfwGetTime() - timer;
                timer = glfwGetTime();

                ball.moveBall(delta);

                if(brick.bricksLeft == 0)
                {
                    std::cout << "WYGRAŁEŚ!! Kulka spadła Ci " << tryings << " razy.\n";
                    std::cout << "Wciśnij spację, by zakończyć.\n";
                    gamePhase = 2;
                }
            }
        }
        else
        {
            Key key = ctrl.checkKeyPress(window);

            if(key == Key::StartPause)
            {
                if(gamePhase == 2)
                    break;

                ctrl.checkKeyRelease(window, key);
                gamePhase = 1;
                timer = glfwGetTime();
                std::cout << "\tGRAMY!!\n\n";
            }
        }
    } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
            && glfwWindowShouldClose(window) == 0);

    if(brick.bricksLeft != 0)
        std::cout << "PRZERWANO GRĘ\n\n";

    glfwTerminate();
    return 0;
}
