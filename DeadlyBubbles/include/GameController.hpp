#ifndef GAME_CONTROLLER_HPP
#define GAME_CONTROLLER_HPP

#define GLM_ENABLE_EXPERIMENTAL

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <map>
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

enum class Key : int
{
    SwitchView = GLFW_KEY_TAB,
    ZoomIn = GLFW_KEY_UP,
    ZoomOut = GLFW_KEY_DOWN,
    MoveRight = GLFW_KEY_D,  // forward X axis
    MoveLeft = GLFW_KEY_A,  //backward X axis
    MoveUp = GLFW_KEY_E,  // forward Y axis
    MoveDown = GLFW_KEY_Z,  // backward Y axis
    MoveBack = GLFW_KEY_X,  // forward Z axis
    MoveFront = GLFW_KEY_W  // backward Z axis
};

class GameController
{
public:
    GameController(GLFWwindow * window, bool training);

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
    void movePlayer(GLfloat delta, const std::vector<Key> & keys);
    vec3 getMousePos(GLFWwindow * window);
    std::vector<Key> checkKeyPress(GLFWwindow * window, const std::vector<Key> & keys);
    bool checkMouseAction(GLFWwindow * window, int action);
    void checkKeyReleased(GLFWwindow * window, Key key);

    double points;

private:
    static constexpr GLfloat PI_CONST = M_PI;

    GameAquarium aqua;
    GamePlayer player;
    GameBubble bubble;
    mat4 view, proj;
    vec3 cameraPos, cameraDir;
    std::map<Key, vec4> playerMoves;
    bool viewInside, training;
    GLfloat fov, persBegin, persStep;
    vec4 lightSource;
    int windowWidth, windowHeight;
};

#endif
