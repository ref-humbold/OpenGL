#include "GameController.hpp"

using namespace glm;

void GameController::drawGame(GLuint pID, GameBoard & board, GameBall & ball, GameBrick & brick,
                              GamePaddle & paddle)
{
    board.drawBackground(pID);
    ball.drawBall(pID);
    ball.drawCross(pID);
    brick.drawAllBricks(pID);
    paddle.drawPaddle(pID);
    board.drawBorderTriangles(pID);
    board.countNormalVectors();
}

int GameController::checkKeyPress(GLFWwindow * window)
{
    int action = GLFW_PRESS;

    if(glfwGetKey(window, GLFW_KEY_SPACE) == action)
        return 0;
    else if(glfwGetKey(window, GLFW_KEY_LEFT) == action)
        return 1;
    else if(glfwGetKey(window, GLFW_KEY_RIGHT) == action)
        return 2;

    return -1;
}

void GameController::checkKeyRelease(GLFWwindow * window, int key)
{
    int action = GLFW_RELEASE;

    switch(key)
    {
        case 0:
            while(glfwGetKey(window, GLFW_KEY_SPACE) != action)
                glfwPollEvents();

            break;

        case 1:
            while(glfwGetKey(window, GLFW_KEY_LEFT) != action)
                glfwPollEvents();

            break;

        case 2:
            while(glfwGetKey(window, GLFW_KEY_RIGHT) != action)
                glfwPollEvents();

            break;
    }

    return;
}
