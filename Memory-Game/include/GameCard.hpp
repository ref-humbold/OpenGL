#ifndef GAME_CARD_HPP_
#define GAME_CARD_HPP_

#include <cstdlib>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

enum class Colour : int
{
    Red = 0,
    Green = 1,
    Blue = 2,
    Cyan = 3,
    Magenta = 4,
    Yellow = 5,
    Orange = 6,
    Purple = 7,
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

class GameCard
{
public:
    GameCard(const std::pair<int, int> & size, const std::pair<int, int> & place, Colour colour,
             Sign sign);

    bool matches(const GameCard & card) const
    {
        return colour == card.colour && sign == card.sign;
    }

    void setVisible()
    {
        visible = true;
    }

    void draw(GLuint programID, bool isCurrent, bool hasFrame);

private:
    void drawCard(GLuint programID, bool isVisible);
    void drawSign(GLuint programID);
    void drawFrame(GLuint programID);

    const GLfloat vertexBufferData[42];
    GLuint vertexBuffer;
    glm::vec2 scaleVector;
    glm::vec2 transformVector;

    Colour colour;
    Sign sign;
    bool visible;
};

#endif
