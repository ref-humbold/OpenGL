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

enum class Colour : int
{
    Red,
    Green,
    Blue,
    Cyan,
    Magenta,
    Yellow
};

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

    std::map<BorderPlace, glm::vec2> normalVectors;

private:
    void drawBorderTriangles(GLuint pID);
    void drawOneTriangle(GLuint pID);
    void countNormalVectors();

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

    int reflection()
    {
        return reflect_distrib(rand_eng);
    }

    void restart();
    void draw(GLuint pID);

    void moveLeft(GLfloat delta)
    {
        transformVector[0] = std::max(-0.4f, transformVector[0] - velocity * delta);
    }

    void moveRight(GLfloat delta)
    {
        transformVector[0] = std::min(0.4f, transformVector[0] + velocity * delta);
    }

    GLfloat getPosX()
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

    void restart();
    void draw(GLuint pID);
    bool checkOutside();
    void checkCollision(GameBoard & board);
    void checkCollision(GamePaddle & paddle);
    void checkCollision(GameBrick & brick);
    void move(GLfloat delta);

private:
    bool isInRange(GLfloat value, GLfloat minR, GLfloat maxR)
    {
        return minR <= value && value <= maxR;
    }

    void drawBall(GLuint pID);
    void drawCross(GLuint pID);
    GLfloat distance(glm::vec2 point, glm::vec2 lineVector, glm::vec2 linePoint);
    void brickScored(GameBrick & brick, int row, int column,
                     std::vector<std::pair<int, int>> & bricksHit);

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
    glm::vec2 velocity;
    glm::vec2 normalVector;
    GLfloat radius;
    GLfloat separator;
    GLfloat velocityDistance;
    bool startingShot;
    std::vector<std::vector<std::pair<bool, bool>>> collided;
    std::default_random_engine rand_eng;
    std::uniform_int_distribution<int> velocity_distrib;
    std::uniform_int_distribution<int> angle_distrib;
};

#endif
