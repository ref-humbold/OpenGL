#ifndef GAME_CONTROLLER_HPP_
#define GAME_CONTROLLER_HPP_

#include <cstdlib>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace glm;

enum class Key : int
{
    None = GLFW_KEY_UNKNOWN,
    Space = GLFW_KEY_SPACE,
    Up = GLFW_KEY_UP,
    Down = GLFW_KEY_DOWN,
    Left = GLFW_KEY_LEFT,
    Right = GLFW_KEY_RIGHT
};

enum class Colour : int
{
    Red = 0,
    Green = 1,
    Blue = 2,
    Cyan = 3,
    Magenta = 4,
    Yellow = 5,
    Black = 10,
    Gray = 11
};

enum class Sign : int
{
    Pipe = 0,
    Cross = 1,
    Triangle = 2,
    Square = 3
};

class GameController
{
private:
    const GLfloat vertexBufferData[42];
    GLuint vertexBuffer;
    std::pair<int, int> size;
    std::vector<Colour> colours;
    std::vector<Sign> signs;
    std::vector<std::pair<int, int>> transforms;
    std::vector<bool> visible;

public:
    explicit GameController(const std::pair<int, int> & size);

    bool isVisible(int i);
    void setVisible(int i);
    void drawGame(GLuint pID, int currentIndex, const std::pair<int, int> & visibleIndices);
    Key checkKeyPress(GLFWwindow * window);
    void checkKeyRelease(GLFWwindow * window, Key key);
    int moveFrame(Key key, int currentIndex);
    bool checkSame(const std::pair<int, int> & visibleIndices);

private:
    void drawCards(GLuint pID, Colour colour, std::pair<int, int> transformation, int frameOffset);
    void drawSign(GLuint pID, Sign sign, std::pair<int, int> transformation);
};

#endif
