#include <cstdlib>
#include <iostream>
#include "GLinit.hpp"
#include "GameController.hpp"
#include "GameElement.hpp"

enum class GamePhase : int
{
    NoPlay,
    Playing,
    Ended
};

int main()
{
    GLFWwindow * window;
    GLuint programID;

    std::tie(window, programID) = initializeGL();

    GameController ctrl;
    GameBoard board;
    GameBall ball;
    GameBrick brick;
    GamePaddle paddle;

    GamePhase gamePhase = GamePhase::NoPlay;
    int fallsCount = 0;
    GLfloat timer = 0.0f, pauseTime;

    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        ctrl.drawGame(programID, board, ball, brick, paddle);
        glfwSwapBuffers(window);
        glfwPollEvents();

        Key key = ctrl.checkKeyPress(window);

        if(gamePhase == GamePhase::Playing)
        {
            switch(key)
            {
                case Key::Pause:
                    pauseTime = glfwGetTime();
                    ctrl.checkKeyRelease(window, key);
                    std::cout << "\t* PAUSE - press space to continue.\n";
                    gamePhase = GamePhase::NoPlay;
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
                std::cout << "[[ The ball fell out! Try again. ]]\n";
                gamePhase = GamePhase::NoPlay;
                ++fallsCount;
                ball.restart();
                paddle.restart();
            }
            else
            {
                ball.checkCollisionPaddle(paddle);
                ball.checkCollisionBrick(brick);
                ball.checkCollisionBoard(board);

                GLfloat timeDelta =
                        gamePhase == GamePhase::NoPlay ? pauseTime - timer : glfwGetTime() - timer;

                timer = glfwGetTime();
                ball.moveBall(timeDelta);

                if(brick.bricksLeft == 0)
                {
                    std::cout << "YOU WON! The ball fell out " << fallsCount << " times.\n"
                              << "Press space to end.\n";
                    gamePhase = GamePhase::Ended;
                }
            }
        }
        else if(key == Key::Pause)
        {
            if(gamePhase == GamePhase::Ended)
                break;

            ctrl.checkKeyRelease(window, key);
            gamePhase = GamePhase::Playing;
            timer = glfwGetTime();
            std::cout << "\t## PLAY ##\n\n";
        }
    } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
            && glfwWindowShouldClose(window) == 0);

    if(brick.bricksLeft != 0)
        std::cout << "Game was interrupted.\n\n";

    glfwTerminate();
    return 0;
}
