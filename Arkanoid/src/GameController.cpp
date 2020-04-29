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

Key GameController::checkKeyPress(GLFWwindow * window)
{
    int action = GLFW_PRESS;

    if(glfwGetKey(window, GLFW_KEY_SPACE) == action)
        return Key::StartPause;
    else if(glfwGetKey(window, GLFW_KEY_LEFT) == action)
        return Key::MoveLeft;
    else if(glfwGetKey(window, GLFW_KEY_RIGHT) == action)
        return Key::MoveRight;

    return Key::None;
}

void GameController::checkKeyRelease(GLFWwindow * window, Key key)
{
    if(key != Key::None)
        do
        {
            glfwPollEvents();
        } while(glfwGetKey(window, static_cast<int>(key)) != GLFW_RELEASE);
}
