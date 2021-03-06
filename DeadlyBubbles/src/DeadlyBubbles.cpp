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
#include "Parameters.hpp"

using namespace glm;

bool vecDifferent(vec3 v1, vec3 v2)
{
    return v1[0] != v2[0] || v1[1] != v2[1] || v1[2] != v2[2];
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
    parameters params(argc, argv);

    if(!glfwInit())
        throw std::runtime_error("FAILED TO INITIALIZE GLFW");

    glfwHints();

    GLFWwindow * window = glfwCreateWindow(1024, 768, "Deadly Bubbles", nullptr, nullptr);

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

    GLuint programID = loadShaders();

    createVertexArray();
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    srand(time(nullptr));

    GameController ctrl(window, params.training());
    std::vector<Key> keys = {Key::SwitchView, Key::ZoomIn,   Key::ZoomOut,
                             Key::MoveUp,     Key::MoveDown, Key::MoveFront,
                             Key::MoveBack,   Key::MoveLeft, Key::MoveRight};
    vec3 mouseBegin, mouseEnd;
    int gameLevel = -1;
    bool isRestarted = true, mouseClicked = false;
    GLfloat timer = 0.0f, counter = 0.0f;

    std::cout << "\n\tRUNDA 1: PUNKTY = " << ctrl.points << "\n";
    ctrl.restart();

    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        ctrl.drawGame(programID);
        glfwSwapBuffers(window);
        glfwPollEvents();

        if(gameLevel > 0)
        {
            std::vector<Key> moveKeys;
            std::vector<Key> pressed = ctrl.checkKeyPress(window, keys);

            for(Key k : pressed)
                switch(k)
                {
                    case Key::SwitchView:
                        ctrl.checkKeyReleased(window, k);
                        ctrl.changeCamera();
                        break;

                    case Key::ZoomIn:
                        ctrl.viewScale(0.99f);
                        break;

                    case Key::ZoomOut:
                        ctrl.viewScale(1.01f);
                        break;

                    case Key::MoveUp:
                    case Key::MoveDown:
                    case Key::MoveFront:
                    case Key::MoveBack:
                    case Key::MoveLeft:
                    case Key::MoveRight:
                        moveKeys.push_back(k);
                        break;
                }

            if(!mouseClicked && ctrl.checkMouseAction(window, GLFW_PRESS))
            {
                mouseBegin = ctrl.getMousePos(window);
                mouseClicked = true;
            }

            if(mouseClicked && ctrl.checkMouseAction(window, GLFW_PRESS))
            {
                mouseEnd = ctrl.getMousePos(window);

                if(vecDifferent(mouseBegin, mouseEnd))
                {
                    vec3 normBegin = normalize(mouseBegin);
                    vec3 normEnd = normalize(mouseEnd);
                    GLfloat cosine = dot(normBegin, normEnd);
                    GLfloat angleRad = acos(min(cosine, 1.0f));
                    vec3 axis = normalize(cross(normBegin, normEnd));

                    ctrl.viewRotate(angleRad, axis);
                    mouseBegin = mouseEnd;
                }
            }

            if(ctrl.checkMouseAction(window, GLFW_RELEASE))
                mouseClicked = false;

            int bubleNumber = ctrl.checkCollisionBubble();

            if(bubleNumber > 0)
            {
                std::cout << "\tTRAFIŁEŚ W PUNKTOWANY BĄBELEK!! ZDOBYWASZ 10% WIĘCEJ PUNKTÓW\n\n";
                ctrl.points *= 1.1;
                ctrl.deletePointedBubble(bubleNumber);
            }
            else if(bubleNumber < 0)
            {
                std::cout << "\tUDERZYŁEŚ W BĄBELEK!! PRZEGRANA.\n\n";
                gameLevel = 0;
                counter = 0.0f;
                timer = glfwGetTime();
            }
            else if(ctrl.checkEndRound())
            {
                gameLevel = -gameLevel - 1;
                counter = 0.0f;
                ctrl.points += 10.0;
                std::cout << "\tZA WYGRANIE RUNDY DOSTAJESZ 10 PUNKTÓW.\n";
                std::cout << "\tRUNDA " << -gameLevel << ": PUNKTY = " << ctrl.points << "\n";
                timer = glfwGetTime();
            }
            else
            {
                GLfloat delta = glfwGetTime() - timer;
                timer = glfwGetTime();
                counter += delta;
                counter = ctrl.moveBubbles(delta, counter, gameLevel);
                ctrl.movePlayer(delta, moveKeys);
            }
        }
        else if(gameLevel == 0)
        {
            if(counter < 2.0f)
            {
                GLfloat delta = glfwGetTime() - timer;
                counter += delta;
                timer = glfwGetTime();
            }
            else
                break;
        }
        else if(counter >= 3.0f)
        {
            std::cout << "\t\tGRAMY!!\n\n";
            gameLevel = -gameLevel;
            isRestarted = false;
            counter = 0.0f;
        }
        else
        {
            if(counter > 1.5f && !isRestarted)
            {
                isRestarted = true;
                ctrl.restart();
            }

            counter += glfwGetTime() - timer;
            timer = glfwGetTime();
        }
    } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
            && glfwWindowShouldClose(window) == 0);

    std::cout << "\tKONIEC GRY: PUNKTY = " << ctrl.points << "\n\n";
    glfwTerminate();
    return 0;
}
