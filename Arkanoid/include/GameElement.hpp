#ifndef GAME_ELEMENT_HPP_
#define GAME_ELEMENT_HPP_

#include <cmath>
#include <cstdlib>
#include <iostream>
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
    std::vector<glm::vec2> normalVectors;

    GameBoard();

    void drawBackground(GLuint pID);
    void drawBorderTriangles(GLuint pID);
    void countNormalVectors();

private:
    void drawOneTriangle(GLuint pID);

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
    bool isVisible[6][13];
    int bricksLeft;

    GameBrick();

    void drawAllBricks(GLuint pID);

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
    void drawPaddle(GLuint pID);
    void moveLeft(GLfloat delta);
    void moveRight(GLfloat delta);
    GLfloat getPosX();
    GLfloat getSurf();

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
    void drawBall(GLuint pID);
    void drawCross(GLuint pID);
    bool isInRange(GLfloat value, GLfloat minR, GLfloat maxR);
    GLfloat countDistance(glm::vec2 pt, glm::vec2 nl, glm::vec2 pl);
    bool checkOutside();
    void checkCollisionBoard(GameBoard & board);
    void checkCollisionPaddle(GamePaddle & paddle);
    void checkCollisionBrick(GameBrick & brick);
    void moveBall(GLfloat delta);

private:
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
