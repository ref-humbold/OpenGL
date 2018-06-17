#ifndef GAME_CONTROLLER_HPP
#define GAME_CONTROLLER_HPP

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "GameElement.hpp"

using namespace glm;

class GameController
{
public:
    GameController()
    {
    }

    void drawGame(GLuint pID, GameBoard * board, GameBall * ball, GameBrick * brick,
                  GamePaddle * paddle);
    int checkKeyPress(GLFWwindow * window);
    void checkKeyRelease(GLFWwindow * window, int key);
};

#endif
