#ifndef GAME_ELEMENT_HPP_
#define GAME_ELEMENT_HPP_

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <map>
#include <random>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

GLuint createVertexBuffer(const GLfloat vbData[], size_t size);

void loadBuffer(GLuint vertexBufferID, GLuint colourBufferID);

class GameBoard
{
public:
    enum class BorderPlace : int
    {
        LeftBottom,
        LeftTop,
        Top,
        RightTop,
        RightBottom
    };

    GameBoard();

    void drawBackground(GLuint pID);
    void drawBorders(GLuint pID);
    GLfloat distance(const glm::vec2 & point, BorderPlace place);

    const BorderPlace borders[5];
    std::map<BorderPlace, glm::vec2> normalVectors;

private:
    glm::vec2 orthogonal(const glm::vec2 & v)
    {
        return glm::vec2(-v[1], v[0]);
    }

    void drawOneTriangle(GLuint pID);
    void countNormalVectors();
    void countInnerSides();

    const GLfloat vbDataHexagon[16];
    const GLfloat cbDataHexagon[24];
    const GLfloat vbDataTriangle[6];
    const GLfloat cbDataTriangle[9];
    GLuint vertexBufferHexagon;
    GLuint colorBufferHexagon;
    GLuint vertexBufferTriangle;
    GLuint colorBufferTriangle;

    glm::mat2 scaleMatrix;
    glm::mat2 rotateMatrix;
    glm::vec2 transformVector;
    std::map<BorderPlace, std::pair<glm::vec2, glm::vec2>> innerSides;
};

class GameBrick
{
public:
    GameBrick();

    void drawAll(GLuint pID);

    static constexpr int rowsNumber = 6, columnsNumber = 13;
    bool isVisible[rowsNumber][columnsNumber];
    int bricksLeft;

private:
    enum class Colour : int
    {
        Red,
        Green,
        Blue,
        Cyan,
        Magenta,
        Yellow
    };

    void drawRect(GLuint pID, Colour colour);
    void drawRectBorder(GLuint pID);

    const GLfloat vbDataRect[8];
    GLfloat cbDataRect[12];
    const GLfloat cbDataRectBorder[12];
    GLuint vertexBufferRect;
    GLuint colorBufferRect;
    GLuint colorBufferRectBorder;

    glm::mat2 scaleMatrix;
    glm::mat2 rotateMatrix;
    glm::vec2 transformVector;
};

class GamePaddle
{
public:
    GamePaddle();

    void restart();
    void draw(GLuint pID);

    int reflection()
    {
        return reflect_distrib(rand_eng) * 0.005f;
    }

    void moveLeft(GLfloat delta)
    {
        transformVector[0] = std::max(-0.4f, transformVector[0] - velocity * delta);
    }

    void moveRight(GLfloat delta)
    {
        transformVector[0] = std::min(0.4f, transformVector[0] + velocity * delta);
    }

    GLfloat getPositionX()
    {
        return transformVector[0];
    }

    GLfloat getSurfaceY()
    {
        return transformVector[1] + 0.01f;
    }

private:
    const GLfloat vbDataPaddle[20];
    const GLfloat cbDataPaddle[30];
    GLuint vertexBufferPaddle;
    GLuint colorBufferPaddle;

    glm::mat2 scaleMatrix;
    glm::mat2 rotateMatrix;
    glm::vec2 transformVector;
    GLfloat velocity;
    std::default_random_engine rand_eng;
    std::uniform_int_distribution<int> reflect_distrib;
};

class GameBall
{
public:
    GameBall();

    void draw(GLuint pID)
    {
        drawBall(pID);
        drawCross(pID);
    }

    bool checkOutside()
    {
        return transformVector[1] <= -1.0f;
    }

    void restart();
    void checkCollision(GameBoard & board);
    void checkCollision(GamePaddle & paddle);
    void checkCollision(GameBrick & brick);
    void move(GLfloat delta);

private:
    struct Collision
    {
        bool previous;
        bool current;

        Collision() : previous{false}, current{false}
        {
        }

        void shift()
        {
            previous = current;
            current = false;
        }
    };

    bool isInRange(GLfloat value, GLfloat minR, GLfloat maxR)
    {
        return minR <= value && value <= maxR;
    }

    void setCollided();
    void drawBall(GLuint pID);
    void drawCross(GLuint pID);
    void shiftCollisions();
    bool checkBorderCollision(GameBoard & board, GameBoard::BorderPlace place);
    void brickScored(GameBrick & brick, int row, int column);

    const GLfloat vbDataBall[28];
    const GLfloat cbDataBall[42];
    const GLfloat vbDataCross[8];
    const GLfloat cbDataCross[12];
    GLuint vertexBufferBall;
    GLuint colorBufferBall;
    GLuint vertexBufferCross;
    GLuint colorBufferCross;

    glm::mat2 scaleMatrix;
    glm::mat2 rotateMatrix;
    glm::vec2 transformVector;
    glm::vec2 normalVector;
    glm::vec2 velocity;
    GLfloat radius;
    GLfloat separator;
    Collision collidedPaddle;
    std::map<GameBoard::BorderPlace, Collision> collidedBoard;
    Collision collidedBricks[GameBrick::rowsNumber][GameBrick::columnsNumber];
    std::default_random_engine rand_eng;
    std::uniform_int_distribution<int> velocity_distrib;
    std::uniform_int_distribution<int> angle_distrib;
};

#endif
