#ifndef GAME_CONTROLLER_HPP_
#define GAME_CONTROLLER_HPP_

#include <cstdlib>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "GameCard.hpp"

enum class Key : int
{
    None = GLFW_KEY_UNKNOWN,
    Select = GLFW_KEY_SPACE,
    MoveUp = GLFW_KEY_UP,
    MoveDown = GLFW_KEY_DOWN,
    MoveLeft = GLFW_KEY_LEFT,
    MoveRight = GLFW_KEY_RIGHT
};

class GameController
{
public:
    GameController(int rows, int columns);

    bool isVisible(int i)
    {
        return visible.find(i) != visible.end();
    }

    void setVisible(int i)
    {
        visible.emplace(i);
    }

    void restart();
    void drawGame(GLuint programID, int currentIndex, const std::pair<int, int> & visibleIndices);
    Key checkKeyPress(GLFWwindow * window);
    void checkKeyRelease(GLFWwindow * window, Key key);
    int moveFrame(Key key, int currentIndex);
    bool checkSame(const std::pair<int, int> & visibleIndices);

    const int fieldsCount;

private:
    void drawCards(GLuint programID, Colour colour, glm::vec2 transformation, int frameOffset);
    void drawSign(GLuint programID, Sign sign, glm::vec2 transformation);

    static constexpr int coloursCount = 8, signsCount = 4;
    const std::pair<int, int> size;
    const GLfloat vertexBufferData[42];
    GLuint vertexBuffer;
    std::vector<glm::vec2> transforms;
    std::map<int, std::pair<Colour, Sign>> cards;
    std::set<int> visible;
    std::default_random_engine rand_eng;
    std::uniform_int_distribution<int> colours_distrib;
    std::uniform_int_distribution<int> signs_distrib;
    std::uniform_int_distribution<int> fields_distrib;
};

#endif
