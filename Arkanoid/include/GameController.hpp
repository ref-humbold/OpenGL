#ifndef GAME_CONTROLLER_HPP_
#define GAME_CONTROLLER_HPP_

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "GameElement.hpp"

using namespace glm;

enum class Key : int
{
    None = GLFW_KEY_UNKNOWN,
    StartPause = GLFW_KEY_SPACE,
    MoveLeft = GLFW_KEY_LEFT,
    MoveRight = GLFW_KEY_RIGHT
};

class GameController
{
public:
    GameController()
    {
    }

    void drawGame(GLuint pID, GameBoard & board, GameBall & ball, GameBrick & brick,
                  GamePaddle & paddle);
    Key checkKeyPress(GLFWwindow * window);
    void checkKeyRelease(GLFWwindow * window, Key key);
};

#endif
