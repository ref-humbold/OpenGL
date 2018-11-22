#ifndef GAME_CONTROLLER_HPP
#define GAME_CONTROLLER_HPP

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <tuple>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "GameAquarium.hpp"
#include "GameBubble.hpp"
#include "GamePlayer.hpp"

using namespace glm;

class GameController
{
private:
    static constexpr GLfloat PI_CONST = M_PI;

    GameAquarium * aqua;
    GamePlayer * player;
    GameBubble * bubble;

    mat4 view;
    mat4 proj;
    vec3 cameraPos;
    vec3 cameraDir;
    std::vector<vec4> playerMoves;
    bool viewInside;
    GLfloat fov;
    GLfloat persBegin;
    GLfloat persStep;
    vec4 lightSource;
    int windowW;
    int windowH;

public:
    double points;

    explicit GameController(GLFWwindow * window);

    ~GameController()
    {
        delete aqua;
        delete player;
        delete bubble;
    }

    void drawGame(GLuint pID);
    void restart();
    void restartMoves();
    void setCamera();
    void changeCamera();
    void viewScale(GLfloat zoom);
    void viewRotate(GLfloat angleRad, vec3 axis);
    int checkCollisionBubble();
    bool checkEndRound();
    void deletePointedBubble(int ix);
    GLfloat moveBubbles(GLfloat delta, GLfloat counter, int freq);
    void movePlayer(GLfloat delta, std::vector<bool> movesMask);
    vec3 getMousePos(GLFWwindow * window);
    std::vector<bool> checkKeyPress(GLFWwindow * window, std::vector<int> & keys);
    bool checkMouseAction(GLFWwindow * window, int action);
    void checkTabReleased(GLFWwindow * window);
};

#endif
