#include "GameController.hpp"

void GameController::drawGame(GLuint pID, GameBoard & board, GameBall & ball, GameBrick & brick,
                              GamePaddle & paddle)
{
    board.drawBackground(pID);
    ball.draw(pID);
    brick.drawAll(pID);
    paddle.draw(pID);
    board.drawBorders(pID);
}

Key GameController::checkKeyPress(GLFWwindow * window)
{
    int action = GLFW_PRESS;

    if(glfwGetKey(window, GLFW_KEY_SPACE) == action)
        return Key::Pause;

    if(glfwGetKey(window, GLFW_KEY_LEFT) == action)
        return Key::MoveLeft;

    if(glfwGetKey(window, GLFW_KEY_RIGHT) == action)
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
