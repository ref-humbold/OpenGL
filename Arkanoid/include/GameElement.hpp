#ifndef GAME_ELEMENT_HPP_
#define GAME_ELEMENT_HPP_

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace glm;

GLuint createVertexBuffer(const GLfloat vbData[], size_t size);

void loadBuffer(GLuint vb, GLuint cb);

class GameBoard
{
public:
    std::vector<vec2> normVecs;

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

    mat2 scaleMatrix;
    mat2 rotateMatrix;
    vec2 transformVector;
};

class GameBrick
{
public:
    std::vector<std::vector<bool>> isVisible;
    int bricksLeft;

    GameBrick();

    void drawAllBricks(GLuint pID);

private:
    void drawRect(GLuint pID, int col);
    void drawRectBorder(GLuint pID);

    const GLfloat vbDataRect[8];
    GLfloat cbDataRect[12];
    const GLfloat cbDataRectBorder[12];
    GLuint vertexBufferRect;
    GLuint colorBufferRect;
    GLuint colorBufferRectBorder;

    mat2 scaleMatrix;
    mat2 rotateMatrix;
    vec2 transformVector;
};

class GamePaddle
{
public:
    GamePaddle();

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

    mat2 scaleMatrix;
    mat2 rotateMatrix;
    vec2 transformVector;
    GLfloat velocity;
};

class GameBall
{
public:
    GameBall();

    void restart();
    void drawBall(GLuint pID);
    void drawCross(GLuint pID);
    bool isInRange(GLfloat value, GLfloat minR, GLfloat maxR);
    GLfloat countDistance(vec2 pt, vec2 nl, vec2 pl);
    bool checkOutside();
    void checkCollisionBoard(GameBoard & board);
    void checkCollisionPaddle(GamePaddle & paddle);
    void checkCollisionBrick(GameBrick & brick);
    void moveBall(GLfloat delta);

private:
    const GLfloat vbDataBall[28];
    const GLfloat cbDataBall[42];
    const GLfloat vbDataCross[8];
    const GLfloat cbDataCross[12];
    GLuint vertexBufferBall;
    GLuint colorBufferBall;
    GLuint vertexBufferCross;
    GLuint colorBufferCross;

    mat2 scaleMatrix;
    mat2 rotateMatrix;
    vec2 transformVector;
    vec2 velocity;
    vec2 vNorm;
    int angleMult;
    GLfloat radius;
    GLfloat separator;
    GLfloat velocityDistance;
    bool startingShot;
    std::vector<std::vector<std::pair<bool, bool>>> collided;
};

#endif
